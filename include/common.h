#ifndef COMMON_H
#define COMMON_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <net/ethernet.h>

#define BUFFER_SIZE 65536
#define TABLE_SIZE 1024

typedef struct {
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t protocol;
} flow_key_t;

typedef struct {
    uint64_t bytes;
    uint64_t packets;
} flow_stats_t;

typedef struct flow_entry {
    flow_key_t key;
    flow_stats_t stats;
    struct flow_entry *next;
} flow_entry_t;

#endif