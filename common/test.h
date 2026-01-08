#pragma once

#include <iostream>
#include <cstdint>
#include <cstring>
#include <arpa/inet.h>
#include <chrono>
#include "packet.h"
#include "packet_serialization.h"
#include "crc32.h"
using namespace std;


inline uint64_t get_current_time(){
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

inline static void print_packet(const char* label = "Packet", const Packet& p = Packet()) {
    std::cout << label << ":\n";
    std::cout << "  magic       = 0x" << std::hex << p.magic_number << std::dec << "\n";
    std::cout << "  version     = " << p.version << "\n";
    std::cout << "  payload_len = " << p.payload_len << "\n";
    std::cout << "  timestamp_sent  = 0x" << std::hex << p.timestamp_sent<< std::dec << "\n";
    std::cout << "  temperature = " << p.temperature << "\n";
    std::cout << "  voltage     = " << p.voltage << "\n";
    std::cout << "  current     = " << p.current << "\n";
    std::cout << "  checksum    = 0x" << std::hex << p.checksum << std::dec << "\n";
    std::cout << "  sequence_number = " << p.sequence_number << "\n";
}


inline static size_t serialize_with_crc(const Packet& pkt, uint8_t* buffer) {
    size_t len = serialize_packet(pkt, buffer);
    uint32_t crc = crc32(buffer, len);

    uint32_t net_crc = htonl(crc);
    memcpy(buffer + len, &net_crc, sizeof(uint32_t));
    return len + sizeof(uint32_t);
}

inline static void dump_hex(const uint8_t* buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}