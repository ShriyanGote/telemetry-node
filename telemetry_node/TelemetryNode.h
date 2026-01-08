#pragma once

#include "../common/udp_socket.h"
#include "../common/packet.h"
#include "../common/packet_serialization.h"
#include "../common/test.h"

class TelemetryNode {
    public:
    TelemetryNode();
    ~TelemetryNode();
    void run();
    private:
    void send_packet();
    UdpSocket socket_;
    uint32_t sequence_number_;
};