#include "icmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <unistd.h>

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

void init_icmp_packet(const sockaddr_in_t* addr, void* buf, int seq) {
    assert(buf  != NULL);
    assert(addr != NULL);
    
    struct iphdr* ip_header = buf;
    char* packet = (char*)buf + ip_header->ihl * 4;
    struct icmphdr* icmp_header = (struct icmphdr*)packet;

    icmp_header->type = ICMP_ECHO;
    icmp_header->code = 0;
    icmp_header->un.echo.id = getpid();
    icmp_header->un.echo.sequence = seq;
    icmp_header->checksum = icmp_checksum(icmp_header, sizeof(struct icmphdr));
}