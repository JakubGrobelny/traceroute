#ifndef TRACEROUTE_H
#define TRACEROUTE_H

#include <netinet/in.h>

void traceroute(const struct sockaddr_in* dest, int socket_fd);

#endif