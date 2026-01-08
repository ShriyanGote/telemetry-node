#include <iostream>
#include <cstdint>
#include <cstring>
#include <arpa/inet.h>

#include <unistd.h>
#include "packet.h"
#include "packet_serialization.h"
#include "test.h"
using namespace std;



int main(){
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        cerr << "Error creating socket" << endl;
        return -1;
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(9000);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (const sockaddr*)&address, sizeof(address)) < 0) {
        cerr << "Error binding socket" << endl;
        close(sockfd);
        return -1;
    }

    cout << "udp listening on port 9000" << endl;
    static uint32_t last_sequence_number = UINT32_MAX;
    uint8_t buffer[512];
    while (true){
        size_t len = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
        Packet packet;
        if (!deserialize_packet(packet, buffer, len)) {
            cout << "deserialize failed" << endl;
            continue;
        }
        print_packet("packet",packet);
        
        if (packet.sequence_number != UINT32_MAX && packet.sequence_number != (last_sequence_number + 1)) {
            cout << "sequence number mismatch" << " expected: " << last_sequence_number + 1 << " got: " << packet.sequence_number << endl;
            continue;
        }
        last_sequence_number = packet.sequence_number;

        
    }
    close(sockfd);
    return 0;
}