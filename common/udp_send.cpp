#include <iostream>
#include <cstdint>
#include <cstring>
#include <arpa/inet.h>

#include <unistd.h>
#include "packet.h"
#include "packet_serialization.h"
#include "test.cpp"

using namespace std;

size_t serialize_crc(const Packet& packet, uint8_t* buffer){
    size_t len = serialize_packet(packet, buffer);
    uint32_t crc = crc32(buffer, len);
    uint32_t net_crc = htonl(crc);
    memcpy(buffer + len, &net_crc, sizeof(uint32_t));
    return len + sizeof(uint32_t);
}


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
    float voltage_temp = 3.3f;
    static uint32_t sequence_number = 0;

    cout << "udp sending to port 9000" << endl;
    while (true){
        Packet packet;
        packet.magic_number = MAGIC_NUMBER;
        packet.version = VERSION;
        packet.payload_len = 0;
        packet.timestamp = time(nullptr);
        packet.temperature = 123.456f;
        packet.voltage = voltage_temp;
        packet.current = 1.25f;
        packet.sequence_number = sequence_number++;
        voltage_temp += 0.01f;
        uint8_t buffer[512];
        size_t len = serialize_crc(packet, buffer);
        sendto(sockfd, buffer, len, 0, (const sockaddr*)&address, sizeof(address));
        cout << "packet sent" << " sequence number: " << packet.sequence_number << endl;
        sleep(1);
    }
    close(sockfd);
    return 0;
}