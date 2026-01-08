#pragma once

#include <iostream>
#include <cstdint>
#include <cstring>
#include <arpa/inet.h>

using namespace std;


// consteval has to be a compile-time constant expression
// constexpr CAN be computed at compile time
// const can be computed at compile or runtime

constexpr uint32_t MAGIC_NUMBER = 0x12345678;
constexpr uint32_t VERSION = 0x00000001;


struct Packet {
    uint32_t magic_number; // 0x12345678
    uint32_t version;
    uint32_t payload_len;
    uint32_t sequence_number;
    uint64_t timestamp;
    float temperature;
    float voltage;
    float current;
    uint32_t checksum;
} __attribute__((packed));

static_assert(sizeof(Packet) == 40, "Packet size is not 40 bytes");








