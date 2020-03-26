// Jakub Grobelny 300481

#ifndef UTILITY_H
#define UTILITY_H

#include <sys/time.h>
#include <netinet/ip.h>

typedef struct response_info {
    char ip[20];
    struct timeval time;
} response_info_t;

char* translate_address(const struct sockaddr_in* addr, char buffer[20]);

void print_unique_responders(response_info_t* responders, int n);

void update_time(response_info_t* responder, struct timeval* time_left);

void print_responders_avg_time(response_info_t* responders, int n);

#endif