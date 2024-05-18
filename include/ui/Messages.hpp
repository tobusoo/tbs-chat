#pragma once
#include <ftxui/component/component.hpp>

#include <ui/MessageComponent.hpp>

namespace ftxui {

class Messages {
public:
    ftxui::Component container;
    ftxui::Component renderer;

    Messages();
    void Add(const std::string& sender, const std::string& content) noexcept;
    void Add(const std::string& content, Color color = Color::Aquamarine3) noexcept;
};

}; // namespace ftxui