#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <thread>

bool server_is_running = true;

void server_listen(int sock)
{
    char buf[1024] = {0};
    int bytes_read;
    printf("Starting listening\n");

    while (server_is_running) {
        bytes_read = recv(sock, buf, sizeof(buf), 0);
        if (bytes_read == 0) {
            server_is_running = false;
        }
        printf("%.*s\n", bytes_read, buf);
    }

    printf("Server is closed\n");
}

int main()
{
    int sock;
    struct sockaddr_in addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(6969);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(sock, (struct sockaddr*)&(addr), sizeof(addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    std::thread listener(server_listen, sock);

    std::string s;
    while (s != "q" && server_is_running) {
        std::cin >> s;
        send(sock, s.c_str(), s.size(), 0);
    }

    listener.join();
    close(sock);

    return 0;
}