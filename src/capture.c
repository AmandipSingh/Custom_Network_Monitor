#include "../include/common.h"
#include "../include/capture.h"

int create_raw_socket() {

    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

ssize_t capture_packet(int sockfd, char *buffer) {

    struct sockaddr saddr;
    socklen_t saddr_len = sizeof(saddr);

    ssize_t data_size = recvfrom(
        sockfd,
        buffer,
        BUFFER_SIZE,
        0,
        &saddr,
        &saddr_len
    );

    return data_size;
}