#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

#include "../assets/theme.hpp"
#include "ui-base.hpp"

extern const float BLINK_INTERVAL;

namespace UI {

class TextInputField : public UI::Base {
private:
    std::string currentMessage;
    UI::Text label;
    sf::RectangleShape rectangle, cursor;
    bool activation, focus, showCursor;
    sf::Clock blinkClock;
    std::function<void(const std::string&)> callbackFunction;

public:
    TextInputField(float width, float height);

    // set callback function for input field
    void setCallbackFunction(auto func) {
        callbackFunction = func;
    }

    // set label string
    void setString(const std::string &msg);

    // release text
    std::string releaseText();

    void setScale(float scaleConstant);

    // (override) handle mouse press
    void handleMousePress(const sf::Vector2f &mousePos) override;

    // (override) handle mouse release
    void handleMouseRelease(const sf::Vector2f &mousePos) override;

    // (override) handle mouse movement
    void handleMouseMovement(const sf::Vector2f &mousePos) override;

    bool isFocused() const;

    void handleTextEntered(const char &unicode) override;

    // (override) get boundary of the button's shape
    sf::FloatRect getBoundary() const override;

    // (override) draw button onto the screen
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    // time propagation
    void timePropagation();
};

} // namespace UI