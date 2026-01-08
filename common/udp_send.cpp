#include <iostream>
#include <cstdint>
#include <cstring>
#include <arpa/inet.h>

#include <unistd.h>
#include "packet.h"
#include "packet_serialization.h"
#include "test.h"
#include "udp_socket.h"
#include "../telemetry_node/TelemetryNode.h"

using namespace std;

size_t serialize_crc(const Packet& packet, uint8_t* buffer){
    size_t len = serialize_packet(packet, buffer);
    uint32_t crc = crc32(buffer, len);
    uint32_t net_crc = htonl(crc);
    memcpy(buffer + len, &net_crc, sizeof(uint32_t));
    return len + sizeof(uint32_t);
}


int main(){
    UdpSocket socket("127.0.0.1", 9000);
    float voltage_temp = 3.3f;
    static uint32_t sequence_number = 0;
    TelemetryNode telemetry_node;

    cout << "udp sending to port 9000" << endl;
    while (true){
        telemetry_node.run();
    }
    return 0;
}