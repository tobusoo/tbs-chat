#include "server.hpp"

int main()
{
    AsioTCPServer server(6969);
    server.StartListen(69);

    return 0;
}