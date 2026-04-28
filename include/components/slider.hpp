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
    UI::RoundedRectangle holder, hovered, filled;
    UI::Text annotation;
    bool followMouse;
    float radius;

    sf::Color *holderColor, *fillColor, *textColor;

    std::function<void(float)> changeValue;

public:
    Slider (float width, float height, float radius = 20.f);

    void setString(const std::string &msg);

    void setCharacterSize (unsigned characterSize);

    // set change-value function
    void setChangeValueFunction(auto cvf) {
        changeValue = cvf;
    }

    // (override) draw slider onto the screen
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setOutlineThickness (float thickness);
    
    // (override) get boundary
    sf::FloatRect getBoundary() const override;

    // update slider value
    void updateSliderValue();

    void changeColor();

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