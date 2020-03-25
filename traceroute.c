#include "traceroute.h"
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>


static int sequence_number(int ttl, int i) {
    return ttl << 2 | i;
}

static void send_packet(icmphdr_t* packet, int fd, const address_t* dest) {
    ssize_t sent = sendto(
        fd,
        &packet,
        sizeof(packet),
        0,
        (struct sockaddr*)dest,
        sizeof(struct sockaddr_in)
    );

    if (sent == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    assert(sent == sizeof(packet));
}

void traceroute(const address_t* dest, int socket_fd) {
    for (int ttl = 1; ttl <= 30; ttl++) {
        for (int i = 0; i < 3; i++) {
            struct icmphdr packet = {0};
            init_icmp_packet(&packet, dest, sequence_number(ttl, i));
            setsockopt(socket_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));

            send_packet(&packet, socket_fd, dest);
            #ifdef DEBUG
            printf(
                "Sent packet [ttl: %d] [id: %d] [seq: %d]\n",
                ttl, getpid(), sequence_number(ttl, i)
            );
            #endif
        }

        // TODO: wait for three packets up to 1 second


        // TODO: calculate the average time for each router that responded
        // TODO: if the response came from destination address then abort
    }

}
