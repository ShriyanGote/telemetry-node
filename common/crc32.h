#pragma once

#include <cstdint>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include "packet.h"

using namespace std;

constexpr uint32_t crc32_polynomial = 0xEDB88320;
uint32_t crc32(const uint8_t *data, size_t size);