#include "ftxui/component/component_options.hpp"

#include <ftxui/dom/linear_gradient.hpp> // for LinearGradient
#include <ftxui/screen/color.hpp>        // for Color, Color::White, Color::Black, Color::GrayDark, Color::Blue, Color::GrayLight, Color::Red
#include <memory>                        // for shared_ptr
#include <utility>                       // for move

#include "ftxui/component/animation.hpp" // for Function, Duration
#include "ftxui/dom/elements.hpp"

#include "custom_msg_option.hpp"

namespace ftxui
{
    MessageOption MessageOption::Ascii()
    {
        MessageOption option;
        option.transform = [](const EntryState &s)
        {
            const std::string t = s.focused ? "[" + s.label + "]" //
                                            : " " + s.label + " ";
            return text(t);
        };
        return option;
    }

    /// @brief Create a MessageOption, inverted when focused.
    /// @ingroup component
    // static
    MessageOption MessageOption::Simple()
    {
        MessageOption option;
        option.transform = [](const EntryState &s)
        {
            auto element = text(s.label) | borderLight;
            if (s.focused)
            {
                element |= inverted;
            }
            return element;
        };
        return option;
    }

    /// @brief Create a MessageOption. The button is shown using a border, inverted
    /// when focused. This is the current default.
    /// @ingroup component
    MessageOption MessageOption::Border()
    {
        MessageOption option;
        option.transform = [](const EntryState &s)
        {
            auto element = text(s.label) | border;
            if (s.active)
            {
                element |= bold;
            }
            if (s.focused)
            {
                element |= inverted;
            }
            return element;
        };
        return option;
    }

    /// @brief Create a MessageOption, using animated colors.
    /// @ingroup component
    // static
    MessageOption MessageOption::Animated()
    {
        return Animated(Color::Black, Color::GrayLight, //
                        Color::GrayDark, Color::White);
    }

    /// @brief Create a MessageOption, using animated colors.
    /// @ingroup component
    // static
    MessageOption MessageOption::Animated(Color color)
    {
        return MessageOption::Animated(
            Color::Interpolate(0.85F, color, Color::Black),  // NOLINT
            Color::Interpolate(0.10F, color, Color::White),  // NOLINT
            Color::Interpolate(0.10F, color, Color::Black),  // NOLINT
            Color::Interpolate(0.85F, color, Color::White)); // NOLINT
    }

    /// @brief Create a MessageOption, using animated colors.
    /// @ingroup component
    // static
    MessageOption MessageOption::Animated(Color background, Color foreground)
    {
        // NOLINTBEGIN
        return MessageOption::Animated(
            /*bakground=*/background,
            /*foreground=*/foreground,
            /*background_active=*/foreground,
            /*foreground_active=*/background);
        // NOLINTEND
    }

    /// @brief Create a MessageOption, using animated colors.
    /// @ingroup component
    // static
    MessageOption MessageOption::Animated(Color background,
                                          Color foreground,
                                          Color background_active,
                                          Color foreground_active)
    {
        MessageOption option;
        option.transform = [](const EntryState &s)
        {
            auto element = text(s.label) | borderEmpty;
            if (s.focused)
            {
                element |= bold;
            }
            return element;
        };
        option.animated_colors.foreground.Set(foreground, foreground_active);
        option.animated_colors.background.Set(background, background_active);
        return option;
    }
}