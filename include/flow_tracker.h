#ifndef FLOW_TRACKER_H
#define FLOW_TRACKER_H

#include "common.h"

extern flow_entry_t* flow_table[TABLE_SIZE];

unsigned int hash(flow_key_t *key);

void normalize_flow(flow_key_t *key);

void update_flow(flow_key_t *key, ssize_t size);

void print_flows();

#endif