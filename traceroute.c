// Jakub Grobelny 300481

#include "traceroute.h"
#include "utility.h"

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdbool.h>



static await_result_t await_packets(int socket_fd, struct timeval* time) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(socket_fd, &fds);

    int result = select(socket_fd + 1, &fds, NULL, NULL, time);

    if (result == -1) {
        perror("select");
        exit(EXIT_FAILURE);
    } else if (result == 0) {
        return TIMEOUT;
    }

    return AVALIABLE;
}


static bool receive_packets(int socket_fd, int ttl) {
    int received = 0;

    struct timeval time;
    time.tv_sec  = 1;
    time.tv_usec = 0;

    bool target_responded = false;
    response_info_t responders[3];

    printf("%d. ", ttl);

    while (received < 3) {
        if (await_packets(socket_fd, &time) == TIMEOUT) {
            break;
        }

        struct sockaddr_in sender;
        socklen_t sender_size = sizeof(sender);
        uint8_t buffer[IP_MAXPACKET] = {0};

        // Uwaga: przez to że przed każdym wywołaniem recvfrom
        // używany jest select, to flaga MSG_DONTWAIT nie jest
        // potrzebna (mamy gwarancję, że czeka na nas pakiet).
        ssize_t packet_size = recvfrom(
            socket_fd,  
            buffer, 
            IP_MAXPACKET, 
            0, 
            (struct sockaddr*)&sender, 
            &sender_size
        );

        if (packet_size == -1) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        translate_address(&sender, responders[received].ip);

        struct iphdr* ip_header = (struct iphdr*)buffer;
        ssize_t ip_header_size  = 4 * ip_header->ihl;

        struct icmphdr* icmp_header = (struct icmphdr*)(buffer+ip_header_size);
        int id = icmp_header->un.echo.id;
        int seq = icmp_header->un.echo.sequence;

        if (icmp_header->type == ICMP_ECHOREPLY) {
            if (id != getpid() || !was_recent(seq, ttl)) {
                continue;
            }
            target_responded = true;
        } else if (icmp_header->type == ICMP_TIME_EXCEEDED) {
            void* payload = (uint8_t*)icmp_header + sizeof(struct icmphdr);
            if (!is_valid_ttl_exceeded_packet(payload, ttl)) {
                continue;
            }
        } else {
            continue;
        }

        update_time(&responders[received], &time);
        received++;
    }

    print_unique_responders(responders, received);

    if (received == 0) {
        printf("*\n");
    } else if (received < 3) {
        printf("???\n");
    } else {
        print_responders_avg_time(responders, received);
    }

    return target_responded;
}


void traceroute(const struct sockaddr_in* dest, int socket_fd) {
    for (int ttl = 1; ttl <= 30; ttl++) {
        for (int i = 0; i < 3; i++) {
            struct icmphdr packet;
            init_icmp_packet(&packet, sequence_number(ttl, i));
            
            int ttl_set = setsockopt(
                socket_fd, 
                IPPROTO_IP, 
                IP_TTL, 
                &ttl, 
                sizeof(int)
            );

            if (ttl_set == -1) {
                perror("setsockopt");
                exit(EXIT_FAILURE);
            }            
            
            send_packet(&packet, socket_fd, dest);
        }

        if (receive_packets(socket_fd, ttl)) {
            return;
        }
    }
}
