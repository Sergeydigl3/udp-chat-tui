#ifndef CUSTOM_MESSAGE_OPTION_H
#define CUSTOM_MESSAGE_OPTION_H

#include <chrono>                        // for milliseconds
#include <ftxui/component/animation.hpp> // for Duration, QuadraticInOut, Function
#include <ftxui/dom/direction.hpp>       // for Direction, Direction::Left, Direction::Right, Direction::Down
#include <ftxui/dom/elements.hpp>        // for Element, separator
#include <ftxui/util/ref.hpp>            // for Ref, ConstRef, StringRef
#include <functional>                    // for function
#include <optional>                      // for optional
#include <string>                        // for string

#include "ftxui/component/component_base.hpp" // for Component
#include "ftxui/screen/color.hpp"             // for Color, Color::GrayDark, Color::White

namespace ftxui
{
    struct MessageEntryState
    {
        std::string nickname = "Random";
        std::string text_message = "Random";
        std::string messageip = "Random";
        bool fromMe = true;
        bool active = false;
        bool focused = false;
        bool state = false;
    };

    struct MessageOption
    {
        // Standard constructors:
        static MessageOption Ascii();
        static MessageOption Simple();
        static MessageOption Border();
        static MessageOption Animated();
        static MessageOption Animated(Color color);
        static MessageOption Animated(Color background, Color foreground);
        static MessageOption Animated(Color background,
                                     Color foreground,
                                     Color background_active,
                                     Color foreground_active);

        ConstStringRef nickname = "Random";
        std::string text_message = "Random";
        ConstStringRef messageip = "Random";
        bool fromMe = true;
        std::function<void()> on_click = [] {};
        // Style:
        std::function<Element(const EntryState &)> transform;
        AnimatedColorsOption animated_colors;
    };

}
#endif