#include "../../include/components/slider.hpp"

const sf::Color holderColor = sf::Color::White;
const sf::Color hoveredColor = sf::Color({0, 0, 0, 55});

namespace UI {

Slider::Slider(float width, float height) :
    annotation(Theme::ibmRegular), 
    label(Theme::notoCondensed), 
    displayValue(floatToPercentage),
    followMouse(false) {
    
    // setup holder bar
    holder.setSize({width, height});
    holder.setOutlineColor(Theme::getPrimary());
    holder.setOutlineThickness(2.f);
    holder.setFillColor(holderColor);

    // setup hover bar
    hovered.setSize({0.f, height});
    hovered.setFillColor(hoveredColor);

    // setup filled bar
    filled.setSize({width / 2.f, height});
    filled.setFillColor(Theme::getPressedButton());

    // setup annotation
    annotation.setFillColor(Theme::getPrimary());
    annotation.setAutoCharacterSize(width, height);
    annotation.setPosition({width / 2.f, height / 2.f});
    setString("50%");
    
    // center origin
    setOrigin({width / 2.f, height / 2.f});
}

void Slider::setString(const std::string &msg) {
    annotation.setString(msg);
    annotation.setAutoCharacterSize(
        holder.getSize().x,
        holder.getSize().y
    );
    annotation.centerOrigin();
}

void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(holder, states);
    target.draw(filled, states);
    target.draw(hovered, states);
    target.draw(annotation, states);
    target.draw(label, states);
}

sf::FloatRect Slider::getBoundary() const {
    return holder.getGlobalBounds();
}

void Slider::updateSliderValue() {
    filled.setSize(hovered.getSize());
    float newValue = filled.getSize().x / holder.getSize().x;
    changeValue(newValue);
    setString(displayValue(newValue));
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
        hovered.setSize({hoverLength, holder.getSize().y});
        if (followMouse) updateSliderValue();
    }
    else hovered.setSize({0.f, holder.getSize().y});
}

void Slider::handleTextEntered(const char &unicode) {}

}; // namespace UI