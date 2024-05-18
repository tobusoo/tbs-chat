#pragma once
#include <set>

#include <ftxui/component/component.hpp>

#include <ui/MessageComponent.hpp>

namespace ftxui {

class Users {
public:
    std::set<std::string> online;
    ftxui::Component container;
    ftxui::Component renderer;

public:
    Users();
    void Add(const std::string& user);
    void Remove(const std::string& user);
};

}; // namespace ftxui