
#include "sniffer.h"
#include "helper.h"

void process_tcp_packet(struct tcphdr *tcp, flow_key_t *key, ssize_t data_size){
    key->src_port = tcp->source;
    key->dst_port = tcp->dest;
}

void process_udp_packet(struct udphdr *udp, flow_key_t *key, ssize_t data_size){
    key->src_port = udp->source;
    key->dst_port = udp->dest;
}

unsigned int hash(flow_key_t *key) {
    unsigned int h = key->src_ip;
    h = (h * 31) + key->dst_ip;
    h = (h * 31) + key->src_port;
    h = (h * 31) + key->dst_port;
    h = (h * 31) + key->protocol;
    return h % TABLE_SIZE;
}

void update_flow(flow_key_t *key, ssize_t size) {
    unsigned int index = hash(key);

    flow_entry_t *entry = flow_table[index];

    while (entry) {
        if (memcmp(&entry->key, key, sizeof(flow_key_t)) == 0) {
            entry->stats.bytes += size;
            entry->stats.packets += 1;
            return;
        }
        entry = entry->next;
    }

    // New flow
    // TODO: implement cleanup or LRU eviction for long-running capture
    flow_entry_t *new_entry = malloc(sizeof(flow_entry_t)); 
    new_entry->key = *key;
    new_entry->stats.bytes = size;
    new_entry->stats.packets = 1;
    new_entry->next = flow_table[index];

    flow_table[index] = new_entry;
}

void print_flows() {
    printf("\n==== Flow Table ====\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        flow_entry_t *entry = flow_table[i];

        while (entry) {
            char src_ip[INET_ADDRSTRLEN];
            char dst_ip[INET_ADDRSTRLEN];

            inet_ntop(AF_INET, &entry->key.src_ip, src_ip, sizeof(src_ip));
            inet_ntop(AF_INET, &entry->key.dst_ip, dst_ip, sizeof(dst_ip));

            printf("%s:%d → %s:%d | Proto: %d | Bytes: %lu | Packets: %lu\n",
                src_ip,
                ntohs(entry->key.src_port),
                dst_ip,
                ntohs(entry->key.dst_port),
                entry->key.protocol,
                entry->stats.bytes,
                entry->stats.packets
            );

            entry = entry->next;
        }
    }
}

int main() {
    time_t last_print = time(NULL);
    int sockfd;
    char buffer[65536];
    struct sockaddr saddr;
    
    struct sockaddr_in src, dest;
    int saddr_len = sizeof(saddr);

    // Create a raw socket
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    while (1) {
        // Receive a packet
        ssize_t data_size = recvfrom(sockfd, buffer, sizeof(buffer), 0, &saddr, &saddr_len);
        if (data_size < 0) {
            perror("recvfrom");
            return 1;
        }

        

        struct ethhdr *eth = (struct ethhdr*)buffer;

        if(ntohs(eth->h_proto) != ETH_P_IP) {
            continue; // Skip non-IP packets
        }


        struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));

        if (data_size < sizeof(struct ethhdr) + sizeof(struct iphdr)) {
            continue;
        }

        if (ip->ihl < 5) {
            continue;
        }

        int ip_header_len = ip->ihl * 4;


        memset(&src, 0, sizeof(src));
            src.sin_addr.s_addr = ip->saddr;

        memset(&dest, 0, sizeof(dest));
        dest.sin_addr.s_addr = ip->daddr;

        char src_ip[INET_ADDRSTRLEN];
        char dst_ip[INET_ADDRSTRLEN];

        inet_ntop(AF_INET, &ip->saddr, src_ip, sizeof(src_ip));
        inet_ntop(AF_INET, &ip->daddr, dst_ip, sizeof(dst_ip));

        flow_key_t key;

        key.src_ip = ip->saddr;
        key.dst_ip = ip->daddr;
        key.protocol = ip->protocol;
        

        if (ip->protocol == IPPROTO_TCP) {

            int header_offset = sizeof(struct ethhdr) + ip_header_len;

            if (data_size < header_offset + sizeof(struct tcphdr)) continue;

            struct tcphdr *tcp = (struct tcphdr *)(buffer + sizeof(struct ethhdr) + ip_header_len);

            process_tcp_packet(tcp, &key, data_size);

            update_flow(&key, data_size);
        }
        else if (ip->protocol == IPPROTO_UDP) {

            int header_offset = sizeof(struct ethhdr) + ip_header_len;

            if (data_size < header_offset + sizeof(struct udphdr)) continue;

            struct udphdr *udp = (struct udphdr *)(buffer + sizeof(struct ethhdr) + ip_header_len);

            process_udp_packet(udp, &key, data_size);

            update_flow(&key, data_size);
        }

        if (time(NULL) - last_print >= 5) {
            print_flows();
            last_print = time(NULL);
        }
    }

    close(sockfd);
    return 0;
}