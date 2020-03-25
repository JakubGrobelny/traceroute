#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "icmp.h"
#include "traceroute.h"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./traceroute [address]\n");
        return EXIT_FAILURE;
    }

    const char* address = argv[1];

    struct sockaddr_in addr = {0};
    if (inet_pton(AF_INET, address, &addr.sin_addr) == 0) {
        fprintf(stderr, "'%s' is not a valid network address!\n", address);
        return EXIT_FAILURE;
    }

    int socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (socket_fd == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    traceroute(&addr, socket_fd);

    close(socket_fd);
    return 0;
}