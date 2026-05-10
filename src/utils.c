#include "../include/common.h"
#include "../include/utils.h"

void ip_to_string(uint32_t ip, char *buffer) {

    struct in_addr addr;
    addr.s_addr = ip;

    inet_ntop(AF_INET, &addr, buffer, INET_ADDRSTRLEN);
}

const char* protocol_to_string(uint8_t proto) {

    switch(proto) {

        case IPPROTO_TCP:
            return "TCP";

        case IPPROTO_UDP:
            return "UDP";

        default:
            return "OTHER";
    }
}