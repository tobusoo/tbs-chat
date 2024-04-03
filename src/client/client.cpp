#include <iostream>
#include <thread>

#include <Package.pb.h>

#include <client.hpp>

void server_listen(TCPClient& client)
{
    printf("Starting listening\n");
    core::Package msg;
    std::string buf;

    while (client.ServerIsRunning()) {
        client.ReadMessage(buf);
        msg.ParseFromString(buf);
        std::cout << msg.sender() << ' ' << msg.content() << '\n';
    }

    printf("Server is closed\n");
}

int main(int argc, char* argv[])
{
    TCPClient client(0, 6969);
    std::thread listener(server_listen, std::ref(client));

    core::Package msg;
    msg.set_sender(argc > 1 ? argv[1] : "default");
    std::string buf;

    while (buf != "q" && client.ServerIsRunning()) {
        std::cin >> buf;
        msg.set_content(buf.c_str());
        msg.SerializeToString(&buf);
        client.SendMessage(buf);
    }

    listener.join();
    return 0;
}