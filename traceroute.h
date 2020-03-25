#ifndef TRACEROUTE_H
#define TRACEROUTE_H

#include <netinet/in.h>
#include "icmp.h"


void traceroute(const address_t* dest, int socket_fd);

#endif