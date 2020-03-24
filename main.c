#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./traceroute [address]\n");
        return EXIT_FAILURE;
    }

    const char* address = argv[1];
    const pid_t pid = getpid();

    struct sockaddr_in addr;
    if (inet_pton(AF_INET, address, &addr.sin_addr) == 0) {
        fprintf(stderr, "'%s' is not a valid network address!\n", address);
        return EXIT_FAILURE;
    }

    int socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (socket_fd == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    printf("pid: [%d], socket: [%d], address:[%s]\n", pid, socket_fd, address);

    return 0;
}