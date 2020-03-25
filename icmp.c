// Jakub Grobelny 300481

#include "icmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <string.h>


uint16_t icmp_checksum(const void* buf, int length) {
    assert (length % 2 == 0);

    uint16_t sum = 0;
    const uint16_t* ptr = buf;

    while (length > 0) {
        sum += *ptr++;
        length -= 2;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    return ~(sum + (sum >> 16));
}

void init_icmp_packet(struct icmphdr* dest, int seq) {
    assert(dest != NULL);

    dest->type = ICMP_ECHO;
    dest->code = 0;
    dest->un.echo.id = getpid();
    dest->un.echo.sequence = seq;
    dest->checksum = 0;
    dest->checksum = icmp_checksum(dest, sizeof(struct icmphdr));
}

int sequence_number(int ttl, int i) {
    return ttl << 2 | i;
}

int was_recent(int seq, int ttl) {
    return seq >> 2 == ttl;
}

void send_packet(
    const struct icmphdr* packet, 
    int fd, 
    const struct sockaddr_in* dest
) {
    ssize_t sent = sendto(
        fd,
        packet,
        sizeof(struct icmphdr),
        0,
        (struct sockaddr*)dest,
        sizeof(struct sockaddr_in)
    );

    if (sent == -1) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
    assert(sent == sizeof(packet));
}

bool is_valid_ttl_exceeded_packet(const void* buffer, int ttl) {
    struct iphdr* ip_header = (struct iphdr*)buffer;
    ssize_t ip_header_size  = 4 * ip_header->ihl;

    struct icmphdr* icmp_header = (
        struct icmphdr*)((uint8_t*)buffer + ip_header_size
    );
    int id = icmp_header->un.echo.id;
    int seq = icmp_header->un.echo.sequence;

    return id == getpid() && was_recent(seq, ttl);
}
