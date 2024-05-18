#pragma once
#include <string>

struct Settings {
    std::string username;
    std::string host;
    int port;
    int password;
};

int parseCLI(int argc, char* argv[], Settings& settings);
