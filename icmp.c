#include "icmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <netinet/ip.h>
#include <unistd.h>

const size_t icmp_packet_size = sizeof(struct icmphdr);

uint16_t icmp_checksum(const void *buf, int length) {
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

void init_icmp_packet(icmphdr_t* dest, const address_t* addr, int seq) {
    assert(dest != NULL);
    assert(addr != NULL);

    dest->type = ICMP_ECHO;
    dest->code = 0;
    dest->un.echo.id = getpid();
    dest->un.echo.sequence = seq;
    dest->checksum = icmp_checksum(dest, sizeof(struct icmphdr));
}