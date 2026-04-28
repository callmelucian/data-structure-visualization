#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <optional>

#include "../assets/theme.hpp"
#include "ui-base.hpp"

namespace UI {

class Button : public UI::Base {
private:
    UI::RoundedRectangle container;
    UI::Text label;
    std::function<void()> onClick;
    bool activation, clickable;
    std::optional<sf::Sprite> icon;
    sf::Color *containerFill, *containerOutline, *labelFill;

public:
    // constructor
    Button(float width, float height, float radius = 0.f);

    void setIcon (const sf::Texture &texture, float padding = 20.f);

    void setIcon (const sf::Texture &texture, float width, float height);

    void centerIcon();

    // set label string
    void setString(const std::string &msg);
    void setString (const std::string &msg, int characterSize);

    // scale UI elements
    void setScale(float scaleConstant);

    // set outline thickness
    void setOutlineThickness(float thickness);

    // set outline color
    void setOutlineColor(const sf::Color &color);

    // set font-size
    void setCharacterSize(unsigned int fontSize);

    // get font-size
    int getCharacterSize() const;

    // check if button is clickable
    bool isClickable() const;

    // set call-back function so the button can call whenever it is pressed
    void setCallback(std::function<void()> onClickFunction);

    // (override) get boundary of the button's shape
    sf::FloatRect getBoundary() const override;

    // (override) handle mouse press
    void handleMousePress(const sf::Vector2f &mousePos) override;

    // (override) handle mouse release
    void handleMouseRelease(const sf::Vector2f &mousePos) override;

    // (override) handle mouse movement
    void handleMouseMovement(const sf::Vector2f &mousePos) override;

    // (override) draw button onto the screen
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    // (override) handle text events
    void handleTextEntered(const char &unicode) override;

    // disable button (make un-clickable)
    void disableButton();

    // enable button (make clickable)
    void enableButton();

    // toggle clickable state
    void toggleState();

    void changeColor();
};

}; // namespace UI