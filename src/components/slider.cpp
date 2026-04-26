#include "../../include/components/slider.hpp"

const sf::Color holderColor = sf::Color::White;
const sf::Color hoveredColor = sf::Color({0, 0, 0, 55});

namespace UI {

Slider::Slider (float width, float height, float radius) :
    holder(width, height, radius),
    hovered(0.f, height, radius),
    filled(width / 2.f, height, radius),
    annotation(Theme::googleSansRegular),
    followMouse(false), radius(radius) {
    
    // setup holder bar
    // holder.setOutlineColor(Theme::getPrimary());
    // holder.setOutlineThickness(2.f);
    holder.setFillColor(holderColor);

    // setup hover bar
    hovered.setFillColor(hoveredColor);

    // setup filled bar
    filled.setFillColor(Theme::getPressedButton());

    // setup annotation
    annotation.setFillColor(Theme::getTextPrimary());
    annotation.setAutoCharacterSize(width, height);
    annotation.setPosition({width / 2.f, height / 2.f});
    setString("50%");

    annotation.setAutoCharacterSize(holder.getSize().x, holder.getSize().y, 0.7);
    annotation.centerOrigin();
    
    // center origin
    setOrigin({width / 2.f, height / 2.f});
}

void Slider::setString(const std::string &msg) {
    annotation.setString(msg);
    annotation.centerOrigin();
}

void Slider::setCharacterSize (unsigned characterSize) {
    annotation.setCharacterSize(characterSize);
    annotation.centerOrigin();
}

void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(holder, states);
    if (filled.getWidth() >= radius * 2.f)
        target.draw(filled, states);
    if (hovered.getWidth() >= radius * 2.f)
        target.draw(hovered, states);
    target.draw(annotation, states);
}

sf::FloatRect Slider::getBoundary() const {
    return holder.getGlobalBounds();
}

void Slider::updateSliderValue() {
    filled.setWidth(hovered.getSize().x);
    float newValue = filled.getSize().x / holder.getSize().x;
    changeValue(newValue);
    setString(floatToPercentage(newValue));
}

void Slider::handleMousePress(const sf::Vector2f &mousePos) {
    if (!containPosition(mousePos)) return;
    updateSliderValue();
    followMouse = true;
}

void Slider::handleMouseRelease(const sf::Vector2f &mousePos) {
    if (!followMouse) return;
    updateSliderValue();
    followMouse = false;
}

void Slider::handleMouseMovement(const sf::Vector2f &mousePos) {
    if (followMouse or containPosition(mousePos)) {
        float hoverLength = this->getInverseTransform().transformPoint(mousePos).x;
        hoverLength = std::max(0.f, hoverLength);
        hoverLength = std::min(holder.getSize().x, hoverLength);
        hovered.setWidth(hoverLength);
        if (followMouse) updateSliderValue();
    }
    else hovered.setWidth(0.f);
}

void Slider::handleTextEntered(const char &unicode) {}

}; // namespace UI