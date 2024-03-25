#pragma once

#include <functional> // for function
#include <utility>    // for move

#include "ftxui/component/captured_mouse.hpp"    // for CapturedMouse
#include "ftxui/component/component.hpp"         // for Make, Checkbox
#include "ftxui/component/component_base.hpp"    // for Component, ComponentBase
#include "ftxui/component/component_options.hpp" // for CheckboxOption, EntryState
#include "ftxui/component/event.hpp"             // for Event, Event::Return
#include "ftxui/component/mouse.hpp"             // for Mouse, Mouse::Left, Mouse::Pressed
#include "ftxui/dom/elements.hpp"                // for operator|, Element, reflect, focus, nothing, select
#include "ftxui/screen/box.hpp"                  // for Box
#include "ftxui/util/ref.hpp"                    // for Ref, ConstStringRef
#include <string>

namespace ftxui {

enum MessageType { MessageT };

class Message {
private:
    std::string message_;
    Color color_;

    static ftxui::CheckboxOption myOpt()
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

    static ftxui::CheckboxOption myOptv2()
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
    Message(const char* msg, Color color = Color::Default) : message_(msg), color_(color)
    {
    }

    ~Message() = default;

    Component toComponent()
    {
        return toComponent(message_, color_);
    }

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
                    element = myOpt().transform(entry_state);
                else
                    element = myOptv2().transform(entry_state);

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
    Message sender_;
    Message message_;

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
