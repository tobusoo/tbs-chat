#include <Serializer.hpp>

std::string Serializer::request(const std::string& name, int password) noexcept
{
    res.clear();
    type.value = REQUEST;
    strAppendUint(res, type.value);
    strAppendUint(res, name.length());
    res += name;
    strAppendUint(res, password);

    return res;
}

std::string Serializer::newConnect(const std::string& name) noexcept
{
    res.clear();
    type.value = NEW_CONNECT;
    strAppendUint(res, type.value);
    strAppendUint(res, name.length());

    return res + name;
}

std::string Serializer::forbidden() noexcept
{
    res.clear();
    type.value = FORBIDDEN;
    strAppendUint(res, type.value);

    return res;
}

std::string Serializer::message(const std::string& name, const std::string& data) noexcept
{
    res.clear();
    type.value = MESSAGE;
    strAppendUint(res, type.value);
    strAppendUint(res, name.length());

    return res + name + data;
}

std::string Serializer::users(const std::set<std::string> users) noexcept
{
    res.clear();
    type.value = USERS;
    strAppendUint(res, type.value);
    strAppendUint(res, users.size());
    for (auto&& user : users) {
        strAppendUint(res, user.length());
        res += user;
    }

    return res;
}

std::string Serializer::users(const std::map<int, std::string> users) noexcept
{
    res.clear();
    type.value = USERS;
    strAppendUint(res, type.value);
    strAppendUint(res, users.size());
    for (auto&& [user_fd, username] : users) {
        strAppendUint(res, username.length());
        res += username;
    }

    return res;
}

std::string Serializer::users(const std::map<int, std::string> users, int fd_user) noexcept
{
    res.clear();
    type.value = USERS;
    strAppendUint(res, type.value);
    strAppendUint(res, users.size());
    for (auto&& [user_fd, username] : users) {
        if (user_fd == fd_user)
            continue;
        strAppendUint(res, username.length());
        res += username;
    }

    return res;
}

std::string Serializer::userDisconnect(const std::string& user) noexcept
{
    res.clear();
    type.value = USER_DISCONNECT;
    strAppendUint(res, type.value);
    strAppendUint(res, user.length());

    return res + user;
}
