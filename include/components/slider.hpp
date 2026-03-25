#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <cmath>
#include <algorithm>

#include "../core/utility.hpp"
#include "../assets/theme.hpp"
#include "ui-base.hpp"

namespace UI {

class Slider : public UI::Base {
private:
    sf::RectangleShape holder;
    sf::RectangleShape hovered;
    sf::RectangleShape filled;
    UI::Text annotation, label;
    std::function<void(float)> changeValue;
    std::function<std::string(float)> displayValue;
    bool followMouse;

public:
    Slider(float width, float height);

    void setString(const std::string &msg);

    // set change-value function
    void setChangeValueFunction(auto cvf) {
        changeValue = cvf;
    }

    // set display-value function
    void setDisplayValueFunction(auto dvf) {
        displayValue = dvf;
    }

    // (override) draw slider onto the screen
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // (override) get boundary
    sf::FloatRect getBoundary() const override;

    // update slider value
    void updateSliderValue();

    // (override) handle mouse press
    void handleMousePress(const sf::Vector2f &mousePos) override;

    // (override) handle mouse release
    void handleMouseRelease(const sf::Vector2f &mousePos) override;

    // (override) handle mouse movement
    void handleMouseMovement(const sf::Vector2f &mousePos) override;

    // (override) handle text events
    void handleTextEntered(const char &unicode) override;
};

} // namespace UI