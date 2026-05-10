#include <stdio.h>
#include <arpa/inet.h>
#include "../include/flow_tracker.h"
#include "../include/exporter.h"
void export_flows_to_csv() {

    FILE *fp = fopen("output/flows.csv", "w");

    if (!fp) {
        perror("fopen");
        return;
    }

    fprintf(fp,
        "src_ip,src_port,dst_ip,dst_port,protocol,bytes,packets\n");

    for (int i = 0; i < TABLE_SIZE; i++) {

        flow_entry_t *entry = flow_table[i];

        while (entry) {

            char src_ip[INET_ADDRSTRLEN];
            char dst_ip[INET_ADDRSTRLEN];

            inet_ntop(AF_INET,
                      &entry->key.src_ip,
                      src_ip,
                      sizeof(src_ip));

            inet_ntop(AF_INET,
                      &entry->key.dst_ip,
                      dst_ip,
                      sizeof(dst_ip));

            fprintf(fp,
                "%s,%u,%s,%u,%u,%lu,%lu\n",
                src_ip,
                ntohs(entry->key.src_port),
                dst_ip,
                ntohs(entry->key.dst_port),
                entry->key.protocol,
                entry->stats.bytes,
                entry->stats.packets);

            entry = entry->next;
        }
    }

    fclose(fp);
}