#include "udp_socket.h"
#include <unistd.h>
#include <cstring>
#include <stdexcept>

UdpSocket::UdpSocket(uint16_t listen_port)
    : has_peer_(false)
{
    sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_ < 0)
        throw std::runtime_error("socket failed");

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(listen_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd_, (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("bind failed");
}

UdpSocket::UdpSocket(const char* ip, uint16_t port)
    : has_peer_(true)
{
    sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_ < 0)
        throw std::runtime_error("socket failed");

    memset(&peer_addr_, 0, sizeof(peer_addr_));
    peer_addr_.sin_family = AF_INET;
    peer_addr_.sin_port = htons(port);
    inet_pton(AF_INET, ip, &peer_addr_.sin_addr);
}

UdpSocket::~UdpSocket() {
    close(sockfd_);
}

ssize_t UdpSocket::send(const uint8_t* data, size_t size) {
    if (!has_peer_) return -1;
    return sendto(sockfd_, data, size, 0,
                  (sockaddr*)&peer_addr_, sizeof(peer_addr_));
}

ssize_t UdpSocket::receive(uint8_t* buffer, size_t max_size) {
    return recvfrom(sockfd_, buffer, max_size, 0, nullptr, nullptr);
}
