#include "../../include/components/highlight-circle.hpp"

const float HIGHLIGHT_SMALL_DISTANCE = 1e-3;

namespace UI {

HighlightCircle::HighlightCircle() :
    circle(36.f, 100), chasing(false), nodeObserver(nullptr) {
        circle.setOutlineColor(Theme::getFixedColor());
        circle.setFillColor(Theme::getTransparent());
        circle.setOrigin({36.f, 36.f});
        circle.setOutlineThickness(6.f);
    }

void HighlightCircle::setTargetNode (AnimatedNode* ptr) {
    nodeObserver = ptr;
}

void HighlightCircle::timePropagation (float deltaTime) {
    if (nodeObserver == nullptr) return;
    setPosition(nodeObserver->getPosition());
    
    // if (chasing) // directly jump to target node
    //     setPosition(nodeObserver->getPosition());
    // else {
    //     sf::Vector2f displacement = nodeObserver->getPosition() - getPosition();
    //     if (magnitude(displacement) < HIGHLIGHT_SMALL_DISTANCE) {
    //         setPosition(nodeObserver->getPosition()), chasing = true;
    //         std::cerr << "Locked in" << std::endl;
    //     }
    //     else setPosition(getPosition() + displacement * Setting::animationFactor * deltaTime * 5.f);
    // }
}

void HighlightCircle::free() {
    nodeObserver = nullptr, chasing = false;
}

const AnimatedNode* HighlightCircle::getAddress() const {
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

}; // namespace UI