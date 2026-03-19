// File cleaned up
#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

#include "../assets/theme.hpp"
#include "ui-base.hpp"

class Button : public UI::Base {
private:
    sf::RectangleShape rectangle;
    UI::Text label;
    std::function<void()> onClick;
    bool activation, clickable;

public:
    // constructor
    Button (float width, float height) :
        // call other constructors
        label(Theme::notoRegular, "Button"), activation(false), clickable(true) {

        // setup rectangle
        rectangle.setSize({width, height});
        rectangle.setOrigin({width / 2.f, height / 2.f});
        rectangle.setFillColor(Theme::getButton());
        rectangle.setOutlineThickness(2);
        rectangle.setOutlineColor(sf::Color::Black);

        // setup label
        label.setAutoCharacterSize(width, height);
        label.setFillColor(sf::Color::Black);
        label.centerOrigin();
    }

    // set label string
    void setString (const std::string &msg) {
        label.setString(msg);
        label.setAutoCharacterSize(
            rectangle.getSize().x, rectangle.getSize().y, 0.75
        );
        label.centerOrigin();
    }

    // scale UI elements
    void setScale (float scaleConstant) {
        rectangle.setScale({scaleConstant, scaleConstant});
        label.setScale({scaleConstant, scaleConstant});
    }

    // set outline thickness
    void setOutlineThickness (float thickness) {
        rectangle.setOutlineThickness(thickness);
    }

    // set outline color
    void setOutlineColor (const sf::Color &color) {
        rectangle.setOutlineColor(color);
    }

    // set font-size
    void setCharacterSize (unsigned int fontSize) {
        label.setCharacterSize(fontSize);
    }

    // get font-size
    int getCharacterSize() const {
        return label.getCharacterSize();
    }

    // check if button is clickable
    bool isClickable() const {
        return clickable;
    }

    // set call-back function so the button can call whenever it is pressed
    void setCallback (std::function<void()> onClickFunction) {
        onClick = onClickFunction;
    }

    // (override) get boundary of the button's shape
    sf::FloatRect getBoundary() const override {
        return rectangle.getGlobalBounds();
    }

    // (override) handle mouse press
    void handleMousePress (const sf::Vector2f &mousePos) override {
        if (!containPosition(mousePos) or !clickable) return;
        rectangle.setFillColor(Theme::getPressedButton());
        setScale(0.97f);
        activation = true;
    }

    // (override) handle mouse release
    void handleMouseRelease (const sf::Vector2f &mousePos) override {
        if (!activation or !clickable) return;
        rectangle.setFillColor(
            containPosition(mousePos) ? Theme::getHoveredButton() : Theme::getButton()
        );
        setScale(1.f);
        activation = false;
        onClick();
    }

    // (override) handle mouse movement
    void handleMouseMovement (const sf::Vector2f &mousePos) override {
        if (activation or !clickable) return;
        rectangle.setFillColor(
            containPosition(mousePos) ? Theme::getHoveredButton() : Theme::getButton()
        );
    }

    // (override) draw button onto the screen
    void draw (sf::RenderTarget &target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(rectangle, states);
        target.draw(label, states);
    }

    // (override) handle text events
    void handleTextEntered (const char &unicode) override {}

    // disable button (make un-clickable)
    void disableButton() {
        clickable = false;
        rectangle.setFillColor(Theme::getIdleButton());
        label.setFillColor(Theme::getSecondary());
    }

    // enable button (make clickable)
    void enableButton() {
        clickable = true;
        rectangle.setFillColor(Theme::getButton());
        label.setFillColor(Theme::getPrimary());
    }

    // toggle clickable state
    void toggleState() {
        if (isClickable()) disableButton();
        else enableButton();
    }
};