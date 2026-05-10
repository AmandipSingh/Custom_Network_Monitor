#ifndef PARSER_H
#define PARSER_H

#include <sys/types.h>

int is_ipv4_packet(struct ethhdr *eth);

void process_packet(
    char *buffer,
    ssize_t data_size
);

#endif