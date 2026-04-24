#include "../../include/components/ui-base.hpp"

namespace UI {

// ========== UI BASE ==========
bool Base::containPosition(const sf::Vector2f &position) const {
    sf::Vector2f localPos = this->getInverseTransform().transformPoint(position);
    return getBoundary().contains(localPos);
}

void Base::handleMouseEvents(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    // event: mouse moved
    if (const auto *mouseMovement = event->getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mousePos = window.mapPixelToCoords(mouseMovement->position);
        handleMouseMovement(mousePos);
    }
    // event: left-mouse pressed
    if (const auto *mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);
            handleMousePress(mousePos);
        }
    }
    // event: left-mouse released 
    if (const auto *mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(mouseReleased->position);
            handleMouseRelease(mousePos);
        }
    }
}

void Base::handleTextEvents(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    if (const auto *textEntered = event->getIf<sf::Event::TextEntered>()) {
        handleTextEntered(textEntered->unicode);
    }
}

// ========== TEXT ==========
void Text::centerOrigin() {
    auto localRectangle = getLocalBounds();
    setOrigin({
        localRectangle.position.x + localRectangle.size.x / 2.f,
        localRectangle.position.y + localRectangle.size.y / 2.f
    });
}

float Text::getWidth() const {
    auto localRectangle = getLocalBounds();
    return localRectangle.size.x;
}

void Text::setAutoCharacterSize(float boxW, float boxH, float textFill) {
    setCharacterSize(100); // set a reference size
    auto localRectangle = getLocalBounds();

    float targetW = boxW * textFill;
    float scaleW = targetW / localRectangle.size.x;
    float targetH = boxH * textFill;
    float scaleH = targetH / localRectangle.size.y;

    float scale = std::min(scaleW, scaleH);
    float fontSize = std::max(1.f, 100.f * scale);
    setCharacterSize(static_cast<unsigned int>(fontSize));
}

// ========== ROUNDED RECTANGLE ==========
RoundedRectangle::RoundedRectangle(float width, float height, float radius, unsigned int cornerPointCount)
    : sizeVector(width, height), radius(radius), cornerPointCount(cornerPointCount) {
    update();
}

void RoundedRectangle::setProperties(float width, float height, float radius) {
    this->sizeVector = sf::Vector2f(width, height);
    this->radius = radius;
    update();
}

sf::Vector2f RoundedRectangle::getSize() const {
    return sizeVector;
}

std::size_t RoundedRectangle::getPointCount() const {
    return cornerPointCount * 4;
}

void RoundedRectangle::centerOrigin() {
    setOrigin(sizeVector / 2.f);
}

sf::Vector2f RoundedRectangle::getPoint(std::size_t index) const {
    if (index >= getPointCount()) return {0, 0};
    size_t cornerIndex = index / cornerPointCount;

    float deltaAngle = (cornerPointCount > 1) ? 90.f / (cornerPointCount - 1) : 0.f;
    float startAngle = 270.f + (cornerIndex * 90.f);
    float angle = startAngle + (index % cornerPointCount) * deltaAngle;
    float radians = angle * std::acos(-1) / 180.f;

    sf::Vector2f center;
    switch (cornerIndex) {
        case 0: center = { sizeVector.x - radius, radius }; break;                 // TR
        case 1: center = { sizeVector.x - radius, sizeVector.y - radius }; break;  // BR
        case 2: center = { radius, sizeVector.y - radius }; break;                 // BL
        case 3: center = { radius, radius }; break;                                // TL
    }

    return { center.x + radius * std::cos(radians), center.y + radius * std::sin(radians) };
}

}; // namespace UI