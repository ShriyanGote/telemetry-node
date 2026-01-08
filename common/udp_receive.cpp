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



int main(){
    UdpSocket socket(9000);
    cout << "udp listening on port 9000" << endl;
    static uint32_t last_sequence_number = UINT32_MAX;
    uint8_t buffer[512];
    while (true){
        size_t len = socket.receive(buffer, sizeof(buffer));


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
        uint64_t current_time = get_current_time();
        uint32_t latency = current_time - packet.timestamp_sent;
        double latency_ms = latency / 1000000.0;
        cout << "current time: " << current_time << endl;
        cout << "packet timestamp sent: " << packet.timestamp_sent << endl;
        cout << "latency: " << latency_ms << " ms" << endl;

        last_sequence_number = packet.sequence_number;

        
    }
    socket.~UdpSocket();
    return 0;
}