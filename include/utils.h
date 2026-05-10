#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

void ip_to_string(uint32_t ip, char *buffer);

const char* protocol_to_string(uint8_t proto);

#endif