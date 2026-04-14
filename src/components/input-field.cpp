#include "../../include/components/input-field.hpp"

const float BLINK_INTERVAL = 0.3f;

namespace UI {

TextInputField::TextInputField(float width, float height) 
    : label(Theme::notoRegular), focus(false), showCursor(false), isEnabled(true) {
    
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

void TextInputField::setString(const std::string &msg) {
    label.setString(msg);
    label.setAutoCharacterSize(
        rectangle.getSize().x, rectangle.getSize().y, 0.75
    );
    label.centerOrigin();
    cursor.setPosition({label.getWidth() / 2.f, 0});
}

std::string TextInputField::releaseText() {
    std::string holder = currentMessage;
    currentMessage = "", setString("");
    if (holder.size()) callbackFunction(holder);
    return holder;
}

void TextInputField::setScale(float scaleConstant) {
    rectangle.setScale({scaleConstant, scaleConstant});
    label.setScale({scaleConstant, scaleConstant});
}

void TextInputField::handleMousePress(const sf::Vector2f &mousePos) {
    if (!isEnabled) return;
    bool contained = containPosition(mousePos);
    if (isFocused() && contained) blinkClock.restart();
    if (!contained) showCursor = false;
    rectangle.setFillColor(
        contained ? Theme::getPressedButton() : Theme::getButton()
    );
    focus = contained;
}

void TextInputField::handleMouseRelease(const sf::Vector2f &mousePos) {}

void TextInputField::handleMouseMovement(const sf::Vector2f &mousePos) {
    if (focus || !isEnabled) return;
    rectangle.setFillColor(
        containPosition(mousePos) ? Theme::getHoveredButton() : Theme::getButton()
    );
}

bool TextInputField::isFocused() const { 
    return focus; 
}

void TextInputField::handleTextEntered(const char &unicode) {
    if (!isFocused() || !isEnabled) return;
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

sf::FloatRect TextInputField::getBoundary() const {
    return rectangle.getGlobalBounds();
}

void TextInputField::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(rectangle, states);
    target.draw(label, states);
    if (showCursor) target.draw(cursor, states);
}

void TextInputField::timePropagation() {
    if (!isFocused()) return;
    if (blinkClock.getElapsedTime().asSeconds() >= BLINK_INTERVAL) {
        showCursor = !showCursor;
        blinkClock.restart();
    }
}

void TextInputField::enable() {
    isEnabled = true;
    rectangle.setFillColor(Theme::getButton());
}

void TextInputField::disable() {
    isEnabled = false, focus = false, showCursor = false;
    currentMessage = "";
    rectangle.setFillColor(Theme::getIdleButton());
}

}; // namespace UI