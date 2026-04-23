#include "../../include/components/button.hpp"
#include <iostream>

namespace UI {

Button::Button(float width, float height) :
    label(Theme::notoRegular, "Button"), activation(false), clickable(true) {

    rectangle.setSize({width, height});
    rectangle.setOrigin({width / 2.f, height / 2.f});
    rectangle.setFillColor(Theme::getButton());
    rectangle.setOutlineThickness(2);
    rectangle.setOutlineColor(sf::Color::Black);

    label.setAutoCharacterSize(width, height);
    label.setFillColor(sf::Color::Black);
    label.centerOrigin();
}

void Button::setString(const std::string &msg) {
    label.setString(msg);
    label.setAutoCharacterSize(
        rectangle.getSize().x, rectangle.getSize().y, 0.75
    );
    label.centerOrigin();
}

void Button::setScale(float scaleConstant) {
    rectangle.setScale({scaleConstant, scaleConstant});
    label.setScale({scaleConstant, scaleConstant});
}

void Button::setOutlineThickness(float thickness) {
    rectangle.setOutlineThickness(thickness);
}

void Button::setOutlineColor(const sf::Color &color) {
    rectangle.setOutlineColor(color);
}

void Button::setCharacterSize(unsigned int fontSize) {
    label.setCharacterSize(fontSize);
    label.centerOrigin();
}

int Button::getCharacterSize() const {
    return label.getCharacterSize();
}

bool Button::isClickable() const {
    return clickable;
}

void Button::setCallback(std::function<void()> onClickFunction) {
    onClick = onClickFunction;
}

sf::FloatRect Button::getBoundary() const {
    return rectangle.getGlobalBounds();
}

void Button::handleMousePress(const sf::Vector2f &mousePos) {
    if (!containPosition(mousePos) or !clickable) return;
    rectangle.setFillColor(Theme::getPressedButton());
    setScale(0.97f);
    activation = true;
}

void Button::handleMouseRelease(const sf::Vector2f &mousePos) {
    if (!activation or !clickable) return;
    rectangle.setFillColor(
        containPosition(mousePos) ? Theme::getHoveredButton() : Theme::getButton()
    );
    setScale(1.f);
    activation = false;
    onClick();
}

void Button::handleMouseMovement(const sf::Vector2f &mousePos) {
    if (activation or !clickable) return;
    rectangle.setFillColor(
        containPosition(mousePos) ? Theme::getHoveredButton() : Theme::getButton()
    );
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(rectangle, states);
    target.draw(label, states);
}

void Button::handleTextEntered(const char &unicode) {}

void Button::disableButton() {
    clickable = false;
    rectangle.setFillColor(Theme::getIdleButton());
    label.setFillColor(Theme::getSecondary());
}

void Button::enableButton() {
    clickable = true;
    rectangle.setFillColor(Theme::getButton());
    label.setFillColor(Theme::getPrimary());
}

void Button::toggleState() {
    if (isClickable()) disableButton();
    else enableButton();
}

}; // namespace UI