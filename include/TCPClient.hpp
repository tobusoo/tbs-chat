#pragma once
#include <arpa/inet.h>
#include <netinet/ip.h>

#include <config.h>
#include <string>

class TCPClient {
private:
    int sock;
    struct sockaddr_in addr;
    char buf[BUFFER_SIZE];
    size_t bytes_read;
    bool is_server_running = true;

public:
    TCPClient(uint32_t host, uint16_t port) noexcept;
    bool ServerIsRunning() const noexcept;
    void SendMessage(const std::string& str) const noexcept;
    int ReadMessage(std::string& dst) noexcept;
    ~TCPClient();
};