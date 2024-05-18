#include <ui/Messages.hpp>

namespace ftxui {

Messages::Messages()
    : container(Container::Vertical({})), renderer(Renderer(container, [&] { return container->Render(); }))
{
}

void Messages::Add(const std::string& sender, const std::string& content) noexcept
{
    container->Add(SendMessage::toComponent(sender, content));
}

void Messages::Add(const std::string& content, Color color) noexcept
{
    container->Add(Message::toComponent(content, color));
}

}; // namespace ftxui