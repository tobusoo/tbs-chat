#include <iostream>

#include <AsioTCPServer.hpp>
#include <cxxopts.hpp>

int main(int argc, char* argv[])
{
    int port;
    int q_len = 50;
    int password = 0;

    cxxopts::Options options(argv[0]);
    options.add_options()("p, port", "Port", cxxopts::value<int>())(
            "l,q_len", "Max queue len", cxxopts::value<int>())(
            "P, password", "Password", cxxopts::value<int>())("h,help", "Print usage");
    cxxopts::ParseResult result;
    try {
        result = options.parse(argc, argv);
    } catch (cxxopts::exceptions::exception& e) {
        std::cout << e.what() << '\n';
        std::cout << options.help() << '\n';
        return 0;
    }

    if (result.count("help")) {
        std::cout << options.help() << '\n';
        return 0;
    }

    if (result.count("port") == 0) {
        std::cout << options.help() << '\n';
        return 0;
    } else {
        port = result["port"].as<int>();
    }

    if (result.count("password") == 1) {
        password = result["password"].as<int>();
    }

    if (result.count("q_len")) {
        q_len = result["q_len"].as<int>();
    }

    AsioTCPServer server(port);
    server.setPassword(password);
    server.StartListen(q_len);

    return 0;
}