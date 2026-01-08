#pragma once

#include <cstdint>
#include <cstddef>
#include <arpa/inet.h>

class UdpSocket {
public:
    // receive
    explicit UdpSocket(uint16_t listen_port);

    // send
    UdpSocket(const char* ip, uint16_t port);
    ~UdpSocket();
    ssize_t send(const uint8_t* data, size_t size);
    ssize_t receive(uint8_t* buffer, size_t max_size);

private:
    int sockfd_;
    sockaddr_in peer_addr_;
    bool has_peer_;
};
