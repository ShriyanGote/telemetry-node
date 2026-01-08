#include "packet_serialization.h"
#include <cstddef>
#include <cstring>


#include "crc32.h"
#include "endian.h"


uint32_t read_uint32_t(const uint8_t *buffer, size_t *offset){
    uint32_t temp;
    memcpy(&temp, buffer + *offset, sizeof(uint32_t));
    *offset += sizeof(uint32_t);
    return ntohl(temp);
}
uint64_t read_uint64_t(const uint8_t *buffer, size_t *offset){
    uint64_t temp;
    memcpy(&temp, buffer + *offset, sizeof(uint64_t));
    *offset += sizeof(uint64_t);
    return network_to_host_long_long(temp);
}
float read_float(const uint8_t *buffer, size_t *offset){
    uint32_t temp;
    memcpy(&temp, buffer + *offset, sizeof(uint32_t));
    temp = ntohl(temp);
    *offset += sizeof(uint32_t);
    float value;
    memcpy(&value, &temp, sizeof(uint32_t));
    return value;
}

void write_uint32_t(uint8_t *buffer, size_t *offset, const uint32_t value)
{
    uint32_t temp = htonl(value);
    memcpy(buffer + *offset, &temp, sizeof(uint32_t));
    *offset += sizeof(uint32_t);
}
void write_uint64_t(uint8_t *buffer, size_t *offset, const uint64_t value)
{
    uint64_t temp = host_to_network_long_long(value);
    memcpy(buffer + *offset, &temp, sizeof(uint64_t));
    *offset += sizeof(uint64_t);
}

void write_float(uint8_t *buffer, size_t *offset, const float value)
{
    uint32_t temp;
    memcpy(&temp, &value, sizeof(float));
    temp = htonl(temp);
    memcpy(buffer + *offset, &temp, sizeof(uint32_t));
    *offset += sizeof(uint32_t);
}


size_t serialize_packet(const Packet &packet, uint8_t *buffer) {
    size_t offset = 0;
    write_uint32_t(buffer, &offset, packet.magic_number);
    write_uint32_t(buffer, &offset, packet.version);
    write_uint32_t(buffer, &offset, packet.payload_len);
    write_uint64_t(buffer, &offset, packet.timestamp_sent);
    write_uint32_t(buffer, &offset, packet.sequence_number);
    write_float(buffer, &offset, packet.temperature);
    write_float(buffer, &offset, packet.voltage);
    write_float(buffer, &offset, packet.current);

    return offset;
}

bool deserialize_packet(Packet &packet, const uint8_t *buffer, size_t size) {
    if (size < 36) {
        return false;
    }
    size_t offset = 0;
    packet.magic_number = read_uint32_t(buffer, &offset);
    packet.version = read_uint32_t(buffer, &offset);
    packet.payload_len = read_uint32_t(buffer, &offset);
    packet.timestamp_sent = read_uint64_t(buffer, &offset);
    packet.sequence_number = read_uint32_t(buffer, &offset);
    packet.temperature = read_float(buffer, &offset);
    packet.voltage = read_float(buffer, &offset);
    packet.current = read_float(buffer, &offset);

    uint32_t checksum = read_uint32_t(buffer, &offset);
    if (checksum != crc32(buffer, offset-sizeof(uint32_t))) {
        return false;
    }
    packet.checksum = checksum;
    
    return true;
}