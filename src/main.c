#include "../include/common.h"
#include "../include/capture.h"
#include "../include/parser.h"
#include "../include/flow_tracker.h"

#include <time.h>

int main() {

    int sockfd;
    char buffer[BUFFER_SIZE];

    time_t last_print = time(NULL);

    sockfd = create_raw_socket();

    while (1) {

        ssize_t data_size =
            capture_packet(sockfd, buffer);

        if (data_size < 0) {
            perror("recvfrom");
            continue;
        }

        process_packet(buffer, data_size);

        if (time(NULL) - last_print >= 5) {

            print_flows();

            last_print = time(NULL);
        }
    }

    close(sockfd);

    return 0;
}