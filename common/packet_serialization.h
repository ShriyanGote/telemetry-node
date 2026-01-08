#pragma once

#include "packet.h"
#include "crc32.h"
#include "endian.h"

using namespace std;

size_t serialize_packet(const Packet &packet, uint8_t *data);
bool deserialize_packet(Packet &packet, const uint8_t *data, size_t size);

