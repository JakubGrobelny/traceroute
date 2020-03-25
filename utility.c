#include <stdlib.h>

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>

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