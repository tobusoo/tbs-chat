#include <ui/TextArea.hpp>

namespace ftxui {

TextArea::TextArea()
    : user_message(std::string()),
      text_area(Input(&user_message, "Type message for your friends...", InputOption::Default()))
{
}

}; // namespace ftxui