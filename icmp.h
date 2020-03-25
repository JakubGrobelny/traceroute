#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>


uint16_t icmp_checksum(const void *buf, int length);

void init_icmp_packet(struct icmphdr* dest, int seq);

#endif