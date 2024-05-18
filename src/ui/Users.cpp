#include <ui/Users.hpp>

namespace ftxui {

Users::Users()
    : container(Container::Vertical({})), renderer(Renderer(container, [&] {
          size_t elem_cnt = container->ChildCount();
          size_t max_len = Terminal::Size().dimy - 10;
          size_t len = max_len > elem_cnt ? elem_cnt : max_len;

          return container->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, max_len);
      }))
{
}

void Users::Add(const std::string& user)
{
    online.insert(user);
    container->Add(Message::toComponent(user, Color::RoyalBlue1));
}

void Users::Remove(const std::string& user)
{
    online.erase(user);
    container->DetachAllChildren();
    for (auto&& i : online) {
        container->Add(Message::toComponent(i, Color::RoyalBlue1));
    }
}

}; // namespace ftxui