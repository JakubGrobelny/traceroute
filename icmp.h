#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>
#include <netinet/in.h>


uint16_t icmp_checksum(const void *buf, int length);

void init_icmp_packet(const struct sockaddr_in* addr, void* buf, int seq);

#endif