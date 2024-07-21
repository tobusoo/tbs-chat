#pragma once
#include <string>
#include <cinttypes>

#include <Core.hpp>

union Converter {
    char byte[4];
    uint32_t value;
    void setValue(const std::string& str, size_t i) noexcept;
    void setValue(const char* str, size_t i) noexcept;
};

void strAppendUint(std::string& str, uint32_t value) noexcept;
