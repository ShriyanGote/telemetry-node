#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>

#include "packet.h"
#include "packet_serialization.h"
#include "crc32.h"

// ---- helpers -------------------------------------------------------------

static void print_packet(const char* label = "Packet", const Packet& p) {
    std::cout << label << ":\n";
    std::cout << "  magic       = 0x" << std::hex << p.magic_number << std::dec << "\n";
    std::cout << "  version     = " << p.version << "\n";
    std::cout << "  payload_len = " << p.payload_len << "\n";
    std::cout << "  timestamp   = 0x" << std::hex << p.timestamp << std::dec << "\n";
    std::cout << "  temperature = " << p.temperature << "\n";
    std::cout << "  voltage     = " << p.voltage << "\n";
    std::cout << "  current     = " << p.current << "\n";
    std::cout << "  checksum    = 0x" << std::hex << p.checksum << std::dec << "\n";
}


static size_t serialize_with_crc(const Packet& pkt, uint8_t* buffer) {
    size_t len = serialize_packet(pkt, buffer);
    uint32_t crc = crc32(buffer, len);

    uint32_t net_crc = htonl(crc);
    memcpy(buffer + len, &net_crc, sizeof(uint32_t));
    return len + sizeof(uint32_t);
}

static void dump_hex(const uint8_t* buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

// ---- tests ---------------------------------------------------------------

void test_roundtrip() {
    std::cout << "[TEST] roundtrip\n";

    Packet in{};
    in.magic_number = MAGIC_NUMBER;
    in.version = VERSION;
    in.payload_len = 0;
    in.timestamp = 0x1122334455667788ULL;
    in.temperature = 123.456f;
    in.voltage = 3.3f;
    in.current = 1.25f;

    uint8_t buffer[64];
    size_t len = serialize_with_crc(in, buffer);

    std::cout << "Serialized buffer (" << len << " bytes):\n";
    dump_hex(buffer, len);

    Packet out{};
    bool ok = deserialize_packet(out, buffer, len);
    assert(ok);

    print_packet("Input packet", in);
    print_packet("Output packet", out);

    assert(out.magic_number == in.magic_number);
    assert(out.version == in.version);
    assert(out.timestamp == in.timestamp);
    assert(out.temperature == in.temperature);
    assert(out.voltage == in.voltage);
    assert(out.current == in.current);
}


void test_crc_corruption() {
    std::cout << "[TEST] crc corruption\n";

    Packet pkt{};
    pkt.magic_number = MAGIC_NUMBER;
    pkt.version = VERSION;

    uint8_t buffer[64];
    size_t len = serialize_with_crc(pkt, buffer);

    std::cout << "Original buffer:\n";
    dump_hex(buffer, len);

    buffer[10] ^= 0x01;

    std::cout << "Corrupted buffer:\n";
    dump_hex(buffer, len);

    Packet out{};
    bool ok = deserialize_packet(out, buffer, len);
    assert(!ok);

    std::cout << "CRC correctly rejected corrupted packet\n";
}


void test_truncation() {
    std::cout << "[TEST] truncation\n";

    Packet pkt{};
    pkt.magic_number = MAGIC_NUMBER;
    pkt.version = VERSION;

    uint8_t buffer[64];
    size_t len = serialize_with_crc(pkt, buffer);

    Packet out{};
    bool ok = deserialize_packet(out, buffer, len - 1);
    assert(!ok);
}

void test_endianness_values() {
    std::cout << "[TEST] endianness\n";

    Packet pkt{};
    pkt.magic_number = 0x01020304;
    pkt.version = 0xAABBCCDD;
    pkt.timestamp = 0x1122334455667788ULL;
    pkt.temperature = -273.15f;
    pkt.voltage = 12345.678f;
    pkt.current = 0.0001f;

    uint8_t buffer[64];
    size_t len = serialize_with_crc(pkt, buffer);

    Packet out{};
    bool ok = deserialize_packet(out, buffer, len);
    assert(ok);

    assert(out.magic_number == pkt.magic_number);
    assert(out.version == pkt.version);
    assert(out.timestamp == pkt.timestamp);
    assert(out.temperature == pkt.temperature);
    assert(out.voltage == pkt.voltage);
    assert(out.current == pkt.current);
}

void test_determinism() {
    std::cout << "[TEST] determinism\n";

    Packet pkt{};
    pkt.magic_number = MAGIC_NUMBER;
    pkt.version = VERSION;
    pkt.timestamp = 42;
    pkt.temperature = 1.0f;
    pkt.voltage = 2.0f;
    pkt.current = 3.0f;

    uint8_t b1[64], b2[64];
    size_t l1 = serialize_with_crc(pkt, b1);
    size_t l2 = serialize_with_crc(pkt, b2);

    assert(l1 == l2);
    assert(memcmp(b1, b2, l1) == 0);
}

// ---- main ---------------------------------------------------------------

int main() {
    test_roundtrip();
    test_crc_corruption();
    test_truncation();
    test_endianness_values();
    test_determinism();

    std::cout << "\nALL TESTS PASSED ✅\n";
    return 0;
}
