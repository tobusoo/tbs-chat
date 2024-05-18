#include <Deserializer.hpp>
#include <iostream>

MessageType Deserializer::getType(const std::string& message) noexcept
{
    Converter type;
    size_t len = message.length();
    if (len < 4)
        return UNKNOWN;

    type.setValue(message, 0);
    if (type.value >= NEW_CONNECT && type.value <= MessageTypeN)
        return static_cast<MessageType>(type.value);
    return UNKNOWN;
}

MessageType Deserializer::getType(const char* message, size_t len) noexcept
{
    Converter type;
    if (len < 4)
        return UNKNOWN;
    type.setValue(message, 0);

    if (type.value >= NEW_CONNECT && type.value <= MessageTypeN)
        return static_cast<MessageType>(type.value);
    return UNKNOWN;
}

std::string Deserializer::getName(const std::string& message) noexcept
{
    Converter sender_len;
    size_t len = message.length();
    if (len < 8)
        return "";

    sender_len.setValue(message, 4);
    if (len - 8 < sender_len.value)
        return "";
    return message.substr(8, sender_len.value);
}

std::string Deserializer::getName(const char* message, size_t len) noexcept
{
    Converter sender_len;
    if (len < 8)
        return "";

    sender_len.setValue(message, 4);
    if (len - 8 < sender_len.value)
        return "";
    return std::string(message + 8, message + 8 + sender_len.value);
}

std::pair<std::string, int> Deserializer::getRequest(const std::string& message) noexcept
{
    Converter password;
    size_t len = message.length();

    auto name = getName(message);
    if (name == "" || len - 8 - name.length() < 4)
        return {name, -1};

    password.setValue(message, 8 + name.length());
    return {name, password.value};
}

std::pair<std::string, int> Deserializer::getRequest(const char* message, size_t len) noexcept
{
    Converter password;

    auto name = getName(message, len);
    if (name == "" || len - 8 - name.length() < 4)
        return {name, -1};

    password.setValue(message, 8 + name.length());
    return {name, password.value};
}

std::pair<std::string, std::string> Deserializer::getMessage(const std::string& message) noexcept
{
    auto name = getName(message);
    if (name == "")
        return {name, ""};

    return {name, message.substr(8 + name.length())};
}

std::pair<std::string, std::string> Deserializer::getMessage(const char* message, size_t len) noexcept
{
    auto name = getName(message, len);
    if (name == "")
        return {name, ""};

    return {name, std::string(message + 8 + name.length(), message + len)};
}

std::set<std::string> Deserializer::getUsers(const std::string& message) noexcept
{
    std::set<std::string> users;
    Converter users_cnt, user_name_len;
    size_t len = message.length(), user_pos;
    if (len < 8)
        return std::set<std::string>();
    users_cnt.setValue(message, 4);

    user_pos = 8;
    for (size_t i = 0; i < users_cnt.value; i++) {
        if (len < user_pos + 4)
            return users;

        user_name_len.setValue(message, user_pos);
        if (len - user_pos < user_name_len.value)
            return users;

        user_pos += 4;
        users.insert(message.substr(user_pos, user_name_len.value));
        user_pos += user_name_len.value;
    }

    return users;
}

std::set<std::string> Deserializer::getUsers(const char* message, size_t len) noexcept
{
    std::set<std::string> users;
    Converter users_cnt, user_name_len;
    size_t user_pos;
    if (len < 8)
        return std::set<std::string>();
    users_cnt.setValue(message, 4);

    user_pos = 8;
    for (size_t i = 0; i < users_cnt.value; i++) {
        if (len < user_pos + 4)
            return users;

        user_name_len.setValue(message, user_pos);
        if (len - user_pos < user_name_len.value)
            return users;

        user_pos += 4;
        users.insert(std::string(message + user_pos, message + user_pos + user_name_len.value));
        user_pos += user_name_len.value;
    }

    return users;
}
