#define _GNU_SOURCE
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <time.h>

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

#define TABLE_SIZE 1024

flow_entry_t* flow_table[TABLE_SIZE] = {0};

void process_tcp_packet(struct tcphdr *tcp, flow_key_t *key, ssize_t data_size);
void process_udp_packet(struct udphdr *udp, flow_key_t *key, ssize_t data_size);
void print_flows();
unsigned int hash(flow_key_t *key);
void normalize_flow(flow_key_t *key);
void update_flow(flow_key_t *key, ssize_t size);