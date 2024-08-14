#include <functional> // for function
#include <memory>     // for shared_ptr
#include <utility>    // for move

#include "ftxui/component/animation.hpp"          // for Animator, Params (ptr only)
#include "ftxui/component/captured_mouse.hpp"     // for CapturedMouse
#include "ftxui/component/component.hpp"          // for Make, Button
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for ButtonOption, AnimatedColorOption, AnimatedColorsOption, EntryState
#include "ftxui/component/event.hpp"              // for Event, Event::Return
#include "ftxui/component/mouse.hpp"              // for Mouse, Mouse::Left, Mouse::Pressed
#include "ftxui/component/screen_interactive.hpp" // for Component
#include "ftxui/dom/elements.hpp"                 // for operator|, Decorator, Element, operator|=, bgcolor, color, reflect, text, bold, border, inverted, nothing
#include "ftxui/screen/box.hpp"                   // for Box
#include "ftxui/screen/color.hpp"                 // for Color
#include "ftxui/util/ref.hpp"                     // for Ref, ConstStringRef

#include "custom_btn.hpp"
#include "custom_msg_option.hpp"

namespace ftxui
{

  namespace
  {

    Element CustomDefaultTransform(MessageEntryState params)
    { // NOLINT
      Element element;
      if (params.fromMe) element = hbox({filler(), text(params.text_message) | border});
      else element = vbox({
        hbox({text(params.nickname) | bold, text("("+params.messageip+")") | color(Color::Red) }),
        hbox({text(params.text_message) | border, filler()}),
        emptyElement() | size(HEIGHT, EQUAL, 2)
        
        });

      if (params.active)
      {
        element |= bold;
      }
      if (params.focused)
      {
        // element |= inverted;
      }
      return element;
    }

    class CustomMessageBase : public ComponentBase, public MessageOption
    {
    public:
      explicit CustomMessageBase(MessageOption option) : MessageOption(std::move(option)) {}

      // Component implementation:
      Element Render() override
      {
        const bool active = Active();
        const bool focused = Focused();
        const bool focused_or_hover = focused || mouse_hover_;

        float target = focused_or_hover ? 1.f : 0.f; // NOLINT
        if (target != animator_background_.to())
        {
          SetAnimationTarget(target);
        }

        auto focus_management = focused ? focus : active ? select
                                                         : nothing;
        const MessageEntryState state = {
            *nickname,
            text_message,
            *messageip,
            fromMe,
            active,
            focused_or_hover};

        // auto element = (transform ? transform : CustomDefaultTransform) //
        //     (state);
        auto element = CustomDefaultTransform(state);
        return element | AnimatedColorStyle() | focus_management | reflect(box_);
      }

      Decorator AnimatedColorStyle()
      {
        Decorator style = nothing;
        if (animated_colors.background.enabled)
        {
          style = style |
                  bgcolor(Color::Interpolate(animation_foreground_, //
                                             animated_colors.background.inactive,
                                             animated_colors.background.active));
        }
        if (animated_colors.foreground.enabled)
        {
          style =
              style | color(Color::Interpolate(animation_foreground_, //
                                               animated_colors.foreground.inactive,
                                               animated_colors.foreground.active));
        }
        return style;
      }

      void SetAnimationTarget(float target)
      {
        if (animated_colors.foreground.enabled)
        {
          animator_foreground_ = animation::Animator(
              &animation_foreground_, target, animated_colors.foreground.duration,
              animated_colors.foreground.function);
        }
        if (animated_colors.background.enabled)
        {
          animator_background_ = animation::Animator(
              &animation_background_, target, animated_colors.background.duration,
              animated_colors.background.function);
        }
      }

      void OnAnimation(animation::Params &p) override
      {
        animator_background_.OnAnimation(p);
        animator_foreground_.OnAnimation(p);
      }

      void OnClick()
      {
        animation_background_ = 0.5F; // NOLINT
        animation_foreground_ = 0.5F; // NOLINT
        SetAnimationTarget(1.F);      // NOLINT
        on_click(); // May delete this.
      }

      bool OnEvent(Event event) override
      {
        if (event.is_mouse())
        {
          return OnMouseEvent(event);
        }

        if (event == Event::Return)
        {
          OnClick(); // May delete this.
          return true;
        }
        return false;
      }

      bool OnMouseEvent(Event event)
      {
        mouse_hover_ =
            box_.Contain(event.mouse().x, event.mouse().y) && CaptureMouse(event);

        if (!mouse_hover_)
        {
          return false;
        }

        if (event.mouse().button == Mouse::Left &&
            event.mouse().motion == Mouse::Pressed)
        {
          TakeFocus();
          OnClick(); // May delete this.
          return true;
        }

        return false;
      }

      bool Focusable() const final { return true; }

    private:
      bool mouse_hover_ = false;
      Box box_;
      MessageOption option_;

      float animation_background_ = 0;
      float animation_foreground_ = 0;
      animation::Animator animator_background_ =
          animation::Animator(&animation_background_);
      animation::Animator animator_foreground_ =
          animation::Animator(&animation_foreground_);
    };

  } // namespace

  /// @brief Draw a button. Execute a function when clicked.
  /// @param option Additional optional parameters.
  /// @ingroup component
  /// @see ButtonBase
  ///
  /// ### Example
  ///
  /// ```cpp
  /// auto screen = ScreenInteractive::FitComponent();
  /// Component button = Button({
  ///   .label = "Click to quit",
  ///   .on_click = screen.ExitLoopClosure(),
  /// });
  /// screen.Loop(button)
  /// ```
  ///
  /// ### Output
  ///
  /// ```bash
  /// ┌─────────────┐
  /// │Click to quit│
  /// └─────────────┘
  /// ```
  Component CustomMessage(MessageOption option)
  {
    return Make<CustomMessageBase>(std::move(option));
  }

  /// @brief Draw a button. Execute a function when clicked.
  /// @param label The label of the button.
  /// @param on_click The action to execute when clicked.
  /// @param option Additional optional parameters.
  /// @ingroup component
  /// @see ButtonBase
  ///
  /// ### Example
  ///
  /// ```cpp
  /// auto screen = ScreenInteractive::FitComponent();
  /// std::string label = "Click to quit";
  /// Component button = Button(&label, screen.ExitLoopClosure());
  /// screen.Loop(button)
  /// ```
  ///
  /// ### Output
  ///
  /// ```bash
  /// ┌─────────────┐
  /// │Click to quit│
  /// └─────────────┘
  /// ```
  // NOLINTNEXTLINE
  // Component CustomMessage(ConstStringRef nickname, ConstStringRef message, ConstStringRef ip
  //                        MessageOption option)
  // {
  //   option.label = label;
  //   option.on_click = std::move([&] {});
  //   return Make<CustomMessageBase>(std::move(option));
  // }

} // namespace ftxui