#include <ui/TabMenu.hpp>
namespace ftxui {

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

}; // namespace ftxui