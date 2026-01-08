#include "../common/udp_socket.h"
#include "../common/packet.h"
#include "../common/packet_serialization.h"
#include "../common/test.h"
#include "../telemetry_node/TelemetryNode.h"
#include <unistd.h>


using namespace std;

TelemetryNode::TelemetryNode()
    : socket_("127.0.0.1", 9000), sequence_number_(0)
{
    cout << "TelemetryNode initialized" << endl;
}

TelemetryNode::~TelemetryNode()
{
    cout << "TelemetryNode destroyed" << endl;
}

void TelemetryNode::run()
{
    while (true){
        send_packet();
        sleep(1);
    }
    cout << "TelemetryNode running" << endl;
}

void TelemetryNode::send_packet()
{
    Packet packet;
    packet.magic_number = MAGIC_NUMBER;
    packet.version = VERSION;
    packet.payload_len = 0;
    packet.timestamp_sent = get_current_time();
    packet.sequence_number = sequence_number_++;
    uint8_t buffer[512];
    size_t len = serialize_with_crc(packet, buffer);
    socket_.send(buffer, len);
    cout << "packet sent" << " sequence number: " << packet.sequence_number << endl;
    print_packet("packet",packet);
}

