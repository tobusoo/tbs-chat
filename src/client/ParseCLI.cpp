#include <iostream>

#include <client/ParseCLI.hpp>
#include <cxxopts.hpp>

int parseCLI(int argc, char* argv[], Settings& settings)
{
    cxxopts::Options options(argv[0]);
    options.add_options()("u, username", "Your username", cxxopts::value<std::string>())(
            "H, host", "Host IPv4", cxxopts::value<std::string>())("p, port", "Port", cxxopts::value<int>())(
            "P, password", "Password", cxxopts::value<int>())("h,help", "Print usage");
    cxxopts::ParseResult parse_result;
    try {
        parse_result = options.parse(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        std::cout << options.help() << '\n';
        return 0;
    }
    if (parse_result.count("help")) {
        std::cout << options.help() << '\n';
        return 0;
    }

    if (parse_result.count("username") == 0) {
        std::cout << options.help() << '\n';
        return -1;
    } else {
        settings.username = parse_result["username"].as<std::string>();
    }
    if (parse_result.count("host") == 0) {
        std::cout << options.help() << '\n';
        return -1;
    } else {
        settings.host = parse_result["host"].as<std::string>();
    }
    if (parse_result.count("port") == 0) {
        std::cout << options.help() << '\n';
        return -1;
    } else {
        settings.port = parse_result["port"].as<int>();
    }
    if (parse_result.count("password") == 0) {
        std::cout << options.help() << '\n';
        return -1;
    } else {
        settings.password = parse_result["password"].as<int>();
    }

    return 0;
}