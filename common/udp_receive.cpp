#include <iostream>
#include <cstdint>
#include <cstring>
#include <arpa/inet.h>

#include <unistd.h>
#include "packet.h"
#include "packet_serialization.h"

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

    cout << "udp listening on port 9000" << endl;
    uint8_t buffer[512];
    while (true){
        size_t len = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
        Packet packet;
        deserialize_packet(packet, buffer, len);
        cout << "received packet: " << packet.magic_number << " " << packet.version << " " << packet.payload_len << " " << packet.timestamp << " " << packet.temperature << " " << packet.voltage << " " << packet.current << " " << packet.checksum << endl;
        close(sockfd);
        return 0;
    }


}