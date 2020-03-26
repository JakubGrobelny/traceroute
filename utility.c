// Jakub Grobelny 300481

#include "utility.h"

#include <stdlib.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char* translate_address(const struct sockaddr_in* addr, char buffer[20]) {
    const char* sender_ip = inet_ntop(
        AF_INET, 
        &addr->sin_addr,
        buffer, 
        20
    );

    if (sender_ip == NULL) {
        perror("inet_ntop");
        exit(EXIT_FAILURE);
    }

    return buffer;
}

void print_unique_responders(response_info_t* responders, int n) {
    for (int i = 0; i < n; i++) {
        const char* ip = responders[i].ip;
        bool unique = true;

        for (int j = 0; j < i; j++) {
            const char* other_ip = responders[j].ip;

            if (strcmp(ip, other_ip) == 0) {
                unique = false;
                break;
            } 
        }

        if (unique) {
            printf("%s ", ip);
        }
    }
}

void update_time(response_info_t* responder, struct timeval* time_left) {
    responder->time.tv_sec  = 1;
    responder->time.tv_usec = 0;
    timersub(&responder->time, time_left, &responder->time);
}