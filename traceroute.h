// Jakub Grobelny 300481

#ifndef TRACEROUTE_H
#define TRACEROUTE_H

#include <netinet/in.h>
#include "icmp.h"


typedef enum await_result {
    TIMEOUT,
    AVALIABLE
} await_result_t;

void traceroute(const struct sockaddr_in* dest, int socket_fd);

#endif