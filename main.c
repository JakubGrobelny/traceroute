#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./traceroute [address]\n");
        return EXIT_FAILURE;
    }

    char* address = argv[1];

    printf("%s\n", address);

    return 0;
}