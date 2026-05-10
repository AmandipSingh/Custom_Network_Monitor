#include "helper.h"

const char* protocol_to_string(uint8_t proto) {
    switch(proto) {
        case IPPROTO_TCP: return "TCP";
        case IPPROTO_UDP: return "UDP";
        default: return "OTHER";
    }
}