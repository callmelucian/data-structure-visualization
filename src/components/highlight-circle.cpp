#include "../../include/components/highlight-circle.hpp"

const float HIGHLIGHT_SMALL_DISTANCE = 1e-3;

namespace UI {

HighlightCircle::HighlightCircle() :
    circle(36.f, 100), highlightLocked(true), nodeObserver(nullptr) {
        circle.setOutlineColor(Theme::getFixedColor());
        circle.setFillColor(Theme::getTransparent());
        circle.setOrigin({36.f, 36.f});
        circle.setOutlineThickness(6.f);
    }

void HighlightCircle::setTargetNode (Node* ptr) {
    nodeObserver = ptr;
    // highlightLocked = false;
}

void HighlightCircle::timePropagation (float deltaTime) {
    if (nodeObserver == nullptr) return;
    if (highlightLocked)
        setPosition(nodeObserver->getPosition());
    else {
        sf::Vector2f displacement = nodeObserver->getPosition() - getPosition();
        if (magnitude(displacement) < HIGHLIGHT_SMALL_DISTANCE)
            setPosition(nodeObserver->getPosition());
        else {
            sf::Vector2f dVelo = displacement * Setting::animationFactor * deltaTime * 5.f;
            setPosition(getPosition() + dVelo);
        }
    }
}

void HighlightCircle::fastForward() {
    if (nodeObserver)
        setPosition(nodeObserver->getPosition());
}

void HighlightCircle::free() {
    nodeObserver = nullptr, highlightLocked = true;
}

const Node* HighlightCircle::getAddress() const {
    return nodeObserver;
}

sf::FloatRect HighlightCircle::getBoundary() const {
    return circle.getLocalBounds();
}

void HighlightCircle::draw (sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    if (nodeObserver != nullptr) target.draw(circle, states);
}

void HighlightCircle::handleMousePress (const sf::Vector2f &mousePos) {}
void HighlightCircle::handleMouseRelease (const sf::Vector2f &mousePos) {}
void HighlightCircle::handleMouseMovement (const sf::Vector2f &mousePos) {}
void HighlightCircle::handleTextEntered (const char &unicode) {}

void HighlightCircle::lockHighlight() {
    highlightLocked = true;
}

}; // namespace UI