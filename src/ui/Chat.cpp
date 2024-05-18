#include <ui/Chat.hpp>

namespace ftxui {

Chat::Chat(Messages& msgs, TextArea& text_area)
    : container(Container::Vertical({msgs.container, text_area.text_area})),
      renderer(Renderer(container, [&] {
          size_t elem_cnt = msgs.container->ChildCount();
          size_t max_len = Terminal::Size().dimy - 10;

          auto msg_container = msgs.container->Render() | reflect(container_box) | vscroll_indicator;
          if (is_need_focused)
              msg_container |= focusPositionRelative(0, 1.0f);
          prev_msg_size = container_box.y_max;
          size_t len = std::min(max_len, prev_msg_size);

          return vbox({
                  window(text("Messages") | bold | align_right,
                         msg_container | yframe | size(HEIGHT, LESS_THAN, len)),
                  hbox({text_area.text_area->Render(),
                        text(" " + std::to_string(text_area.user_message.size()) + "/500")})
                          | flex | border,
          });
      }))
{
}

}; // namespace ftxui