#pragma once
#include <ftxui/component/component.hpp>

#include <ui/Messages.hpp>
#include <ui/TextArea.hpp>

namespace ftxui {

class Chat {
public:
    bool is_need_focused = true;
    size_t prev_msg_size = 0;
    Box container_box;
    ftxui::Component container;
    ftxui::Component renderer;

    Chat(Messages& msgs, TextArea& text_area);
};

}; // namespace ftxui