#include <iostream>
#include <cstdint>
#include <cstring>
#include <arpa/inet.h>

using namespace std;


class UdpSocket {
    public:
    UdpSocket(const string& address, const uint16_t port);
    ~UdpSocket();
    void send(const uint8_t *data, size_t size);
    void receive(uint8_t *data, size_t size);
};