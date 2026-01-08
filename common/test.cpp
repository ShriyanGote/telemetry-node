#include <iostream>
#include <cstddef>
#include "crc32.cpp"
#include "endian.h"
#include "packet.h"

using namespace std;

int main() {
    Packet packet = {MAGIC_NUMBER, VERSION, 0, 0, 0, 0, 0, 0};
    packet.payload_len = sizeof(packet) - offsetof(Packet, checksum);
    packet.checksum = 0;
    packet.checksum = crc32(
        reinterpret_cast<const uint8_t *>(&packet),
        offsetof(Packet, checksum)
    );
    cout << "CRC: " << packet.checksum << endl;
    cout << "Packet: " << packet.magic_number << " " << packet.version << " " << packet.payload_len << " " << packet.timestamp << " " << packet.temperature << " " << packet.voltage << " " << packet.current << " " << packet.checksum << endl;
    cout << "Packet size: " << sizeof(packet) << endl;
    cout << "Packet magic number: " << packet.magic_number << endl;
    cout << "Packet version: " << packet.version << endl;
    cout << "Packet payload len: " << packet.payload_len << endl;
    cout << "Packet timestamp: " << packet.timestamp << endl;
    cout << "Packet temperature: " << packet.temperature << endl;
    cout << "Packet voltage: " << packet.voltage << endl;
    cout << "Packet current: " << packet.current << endl;
    return 0;
}   