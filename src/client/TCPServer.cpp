#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <TCPClient.hpp>

TCPClient::TCPClient(uint32_t host, uint16_t port) noexcept
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(host);
    printf("Trying to connect\n");
    if (connect(sock, (struct sockaddr*)&(addr), sizeof(addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    printf("Connected\n");
}

bool TCPClient::ServerIsRunning() const noexcept
{
    return is_server_running;
}

void TCPClient::SendMessage(const std::string& str) const noexcept
{
    send(sock, str.c_str(), str.size(), 0);
}

int TCPClient::ReadMessage(std::string& dst) noexcept
{
    bytes_read = recv(sock, buf, sizeof(buf), MSG_DONTWAIT);
    if (bytes_read == 0) {
        is_server_running = false;
        return -1;
    }
    if (errno == EAGAIN) {
        errno = 0;
        return 1;
    }

    dst.clear();
    dst.append(buf, bytes_read);
    return 0;
}

TCPClient::~TCPClient()
{
    close(sock);
}
