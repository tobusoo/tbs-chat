#pragma once

enum MessageType {
    NEW_CONNECT,
    REQUEST,
    FORBIDDEN,
    MESSAGE,
    USERS,
    USER_DISCONNECT,
    UNKNOWN,
    MessageTypeN = UNKNOWN,
};

#include "Converter.hpp"
#include "Deserializer.hpp"
#include "Serializer.hpp"
