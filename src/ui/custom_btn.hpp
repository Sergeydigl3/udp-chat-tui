#ifndef CUSTOM_MESSAGE_H
#define CUSTOM_MESSAGE_H

#include <functional> // for function
#include <memory>     // for make_shared, shared_ptr
#include <string>     // for wstring
#include <utility>    // for forward
#include <vector>     // for vector

#include "ftxui/component/component_base.hpp"    // for Component, Components
#include "ftxui/component/component_options.hpp" // for ButtonOption, CheckboxOption, MenuOption
#include "ftxui/dom/elements.hpp"                // for Element
#include "ftxui/util/ref.hpp"                    // for ConstRef, Ref, ConstStringRef, ConstStringListRef, StringRef
#include "custom_msg_option.hpp"

namespace ftxui
{
    
    // Component Scroller(Component child);
    Component CustomMessage(MessageOption options);
    // Component CustomMessage(ConstStringRef label,
    //                  std::function<void()> on_click,
    //                  MessageOption options = MessageOption::Simple());
}
#endif /* end of include guard: SCROLLER_H */
