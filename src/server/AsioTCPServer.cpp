#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>

#include <AsioTCPServer.hpp>

void print_addr(int sock)
{
    struct sockaddr_in peeraddr;
    socklen_t peeraddrlen;
    getpeername(sock, (struct sockaddr*)&peeraddr, &peeraddrlen);
    char* ip = inet_ntoa(peeraddr.sin_addr);
    std::cout << '[' << ip << ':' << ntohs(peeraddr.sin_port) << "] ";
}

AsioTCPServer::AsioTCPServer(int port) noexcept : port(port)
{
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    fcntl(listener, F_SETFL, O_NONBLOCK);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

void AsioTCPServer::setPassword(int password) noexcept
{
    pw = password;
}

int AsioTCPServer::getPassowrd() const noexcept
{
    return pw;
}

void AsioTCPServer::SendToAll(const char* buf, size_t len, int from_fd) noexcept
{
    for (auto&& [user_fd, username] : users) {
        if (user_fd != from_fd)
            send(user_fd, buf, len, 0);
    }
}

int AsioTCPServer::ClientMessageHandler(int client_fd) noexcept
{
    std::pair<std::string, std::string> msg;
    std::pair<std::string, int> request;
    bytes_read = recv(client_fd, buf, sizeof(buf), 0);

    if (bytes_read == 0) {
        std::cout << users[client_fd] << " DISCONNECTED" << '\n';
        return -1;
    }

    MessageType type = deserializer.getType(buf, bytes_read);
    switch (type) {
    case MESSAGE:
        std::cout << "Message from " << users[client_fd] << '\n';
        SendToAll(buf, bytes_read, client_fd);
        break;
    case REQUEST:
        request = deserializer.getRequest(buf, bytes_read);
        std::cout << "REQUEST: " << request.first << " pass: " << request.second << '\n';
        if (pw == 0 || request.second == pw) {
            if (users.size() > 1) {
                auto response = serializer.users(users, client_fd);
                send(client_fd, response.c_str(), response.length(), 0);
            }
            users[client_fd] = request.first;

            auto new_connect = serializer.newConnect(request.first);
            SendToAll(new_connect.c_str(), new_connect.length(), client_fd);
            std::cout << "Correct password\n";
        } else {
            auto forbidden = serializer.forbidden();
            send(client_fd, forbidden.c_str(), forbidden.length(), 0);
            std::cout << "Wrong password. Send \"forbidden\" message\n";
            return -1;
        }
        break;
    default:
        std::cout << "[WARNING] the server does not process this type of message: ";
        std::cout << type << '\n';
        break;
    }

    print_addr(client_fd);
    return 0;
}

void AsioTCPServer::StartListen(int max_queue_len) noexcept
{
    if (listen(listener, max_queue_len) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Starting listening [port:%d, q_len:%d, password:%d] \n", port, max_queue_len, pw);
    while (true) {
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(listener, &readset);

        for (auto&& [user_fd, usename] : users) {
            FD_SET(user_fd, &readset);
        }

        int mx = std::max(listener, (*std::max_element(users.begin(), users.end())).first);
        if (select(mx + 1, &readset, NULL, NULL, 0) < 0) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(listener, &readset)) {
            int sock = accept(listener, 0, 0);
            if (sock < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            print_addr(sock);
            std::cout << "CONNECTED\n";

            fcntl(sock, F_SETFL, O_NONBLOCK);
            users[sock] = "";
        }

        for (auto user = users.begin(); user != users.end();) {
            if (FD_ISSET((*user).first, &readset) && ClientMessageHandler((*user).first) == -1) {
                if ((*user).second != "") {
                    auto response = serializer.userDisconnect((*user).second);
                    SendToAll(response.c_str(), response.length(), (*user).first);
                }
                user = users.erase(user);
                continue;
            }
            user++;
        }
    }
}

AsioTCPServer::~AsioTCPServer()
{
    for (auto&& [user_fd, username] : users) {
        close(user_fd);
    }
    close(listener);
}
