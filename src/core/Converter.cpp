#include <netinet/in.h>

#include <Core.hpp>

void Converter::setValue(const std::string& str, size_t i) noexcept
{
    byte[0] = str[i];
    byte[1] = str[i + 1];
    byte[2] = str[i + 2];
    byte[3] = str[i + 3];
    value = ntohl(value);
}

void Converter::setValue(const char* str, size_t i) noexcept
{
    byte[0] = str[i];
    byte[1] = str[i + 1];
    byte[2] = str[i + 2];
    byte[3] = str[i + 3];
    value = ntohl(value);
}

void strAppendUint(std::string& str, uint32_t value) noexcept
{
    Converter converter = {.value = value};
    std::string buf(4, '\0');
    converter.value = htonl(converter.value);
    buf[0] = converter.byte[0];
    buf[1] = converter.byte[1];
    buf[2] = converter.byte[2];
    buf[3] = converter.byte[3];
    str += buf;
}
