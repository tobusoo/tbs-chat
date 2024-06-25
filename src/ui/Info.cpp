#include <ui/Info.hpp>
#include <ui/MessageComponent.hpp>

namespace ftxui {

Info::Info()
    : renderer(Renderer([&] {
          return vbox({
                         paragraphAlignLeft("Welcome to TOBUSO-CHAT") | center,
                         hbox({paragraphAlignLeft(
                                 "You can use the mouse and keyboard to navigate through the interface.")}),
                         hbox({paragraphAlignLeft(
                                 "To view messages that no longer fit on the screen, press F10. So "
                                 "the chat window will not focus on the last message and you will "
                                 "be able to scroll through this window.")}),
                         text(""),
                         text(""),
                         text("Keys:"),
                         hbox({paragraphAlignLeft("F10 - focus/unfocus on the last message.")
                               | color(Color::Aquamarine1)}),
                         hbox({paragraphAlignLeft("ESC - exit.") | color(Color::Aquamarine1)}),
                         text(""),
                         text(""),
                         text(""),

                         hbox({paragraphAlignLeft("Made by tobuso.")}),
                         hbox({paragraphAlignLeft("To contact me: chppppr@yandex.ru")}),
                 })
                  | border;
      }))
{
}

}; // namespace ftxui