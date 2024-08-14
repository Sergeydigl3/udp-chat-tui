#include <memory>     // for allocator, __shared_ptr_access, shared_ptr
#include <string>     // for to_string, operator+
#include <functional> // for function
#include <iostream>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "ftxui/component/component.hpp"          // for Button, Renderer, Vertical
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for ButtonOption
#include "ftxui/component/screen_interactive.hpp" // for ScreenInteractive
#include "ftxui/dom/elements.hpp"                 // for operator|, text, Element, hbox, separator, size, vbox, border, frame, vscroll_indicator, HEIGHT, LESS_THAN
#include "ftxui/screen/color.hpp"                 // for Color, Color::Default, Color::GrayDark, Color::White

#include "ftxui/component/animation.hpp"      // for Animator, Params (ptr only)
#include "ftxui/component/captured_mouse.hpp" // for CapturedMouse
#include "ftxui/component/event.hpp"          // for Event, Event::Return
#include "ftxui/component/mouse.hpp"          // for Mouse, Mouse::Left, Mouse::Pressed       // for operator|, Decorator, Element, operator|=, bgcolor, color, reflect, text, bold, border, inverted, nothing
#include "ftxui/screen/box.hpp"               // for Box
#include "ftxui/util/ref.hpp"                 // for Ref, ConstStringRef

#include <functional>
#include "engine/chat_client_engine.hpp"
#include "ui/custom_btn.hpp"

using namespace ftxui;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <IP> <PORT>\n";
        return 1;
    }

    std::string ip = argv[1];
    int port = std::stoi(argv[2]);

    ChatClientEngine client(ip, port);

    std::string username;
    std::cout << "Enter your username: ";

    std::getline(std::cin, username);
    for (char c : username)
    {
        if (!isalnum(c))
        {
            std::cerr << "Username must be alphanumeric.\n";
            return -1;
        }
    }

    client.setUsername(username);

    auto on_click_empty = [&] {};

    auto style = ButtonOption::Animated(Color::Default, Color::GrayDark,
                                        Color::Default, Color::Magenta);
    auto style_message = MessageOption::Animated(Color::Default, Color::GrayDark,
                                                 Color::Default, Color::Magenta);

    std::string message_to_send;

    Component messaages_container;
    auto send_button_callback = [&]
    {
        if (message_to_send.empty())
            return;
        client.sendMessage(message_to_send);

        MessageOption message_option = MessageOption::Animated(Color::Default, Color::GrayDark,
                                                               Color::Default, Color::Magenta);
        ;
        message_option.fromMe = true;
        message_option.text_message = message_to_send;
        message_option.nickname = "You";
        message_option.messageip = ip;
        auto custom_button = CustomMessage(message_option);
        messaages_container->Add(custom_button);
        custom_button->TakeFocus();
        message_to_send = "";
    };
    Component input_message_to_send;
    auto input_field_submit = [&](Event event)
    {
        if (event == Event::Return)
        {
            send_button_callback();
            input_message_to_send->TakeFocus();
            return true;
        }
        return false;
    };
    input_message_to_send = Input(&message_to_send, "Type message here") | CatchEvent(input_field_submit);

    auto button = Button("Send", send_button_callback, style);
    auto bottom_container = Container::Horizontal({
        input_message_to_send,
        button,
    });

    auto bottom_panel = Renderer(bottom_container, [&]
                                 { return hbox({
                                       input_message_to_send->Render(),
                                       button->Render(),
                                   }); });

    auto messages_component_list = std::vector<Component>();

    messaages_container = Container::Vertical(messages_component_list);

    client.setMessageCallback([&](const ChatClientMessage &message)
                              {
        MessageOption message_option = MessageOption::Animated(Color::Default, Color::GrayDark,
                                        Color::Default, Color::Magenta);;
        message_option.fromMe = false;
        message_option.text_message = message.text;
        message_option.nickname = message.nickname;
        message_option.messageip = message.address;
        auto custom_button = CustomMessage(message_option);
        messaages_container->Add(custom_button);
        custom_button->TakeFocus(); });
    auto main_composition = Container::Vertical({
        messaages_container,
        bottom_panel,
    });
    auto main_renderer = Renderer(main_composition, [&]
                                  { return vbox({
                                        messaages_container->Render() | vscroll_indicator | frame | yflex | border,
                                        bottom_panel->Render() | border,
                                    }); });

    auto screen = ScreenInteractive::Fullscreen(); // ScreenInteractive::FitComponent();
    client.start();
    screen.Loop(main_renderer);

    return 0;
}
