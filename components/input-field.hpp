#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

#include "../assets/theme.hpp"
#include "ui-base.hpp"

const float BLINK_INTERVAL = 0.3f;

class TextInputField : public UI::Base {
private:
    std::string currentMessage;
    UI::Text label;
    sf::RectangleShape rectangle, cursor;
    bool activation, focus, showCursor;
    sf::Clock blinkClock;
    std::function<void(const std::string&)> callbackFunction;

public:
    TextInputField (float width, float height) : label(Theme::notoRegular), activation(false), focus(false), showCursor(false) {
        // setup rectangle
        rectangle.setSize({width, height});
        rectangle.setOrigin({width / 2.f, height / 2.f});
        rectangle.setFillColor(Theme::getButton());
        rectangle.setOutlineThickness(2);
        rectangle.setOutlineColor(sf::Color::Black);

        // setup cursor
        cursor.setSize({2.f, height * 0.8f});
        cursor.setOrigin({1.f, height * 0.4f});
        cursor.setFillColor(sf::Color({80, 80, 80}));
        cursor.setPosition({label.getWidth() / 2.f, 0});

        // setup label
        label.setFillColor(sf::Color::Black);
        label.centerOrigin();
    }

    // set callback function for input field
    void setCallbackFunction (auto func) {
        callbackFunction = func;
    }

    // set label string
    void setString (const std::string &msg) {
        label.setString(msg);
        label.setAutoCharacterSize(
            rectangle.getSize().x, rectangle.getSize().y, 0.75
        );
        label.centerOrigin();
        cursor.setPosition({label.getWidth() / 2.f, 0});
    }

    // release text
    std::string releaseText() {
        std::string holder = currentMessage;
        currentMessage = "", setString("");
        if (holder.size()) callbackFunction(holder);
        return holder;
    }

    void setScale (float scaleConstant) {
        rectangle.setScale({scaleConstant, scaleConstant});
        label.setScale({scaleConstant, scaleConstant});
    }

    // (override) handle mouse press
    void handleMousePress (const sf::Vector2f &mousePos) override {
        bool contained = containPosition(mousePos);
        if (isFocused() && contained) blinkClock.restart();
        if (!contained) showCursor = false;
        rectangle.setFillColor(
            contained ? Theme::getPressedButton() : Theme::getButton()
        );
        focus = contained;
    }

    // (override) handle mouse release
    void handleMouseRelease (const sf::Vector2f &mousePos) override {}

    // (override) handle mouse movement
    void handleMouseMovement (const sf::Vector2f &mousePos) override {
        if (focus) return;
        rectangle.setFillColor(
            containPosition(mousePos) ? Theme::getHoveredButton() : Theme::getButton()
        );
    }

    bool isFocused() const { return focus; }

    void handleTextEntered (const char &unicode) override {
        if (!isFocused()) return;
        if (unicode == 8) { // handle backspace
            if (currentMessage.size())
                currentMessage.erase(currentMessage.size() - 1);
        }
        else if (unicode >= 32 && unicode < 128)
            currentMessage += unicode;
        else if (unicode == 10 || unicode == 13)
            releaseText();
        setString(currentMessage);
    }

    // (override) get boundary of the button's shape
    sf::FloatRect getBoundary() const override {
        return rectangle.getGlobalBounds();
    }

    // (override) draw button onto the screen
    void draw (sf::RenderTarget &target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(rectangle, states);
        target.draw(label, states);
        if (showCursor) target.draw(cursor, states);
    }

    // time propagation
    void timePropagation() {
        if (!isFocused()) return;
        if (blinkClock.getElapsedTime().asSeconds() >= BLINK_INTERVAL) {
            showCursor = !showCursor;
            blinkClock.restart();
        }
    }
};