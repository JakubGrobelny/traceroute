#include "traceroute.h"
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/select.h>

typedef enum await_result {
    TIMEOUT,
    AVALIABLE
} await_result_t;

static int sequence_number(int ttl, int i) {
    return ttl << 2 | i;
}

static int was_recent(int seq, int ttl) {
    return seq >> 2 == ttl;
}

static void send_packet(struct icmphdr* packet, int fd, const struct sockaddr_in* dest) {
    ssize_t sent = sendto(
        fd,
        packet,
        sizeof(struct icmphdr),
        0,
        (struct sockaddr*)dest,
        sizeof(struct sockaddr_in)
    );

    if (sent == -1) {
        fprintf(stderr, "sendto: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    assert(sent == sizeof(packet));
}

static await_result_t await_packets(fd_set* fds, struct timeval* time) {
    int result = select(1, fds, NULL, NULL, time);

    if (result == -1) {
        fprintf(stderr, "select: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    } else if (result == 0) {
        return TIMEOUT;
    }

    return AVALIABLE;
}

static void receive_packets(int socket_fd, int ttl) {
    int received = 0;

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(socket_fd, &fds);

    struct timeval time;
    time.tv_sec  = 1;
    time.tv_usec = 0;

    while (received < 3) {
        if (await_packets(&fds, &time) == TIMEOUT) {
            #ifdef DEBUG
            printf("TIMEOUT [ttl: %d]\n", ttl);
            #endif
            break;
        }

        struct sockaddr_in sender;
        socklen_t sender_size = sizeof(sender);
        uint8_t buffer[IP_MAXPACKET];

        ssize_t packet_size = recvfrom(
            socket_fd, 
            buffer, 
            IP_MAXPACKET, 
            0, 
            (struct sockaddr*)&sender, 
            &sender_size
        );

        if (packet_size == -1) {
            fprintf(stderr, "recvfrom: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        char sender_ip_buf[20];
        const char* sender_ip = inet_ntop(
            AF_INET, 
            &sender.sin_addr,
            sender_ip_buf, 
            sizeof(sender_ip)
        );

        if (sender_ip == NULL) {
            fprintf(stderr, "inet_ntop: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        #ifdef DEBUG
        printf("[ttl: %d] %s\n", ttl, sender_ip);
        #endif

        received++;
    }

    #ifdef DEBUG
    printf("[received: %d]\n", received);
    #endif
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
                fprintf(stderr, "setsockopt: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }            
            
            send_packet(&packet, socket_fd, dest);

            #ifdef DEBUG
            printf(
                "Sent packet [ttl: %d] [id: %d] [seq: %d]\n",
                ttl, getpid(), sequence_number(ttl, i)
            );
            #endif
        }

        // TODO: wait for three packets up to 1 second
        // receive_packets(socket_fd, ttl);

        // TODO: calculate the average time for each router that responded
        // TODO: if the response came from destination address then abort
    }
}
