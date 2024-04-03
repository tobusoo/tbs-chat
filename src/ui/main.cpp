#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

#include <Message.hpp>

#include <client.hpp>
#include <core/Package.pb.h>

using namespace ftxui;

Component TabMenu(std::vector<std::string>* menu, int* selected)
{
    auto option = MenuOption::HorizontalAnimated();
    option.underline.SetAnimation(std::chrono::milliseconds(1500), animation::easing::ElasticOut);
    option.entries_option.transform = [](EntryState state) {
        Element e = text(state.label) | hcenter | flex;
        if (state.active && state.focused)
            e |= bold;
        if (state.focused && !state.active)
            e |= dim;

        return e;
    };

    option.underline.color_inactive = Color::Default;
    option.underline.color_active = Color::Red;

    return Menu(menu, selected, option);
}

void listener(
        TCPClient& client,
        std::vector<core::Package>& messages,
        ftxui::Component& message_container,
        ScreenInteractive& screen)
{
    core::Package msg;
    std::string buf;

    while (client.ServerIsRunning()) {
        client.ReadMessage(buf);
        msg.ParseFromString(buf);
        messages.push_back(msg);
        message_container->Add(SendMessage::toComponent(msg.sender(), msg.content()) | focus);
        screen.PostEvent(Event::Special("new_message"));
    }
}

int main()
{
    std::cout << "\e[H\e[J";

    auto screen = ScreenInteractive::Fullscreen();
    std::vector<std::string> menu_items = {" Chat ", " Online users ", " Options "};
    std::string user_message;
    InputOption style_2 = InputOption::Default();

    // USERS_ONLINE
    std::vector<std::string> users_online;
    for (size_t i = 0; i < 15; i++) {
        users_online.push_back("user " + std::to_string(i));
    }
    auto users_container
            = Container::Vertical({Message::toComponent("Admin (aka tobuso) 😎", Color::MediumVioletRed)});
    for (int i = 0; i < 15; i++) {
        users_container->Add(Message::toComponent(users_online[i], Color::RoyalBlue1));
    }
    auto users_renderer = Renderer(users_container, [&] {
        size_t elem_cnt = users_container->ChildCount();
        size_t max_len = Terminal::Size().dimy - 10;
        size_t len = max_len > elem_cnt ? elem_cnt : max_len;
        return users_container->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, max_len);
    });
    // END USERS_ONLINE

    // OPTION
    std::vector<std::string> option;
    for (size_t i = 0; i < 50; i++) {
        option.push_back("option " + std::to_string(i));
    }
    auto option_container = Container::Vertical({});
    bool option_state = 0;
    for (int i = 0; i < 50; i++) {
        option_container->Add(Message::toComponent(option[i]));
    }
    auto option_renderer = Renderer(option_container, [&] {
        size_t elem_cnt = option_container->ChildCount();
        size_t max_len = Terminal::Size().dimy - 10;
        size_t len = max_len > elem_cnt ? elem_cnt : max_len;

        return option_container->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, len) | border;
    });
    // END OPTION

    // MESSAGES
    std::vector<core::Package> messages;
    auto message_container = Container::Vertical({});
    auto message_renderer = Renderer(message_container, [&] { return message_container->Render(); });
    // END MESSAGES
    TCPClient client(0, 6969);
    core::Package msg;
    std::string buf;

    std::thread listener_th(
            listener, std::ref(client), std::ref(messages), std::ref(message_container), std::ref(screen));
    auto text_area = Input(&user_message, "Type message for your friends...", style_2);
    text_area |= CatchEvent([&](Event event) {
        if (event == event.Return) {
            if (user_message.size() == 0)
                return false;
            if (client.ServerIsRunning()) {
                msg.set_sender("ui");
                msg.set_content(user_message.c_str());
                msg.SerializeToString(&buf);
                client.SendMessage(buf);
                message_container->Add(SendMessage::toComponent("ui", user_message));
                user_message.clear();
            }
            return true;
        }
        return event.is_character() && user_message.size() >= 500;
    });

    // CHAT
    bool is_need_focused = true;
    size_t prev_msg_cnt_size = 0;
    Box box;

    auto chat_container = Container::Vertical({message_container, text_area});
    auto chat_renderer = Renderer(chat_container, [&] {
        size_t elem_cnt = message_container->ChildCount();
        size_t max_len = Terminal::Size().dimy - 10;

        auto msg_container = message_container->Render() | reflect(box) | vscroll_indicator;
        if (is_need_focused)
            msg_container |= focusPositionRelative(0, 1.0f);
        prev_msg_cnt_size = box.y_max;
        size_t len = std::min(max_len, prev_msg_cnt_size);

        return vbox({
                window(text("Messages [focus=" + std::to_string(is_need_focused) + "]") | bold | align_right,
                       msg_container | yframe | size(HEIGHT, LESS_THAN, len)),
                hbox({text_area->Render(), text(" " + std::to_string(user_message.size()) + "/500")}) | flex
                        | border,
        });
    });
    // END CHAT

    int tab_index = 0;
    auto tab_content = Container::Tab({chat_renderer, users_renderer, option_renderer}, &tab_index);
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
                                 text("server: 127.0.0.1:6969 ") | align_right,
                         }),
                         tab_content->Render() | flex,
                 }) | border});
    });

    main_renderer |= CatchEvent([&](Event event) {
        if (event == Event::Escape) {
            screen.Exit();
            return true;
        }
        if (event == Event::Character('r')) {
            is_need_focused = !is_need_focused;
            return false;
        }

        return false;
    });

    screen.Loop(main_renderer);
}
