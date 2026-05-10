#ifndef CAPTURE_H
#define CAPTURE_H

int create_raw_socket();
ssize_t capture_packet(int sockfd, char *buffer);

#endif