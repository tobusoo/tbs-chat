#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <sys/socket.h>

#include <algorithm>
#include <functional>
#include <set>

void print_in(int sock)
{
    struct sockaddr_in peeraddr;
    socklen_t peeraddrlen;
    getpeername(sock, (struct sockaddr*)&peeraddr, &peeraddrlen);
    char* peeraddrpresn = inet_ntoa(peeraddr.sin_addr);
    printf("[CONNECTED] %s:%d\n", peeraddrpresn, ntohs(peeraddr.sin_port));
}

void print_out(int sock)
{
    struct sockaddr_in peeraddr;
    socklen_t peeraddrlen;
    getpeername(sock, (struct sockaddr*)&peeraddr, &peeraddrlen);
    char* peeraddrpresn = inet_ntoa(peeraddr.sin_addr);
    printf("[DISCONNECTED] %s:%d\n", peeraddrpresn, ntohs(peeraddr.sin_port));
}

void print_addr(int sock)
{
    struct sockaddr_in peeraddr;
    socklen_t peeraddrlen;
    getpeername(sock, (struct sockaddr*)&peeraddr, &peeraddrlen);
    char* peeraddrpresn = inet_ntoa(peeraddr.sin_addr);
    printf("[%s:%d] ", peeraddrpresn, ntohs(peeraddr.sin_port));
}

class AsioTCPServer {
private:
    int listener; // socket descriptor for listen
    struct sockaddr_in addr;
    std::set<int> clients;
    std::function<void(int)> connect_handler = print_in;
    std::function<void(int)> disconnect_handler = print_out;

    char buf[1024];
    int bytes_read;

public:
    AsioTCPServer(int port)
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

    void SetConnecHandler(const std::function<void(int)>& f)
    {
        connect_handler = f;
    }

    void SetDisconnecHandler(const std::function<void(int)>& f)
    {
        disconnect_handler = f;
    }

    void SendToAll(const char* buf, size_t len, int from_fd)
    {
        for (auto client = clients.begin(); client != clients.end();) {
            if (from_fd != *client) {
                printf("\"send\" from %d to %d\n", from_fd, *client);
                send(*client, buf, len, 0);
            }
            client++;
        }
    }

    int HandlerClientMessage(int client)
    {
        bytes_read = recv(client, buf, 1024, 0);

        if (bytes_read == 0) {
            disconnect_handler(client);
            close(client);
            return -1;
        }

        print_addr(client);
        printf("%.*s\n", bytes_read, buf);
        SendToAll(buf, bytes_read, client);

        return 0;
    }

    void StartListen(int max_queue_len)
    {
        if (listen(listener, max_queue_len) == -1) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        while (true) {
            fd_set readset;
            FD_ZERO(&readset);
            FD_SET(listener, &readset);

            for (auto&& client : clients) {
                FD_SET(client, &readset);
            }

            int mx = std::max(listener, *std::max_element(clients.begin(), clients.end()));
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

                connect_handler(sock);

                fcntl(sock, F_SETFL, O_NONBLOCK);
                clients.insert(sock);
            }

            for (auto client = clients.begin(); client != clients.end();) {
                if (FD_ISSET(*client, &readset) && HandlerClientMessage(*client) == -1) {
                    client = clients.erase(client);
                    continue;
                }
                client++;
            }
        }
    }
};
