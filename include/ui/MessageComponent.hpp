#pragma once

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/box.hpp"
#include <string>

namespace ftxui {

class Message {
private:
    static ftxui::CheckboxOption SendMessageOption()
    {
        auto option = ftxui::CheckboxOption();
        option.transform = [](const ftxui::EntryState& s) {
            auto t = text(s.label + ": ");
            if (s.active) {
                t |= bold;
            }
            if (s.focused) {
                t |= inverted;
            }
            return t;
        };

        return option;
    }

    static ftxui::CheckboxOption MessageOption()
    {
        auto option = ftxui::CheckboxOption();
        option.transform = [](const ftxui::EntryState& s) {
            auto t = paragraphAlignLeft(s.label);
            if (s.active) {
                t |= bold;
            }
            if (s.focused) {
                t |= inverted;
            }
            return t;
        };

        return option;
    }

public:
    static Component
    toComponent(const std::string& content, Color color_ = Color::Default, bool is_sender = false)
    {
        class Impl : public ComponentBase {
        private:
            std::string content_;
            Color clr = Color::Default;
            bool hovered_ = false;
            Box box_;
            bool is_sender = false;

            Element Render() override
            {
                const bool is_focused = Focused();
                const bool is_active = Active();
                auto focus_management = is_focused ? focus : is_active ? ftxui::select : nothing;
                auto entry_state = EntryState{
                        content_,
                        0,
                        is_active,
                        is_focused || hovered_,
                };
                Element element;
                if (is_sender)
                    element = SendMessageOption().transform(entry_state);
                else
                    element = MessageOption().transform(entry_state);

                return element | focus_management | reflect(box_) | color(clr);
            }

            bool OnEvent(Event event) override
            {
                if (!CaptureMouse(event)) {
                    return false;
                }

                if (event.is_mouse()) {
                    return OnMouseEvent(event);
                }

                hovered_ = false;
                TakeFocus();
                if (event == Event::Character(' ') || event == Event::Return) {
                    return true;
                }

                return false;
            }

            bool OnMouseEvent(Event event)
            {
                hovered_ = box_.Contain(event.mouse().x, event.mouse().y);

                if (!CaptureMouse(event))
                    return false;

                if (!hovered_)
                    return false;

                if (event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
                    TakeFocus();
                    return true;
                }

                return false;
            }

            bool Focusable() const final
            {
                return true;
            }

        public:
            Impl(const std::string& content, Color clr = Color::Default, bool is_sender = false)
                : content_(content), clr(clr), is_sender(is_sender)
            {
            }
        };

        return Make<Impl>(content, color_, is_sender);
    }
};

class SendMessage {
public:
    static Component toComponent(
            const std::string& sender,
            const std::string& message,
            Color sender_clr = Color::Default,
            Color message_clr = Color::Default)
    {
        return Container::Horizontal({
                Message::toComponent(sender, sender_clr, true),
                Message::toComponent(message, message_clr),
        });
    }
};

}; // namespace ftxui
