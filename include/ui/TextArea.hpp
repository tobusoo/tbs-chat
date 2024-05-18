#pragma once
#include <ftxui/component/component.hpp>

namespace ftxui {

class TextArea {
public:
    std::string user_message;
    ftxui::Component text_area = Input(&user_message, "Type message for your friends...");

    TextArea();
};

}; // namespace ftxui