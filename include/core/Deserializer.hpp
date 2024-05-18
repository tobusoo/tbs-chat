#pragma once
#include <set>

#include <Core.hpp>

class Deserializer {
public:
    MessageType getType(const std::string& message) noexcept;
    MessageType getType(const char* message, size_t len) noexcept;

    std::string getName(const std::string& message) noexcept;
    std::string getName(const char* message, size_t len) noexcept;

    std::pair<std::string, int> getRequest(const std::string& message) noexcept;
    std::pair<std::string, int> getRequest(const char* message, size_t len) noexcept;

    std::pair<std::string, std::string> getMessage(const std::string& message) noexcept;
    std::pair<std::string, std::string> getMessage(const char* message, size_t len) noexcept;

    std::set<std::string> getUsers(const std::string& message) noexcept;
    std::set<std::string> getUsers(const char* message, size_t len) noexcept;
};