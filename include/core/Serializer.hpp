#pragma once
#include <map>
#include <set>

#include <Core.hpp>

class Serializer {
    Converter type;
    std::string res;

public:
    std::string request(const std::string& name, int password) noexcept;
    std::string newConnect(const std::string& name) noexcept;
    std::string forbidden() noexcept;
    std::string message(const std::string& name, const std::string& data) noexcept;
    std::string users(const std::set<std::string> users) noexcept;
    std::string users(const std::map<int, std::string> users) noexcept;
    std::string users(const std::map<int, std::string> users, int fd_user) noexcept;
    std::string userDisconnect(const std::string& user) noexcept;
};