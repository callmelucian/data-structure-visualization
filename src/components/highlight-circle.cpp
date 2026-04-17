#include "../../include/components/highlight-circle.hpp"

const float HIGHLIGHT_SMALL_DISTANCE = 1e-3;

namespace UI {

template <typename NodeType>
HighlightCircle<NodeType>::HighlightCircle() :
    circle(36.f, 100), highlightLocked(true), nodeObserver(nullptr) {
        circle.setOutlineColor(Theme::getFixedColor());
        circle.setFillColor(Theme::getTransparent());
        circle.setOrigin({36.f, 36.f});
        circle.setOutlineThickness(6.f);
    }

template <typename NodeType>
void HighlightCircle<NodeType>::setTargetNode (NodeType* ptr) {
    nodeObserver = ptr;
    // highlightLocked = false;
}

template <typename NodeType>
void HighlightCircle<NodeType>::timePropagation (float deltaTime) {
    if (nodeObserver == nullptr) return;
    if (highlightLocked) setPosition(nodeObserver->getPosition());
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

template <typename NodeType>
void HighlightCircle<NodeType>::fastForward() {
    if (nodeObserver) setPosition(nodeObserver->getPosition());
}

template <typename NodeType>
void HighlightCircle<NodeType>::free() {
    nodeObserver = nullptr, highlightLocked = true;
}

template <typename NodeType>
const NodeType* HighlightCircle<NodeType>::getAddress() const {
    return nodeObserver;
}

template <typename NodeType>
sf::FloatRect HighlightCircle<NodeType>::getBoundary() const {
    return circle.getLocalBounds();
}

template <typename NodeType>
void HighlightCircle<NodeType>::draw (sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    if (nodeObserver != nullptr) target.draw(circle, states);
}

template <typename NodeType>
void HighlightCircle<NodeType>::handleMousePress (const sf::Vector2f &mousePos) {}
template <typename NodeType>
void HighlightCircle<NodeType>::handleMouseRelease (const sf::Vector2f &mousePos) {}
template <typename NodeType>
void HighlightCircle<NodeType>::handleMouseMovement (const sf::Vector2f &mousePos) {}
template <typename NodeType>
void HighlightCircle<NodeType>::handleTextEntered (const char &unicode) {}

template <typename NodeType>
void HighlightCircle<NodeType>::lockHighlight() {
    highlightLocked = true;
}

}; // namespace UI