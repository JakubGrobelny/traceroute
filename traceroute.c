#include "traceroute.h"
#include "icmp.h"
#include <arpa/inet.h>
#include <netinet/ip.h>


void traceroute(const struct sockaddr_in* dest, int socket_fd) {
    for (int ttl = 1; ttl <= 30; ttl++) {
        for (int i = 0; i < 3; i++) {
            char buffer[IP_MAXPACKET] = {0};
            init_icmp_packet(dest, buffer, ttl * 100 + i);


        }

        
        // TODO: send three packets
        // TODO: wait for three packets up to 1 second
        // TODO: calculate the average time for each router that responded
        // TODO: if the response came from destination address then abort
    }

}
