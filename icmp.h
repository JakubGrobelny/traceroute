// Jakub Grobelny 300481

#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <stdbool.h>


uint16_t icmp_checksum(const void* buf, int length);

void init_icmp_packet(struct icmphdr* dest, int seq);

void send_packet(
    const struct icmphdr* packet, 
    int fd, 
    const struct sockaddr_in* dst
);

bool is_valid_ttl_exceeded_packet(const void* buffer, int ttl);

int was_recent(int seq, int ttl);

int sequence_number(int ttl, int i);


#endif