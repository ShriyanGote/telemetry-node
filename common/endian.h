#pragma once 

#include <cstdint>
#include <arpa/inet.h>

using namespace std;

inline uint64_t host_to_network_long_long(uint64_t value) {
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        return (static_cast<uint64_t>(htonl(value & 0xFFFFFFFF)) << 32) |
            htonl(value >> 32);
    #else
        return value;
    #endif
}

inline uint64_t network_to_host_long_long(uint64_t value) {
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        return (static_cast<uint64_t>(ntohl(value & 0xFFFFFFFF)) << 32) |
            ntohl(value >> 32);
    #else
        return value;
    #endif
}

