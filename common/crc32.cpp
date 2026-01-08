#include "crc32.h"
#include <iostream>

using namespace std;

uint32_t crc32(const uint8_t *data, size_t size) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < size; i++){
        crc ^= data[i];
        for (size_t j = 0; j < 8; j++){
            if (crc & 0x00000001){
                crc = (crc >> 1) ^ crc32_polynomial;
            } else {
                crc >>= 1;
            }
        }   
    }
    return crc ^ 0xFFFFFFFF;
}