#pragma once
#include <map>
#include <netinet/ip.h>

#include <Core.hpp>
#include <config.h>

class AsioTCPServer {
private:
    int listener; // socket descriptor for listen
    struct sockaddr_in addr;
    int port;

    std::map<int, std::string> users;

    Deserializer deserializer;
    Serializer serializer;

    char buf[BUFFER_SIZE];
    size_t bytes_read;
    int pw = 0; // If the password is 0, then the server accepts all connections

public:
    AsioTCPServer(int port) noexcept;

    void setPassword(int password) noexcept;
    int getPassowrd() const noexcept;
    void SendToAll(const char* buf, size_t len, int from_fd) noexcept;
    int ClientMessageHandler(int client) noexcept;
    void StartListen(int max_queue_len) noexcept;

    ~AsioTCPServer();
};
