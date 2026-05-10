#include "../include/common.h"
#include "../include/parser.h"
#include "../include/flow_tracker.h"

int is_ipv4_packet(struct ethhdr *eth) {

    return ntohs(eth->h_proto) == ETH_P_IP;
}

void process_packet(char *buffer, ssize_t data_size) {

    if (data_size < (ssize_t)(sizeof(struct ethhdr) + sizeof(struct iphdr))) {
        return;
    }

    struct ethhdr *eth = (struct ethhdr *)buffer;

    if (!is_ipv4_packet(eth)) {
        return;
    }

    struct iphdr *ip =
        (struct iphdr *)(buffer + sizeof(struct ethhdr));

    if (ip->ihl < 5) {
        return;
    }

    int ip_header_len = ip->ihl * 4;

    flow_key_t key;

    key.src_ip = ip->saddr;
    key.dst_ip = ip->daddr;
    key.protocol = ip->protocol;

    if (ip->protocol == IPPROTO_TCP) {

        int offset =
            sizeof(struct ethhdr) + ip_header_len;

        if (data_size < (ssize_t)(offset + sizeof(struct tcphdr))) {
            return;
        }

        struct tcphdr *tcp =
            (struct tcphdr *)(buffer + offset);

        key.src_port = tcp->source;
        key.dst_port = tcp->dest;
    }

    else if (ip->protocol == IPPROTO_UDP) {

        int offset =
            sizeof(struct ethhdr) + ip_header_len;

        if (data_size < (ssize_t)(offset + sizeof(struct udphdr))) {
            return;
        }

        struct udphdr *udp =
            (struct udphdr *)(buffer + offset);

        key.src_port = udp->source;
        key.dst_port = udp->dest;
    }

    else {
        return;
    }

    normalize_flow(&key);

    update_flow(&key, data_size);
}