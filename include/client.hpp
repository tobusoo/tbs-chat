#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <sys/socket.h>

#include <string>

class TCPClient {
private:
    int sock;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;
    bool is_server_running = true;

public:
    TCPClient(uint32_t host, uint16_t port)
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, "192.0.2.33", &(addr.sin_addr));
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        if (connect(sock, (struct sockaddr*)&(addr), sizeof(addr)) < 0) {
            perror("connect");
            exit(EXIT_FAILURE);
        }
    }

    bool ServerIsRunning() const noexcept
    {
        return is_server_running;
    }

    void SendMessage(const std::string& str) const noexcept
    {
        send(sock, str.c_str(), str.size(), 0);
    }

    void ReadMessage(std::string& dst) noexcept
    {
        bytes_read = recv(sock, buf, sizeof(buf), 0);
        if (bytes_read == 0) {
            is_server_running = false;
            return;
        }

        dst.clear();
        dst.append(buf, bytes_read);
    }

    ~TCPClient()
    {
        close(sock);
    }
};