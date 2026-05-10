#include "../include/common.h"
#include "../include/flow_tracker.h"
#include "../include/utils.h"

flow_entry_t* flow_table[TABLE_SIZE] = {0};

unsigned int hash(flow_key_t *key) {

    unsigned int h = key->src_ip;

    h = (h * 31) + key->dst_ip;
    h = (h * 31) + key->src_port;
    h = (h * 31) + key->dst_port;
    h = (h * 31) + key->protocol;

    return h % TABLE_SIZE;
}

void normalize_flow(flow_key_t *key) {

    if (ntohl(key->src_ip) > ntohl(key->dst_ip)) {

        uint32_t temp_ip = key->src_ip;
        key->src_ip = key->dst_ip;
        key->dst_ip = temp_ip;

        uint16_t temp_port = key->src_port;
        key->src_port = key->dst_port;
        key->dst_port = temp_port;
    }
    else if (key->src_ip == key->dst_ip &&
             ntohs(key->src_port) > ntohs(key->dst_port)) {

        uint16_t temp_port = key->src_port;
        key->src_port = key->dst_port;
        key->dst_port = temp_port;
    }
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

    flow_entry_t *new_entry = malloc(sizeof(flow_entry_t));

    if (!new_entry) {
        perror("malloc");
        return;
    }

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

            ip_to_string(entry->key.src_ip, src_ip);
            ip_to_string(entry->key.dst_ip, dst_ip);

            printf(
                "%s:%d → %s:%d | %s | Bytes: %lu | Packets: %lu\n",
                src_ip,
                ntohs(entry->key.src_port),
                dst_ip,
                ntohs(entry->key.dst_port),
                protocol_to_string(entry->key.protocol),
                entry->stats.bytes,
                entry->stats.packets
            );

            entry = entry->next;
        }
    }
}