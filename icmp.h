#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>

#include <netinet/in.h>

typedef struct sockaddr_in sockaddr_in_t;

uint16_t icmp_checksum(const void *buf, int length);

void init_icmp_packet(const sockaddr_in_t* addr, void* buf, int seq);

#endif