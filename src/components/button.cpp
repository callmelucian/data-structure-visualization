#include "../../include/components/button.hpp"
#include <iostream>

namespace UI {

Button::Button(float width, float height, float radius) :
    container(width, height, radius), label(Theme::googleSansRegular), activation(false), clickable(true),
    containerFill(&Theme::button), containerOutline(&Theme::textPrimary), labelFill(&Theme::textPrimary) {

    container.setOrigin({width / 2.f, height / 2.f});
    label.setAutoCharacterSize(width, height);
    label.centerOrigin();
}

void Button::setIcon(const sf::Texture& texture, float padding) {
    icon.emplace(texture);
    float scaleX = (container.getSize().x - padding) / icon->getLocalBounds().size.x;
    float scaleY = (container.getSize().y - padding) / icon->getLocalBounds().size.y;
    float scale = std::min(scaleX, scaleY);
    icon->setScale({scale, scale});
    centerIcon();
}

void Button::setIcon(const sf::Texture& texture, float width, float height) {
    icon.emplace(texture);
    float scaleX = width / icon->getLocalBounds().size.x;
    float scaleY = height / icon->getLocalBounds().size.y;
    float scale = std::min(scaleX, scaleY);
    icon->setScale({scale, scale});
    centerIcon();
}

void Button::centerIcon() {
    if (!icon) return;
    sf::FloatRect b = container.getGlobalBounds();
    sf::FloatRect i = icon->getGlobalBounds();
    icon->setPosition({
        b.position.x + (b.size.x - i.size.x) / 2.f,
        b.position.y + (b.size.y - i.size.y) / 2.f
    });
}

void Button::setString(const std::string &msg) {
    label.setString(msg);
    label.setAutoCharacterSize(
        container.getSize().x, container.getSize().y, 0.75
    );
    label.centerOrigin();
}

void Button::setString (const std::string &msg, int characterSize) {
    label.setString(msg);
    label.setCharacterSize(characterSize);
    label.centerOrigin();
}

void Button::setScale(float scaleConstant) {
    container.setScale({scaleConstant, scaleConstant});
    label.setScale({scaleConstant, scaleConstant});
}

void Button::setOutlineThickness(float thickness) {
    container.setOutlineThickness(thickness);
}

void Button::setOutlineColor(const sf::Color &color) {
    container.setOutlineColor(color);
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
    return container.getGlobalBounds();
}

void Button::handleMousePress(const sf::Vector2f &mousePos) {
    if (!containPosition(mousePos) or !clickable) return;
    // container.setFillColor(Theme::getPressedButton());
    containerFill = &Theme::buttonPressed;
    setScale(0.97f);
    activation = true;
}

void Button::handleMouseRelease(const sf::Vector2f &mousePos) {
    if (!activation or !clickable) return;
    // container.setFillColor(
    //     containPosition(mousePos) ? Theme::getHoveredButton() : Theme::getButton()
    // );
    containerFill = containPosition(mousePos) ? &Theme::buttonHovered : &Theme::button;
    setScale(1.f);
    activation = false;
    onClick();
}

void Button::handleMouseMovement(const sf::Vector2f &mousePos) {
    if (activation or !clickable) return;
    // container.setFillColor(
    //     containPosition(mousePos) ? Theme::getHoveredButton() : Theme::getButton()
    // );
    containerFill = containPosition(mousePos) ? &Theme::buttonHovered : &Theme::button;
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(container, states);
    target.draw(label, states);
    if (icon) target.draw(*icon, states);
}

void Button::handleTextEntered(const char &unicode) {}

void Button::disableButton() {
    clickable = false;
    containerFill = &Theme::button;
    labelFill = &Theme::textSecondary;
    // container.setFillColor(Theme::getButton());
    // label.setFillColor(Theme::getTextSecondary());
}

void Button::enableButton() {
    clickable = true;
    // label.setFillColor(Theme::getTextPrimary());
    labelFill = &Theme::textPrimary;
}

void Button::toggleState() {
    if (isClickable()) disableButton();
    else enableButton();
}

void Button::changeColor() {
    container.setFillColor(*containerFill);
    container.setOutlineColor(*containerOutline);
    label.setFillColor(*labelFill);
}

}; // namespace UI