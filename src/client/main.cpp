#include <iostream>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include <ui/Chat.hpp>
#include <ui/Info.hpp>
#include <ui/MessageComponent.hpp>
#include <ui/Messages.hpp>
#include <ui/TabMenu.hpp>
#include <ui/TextArea.hpp>
#include <ui/Users.hpp>

#include <Core.hpp>
#include <TCPClient.hpp>

#include <client/ParseCLI.hpp>

using namespace ftxui;
using namespace std::chrono_literals;

bool is_forbidden = false;

void listener(
        std::stop_token stoken,
        TCPClient& client,
        Messages& messages,
        Users& users,
        ScreenInteractive* screen)
{
    std::pair<std::string, std::string> message;
    std::set<std::string> new_users;
    Deserializer deserializer;
    MessageType type;
    std::string buf, name;

    while (client.ServerIsRunning()) {
        if (stoken.stop_requested() || is_forbidden)
            break;

        auto status = client.ReadMessage(buf);
        if (status == 1) { // Server doesn't send a message
            std::this_thread::sleep_for(200ms);
            continue;
        }

        type = deserializer.getType(buf);
        switch (type) {
        case NEW_CONNECT:
            name = deserializer.getName(buf);
            users.Add(name);
            messages.Add(name + " connected");
            break;
        case FORBIDDEN:
            is_forbidden = true;
            return;
        case MESSAGE:
            message = deserializer.getMessage(buf);
            messages.Add(message.first, message.second);
            break;
        case USERS:
            new_users = deserializer.getUsers(buf);
            for (auto&& user : new_users) {
                users.Add(user);
            }
            break;
        case USER_DISCONNECT:
            name = deserializer.getName(buf);
            messages.Add(name + " disconnected", Color::RedLight);
            users.Remove(name);
            break;
        default:
            break;
        }
        screen->PostEvent(Event::Special("New event"));
    }
}

int main(int argc, char* argv[])
{
    Settings settings;
    auto ret_value = parseCLI(argc, argv, settings);
    if (ret_value == -1)
        return 0;
    auto screen = ScreenInteractive::Fullscreen();
    screen.Clear();

    struct sockaddr_in sa;
    inet_pton(AF_INET, settings.host.c_str(), &(sa.sin_addr));

    TCPClient client(sa.sin_addr.s_addr, settings.port);
    Serializer serializer;
    std::string buf = serializer.request(settings.username, settings.password);
    client.SendMessage(buf);

    Users users;
    users.Add(settings.username);
    Messages msgs;
    msgs.Add(settings.username + ", welcome to TOBUSO-CHAT!");
    std::jthread listener_jthread(listener, std::ref(client), std::ref(msgs), std::ref(users), &screen);

    TextArea text_area;
    text_area.text_area |= CatchEvent([&](Event event) {
        if (event == event.Return) {
            if (text_area.user_message.size() == 0)
                return false;
            if (client.ServerIsRunning()) {
                buf = serializer.message(settings.username, text_area.user_message);
                client.SendMessage(buf);
                msgs.container->Add(SendMessage::toComponent(
                        settings.username, text_area.user_message, Color::BlueViolet));
                text_area.user_message.clear();
            }
            return true;
        }
        return event.is_character() && text_area.user_message.size() >= 500;
    });

    Chat chat(msgs, text_area);
    Info info;

    int tab_index = 0;
    std::vector<std::string> menu_items = {" Chat ", " Online users ", " Info "};
    auto tab_content = Container::Tab({chat.renderer, users.renderer, info.renderer}, &tab_index);
    auto menu = TabMenu(&menu_items, &tab_index);

    auto main_container = Container::Vertical({
            menu,
            tab_content,
    });

    auto main_renderer = Renderer(main_container, [&] {
        return vbox(
                {text("TOBUSO-CHAT") | bold | hcenter,
                 vbox({
                         hbox({
                                 menu->Render(),
                                 filler(),
                                 text("server: " + settings.host + ":" + std::to_string(settings.port))
                                         | align_right,
                         }),
                         tab_content->Render() | flex,
                 }) | border});
    });

    main_renderer |= CatchEvent([&](Event event) {
        if (is_forbidden) {
            screen.Exit();
            return true;
        }
        if (event == Event::Escape) {
            screen.Exit();
            return true;
        }
        if (event == Event::F10) {
            chat.is_need_focused ^= 1;
            return false;
        }

        return false;
    });

    screen.Loop(main_renderer);
    listener_jthread.request_stop();
    if (is_forbidden)
        std::cout << "[FORBIDDEN] WRONG PASSWORD\n";
}
