#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>


typedef struct sockaddr_in address_t;
typedef struct icmphdr icmphdr_t;

uint16_t icmp_checksum(const void *buf, int length);

void init_icmp_packet(icmphdr_t* dest, const address_t* addr, int seq);

#endif