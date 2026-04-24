#include "../../include/components/input-field.hpp"

const float BLINK_INTERVAL = 0.3f;

namespace UI {

TextInputField::TextInputField (float width, float height, float radius) :
    content(Theme::googleSansRegular),
    fieldLabel(Theme::googleSansItalic),
    container(width, height, radius),
    focus(false), showCursor(false), enableFlag(true) {
    
    // setup rectangle
    container.setOrigin({width / 2.f, height / 2.f});
    container.setFillColor(Theme::getButton());
    container.setOutlineThickness(2);
    container.setOutlineColor(Theme::getTextPrimary());

    // setup cursor
    cursor.setSize({2.f, height * 0.8f});
    cursor.setOrigin({1.f, height * 0.4f});
    cursor.setFillColor(sf::Color({80, 80, 80}));
    cursor.setPosition({content.getWidth() / 2.f, 0});

    // setup content & label
    content.setFillColor(Theme::getTextPrimary());
    content.centerOrigin();
    content.setCharacterSize(20);
    fieldLabel.setFillColor(Theme::getTextPrimary());
    fieldLabel.centerOrigin();
    fieldLabel.setPosition({0.f, -(height / 2.f + 12.f)});
    fieldLabel.setCharacterSize(14);
}

void TextInputField::setString (const std::string &msg) {
    content.setString(msg);
    // content.setAutoCharacterSize(
    //     container.getSize().x, container.getSize().y, 0.75
    // );
    content.centerOrigin();
    cursor.setPosition({content.getWidth() / 2.f, 0});
}

void TextInputField::setLabel (const std::string &msg) {
    fieldLabel.setString(msg);
    fieldLabel.centerOrigin();
}

std::string TextInputField::releaseText() {
    std::string holder = currentMessage;
    currentMessage = "", setString("");
    if (holder.size()) callbackFunction(holder);
    return holder;
}

void TextInputField::setScale(float scaleConstant) {
    container.setScale({scaleConstant, scaleConstant});
    content.setScale({scaleConstant, scaleConstant});
    fieldLabel.setScale({scaleConstant, scaleConstant});
}

void TextInputField::handleMousePress(const sf::Vector2f &mousePos) {
    if (!enableFlag) return;
    bool contained = containPosition(mousePos);
    if (isFocused() && contained) blinkClock.restart();
    if (!contained) showCursor = false;
    container.setFillColor(
        contained ? Theme::getPressedButton() : Theme::getButton()
    );
    focus = contained;
}

void TextInputField::handleMouseRelease(const sf::Vector2f &mousePos) {}

void TextInputField::handleMouseMovement(const sf::Vector2f &mousePos) {
    if (focus || !enableFlag) return;
    container.setFillColor(
        containPosition(mousePos) ? Theme::getHoveredButton() : Theme::getButton()
    );
}

bool TextInputField::isFocused() const { 
    return focus; 
}

bool TextInputField::isEnabled() const {
    return enableFlag;
}

void TextInputField::handleTextEntered(const char &unicode) {
    if (!isFocused() || !enableFlag) return;
    if (unicode == 8) { // handle backspace
        if (currentMessage.size())
            currentMessage.erase(currentMessage.size() - 1);
    }
    else if ((unicode >= 32 && unicode < 128) || unicode == ' ')
        currentMessage += unicode;
    else if (unicode == 10 || unicode == 13)
        releaseText();
    setString(currentMessage);
}

sf::FloatRect TextInputField::getBoundary() const {
    return container.getGlobalBounds();
}

void TextInputField::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (!enableFlag) return;
    states.transform *= getTransform();
    target.draw(container, states);
    target.draw(fieldLabel, states);
    target.draw(content, states);
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
    enableFlag = true;
}

void TextInputField::disable() {
    enableFlag = false, focus = false, showCursor = false;
    currentMessage = "";
}

}; // namespace UI