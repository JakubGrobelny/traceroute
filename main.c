#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void make_icmp_packet(const struct sockaddr_in* addr, int ttl, void* buf) {
    // TODO: implement
    fprintf(stderr, "make_icmp_packet: not implemented!\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./traceroute [address]\n");
        return EXIT_FAILURE;
    }

    const char* address = argv[1];
    const pid_t pid = getpid();

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

    for (int ttl = 1; ttl <= 30; ttl++) {
        char buffer[IP_MAXPACKET] = {0};
        make_icmp_packet(&addr, ttl, buffer);
        // TODO: send three packets
        // TODO: wait for three packets up to 1 second
        // TODO: calculate the average time for each router that responded
        // TODO: if the response came from destination address then abort
    }

    printf("pid: [%d], socket: [%d], address:[%s]\n", pid, socket_fd, address);

    close(socket_fd);
    return 0;
}