#include "../../include/components/nodes.hpp"

const float eps = 1e-6;

namespace UI {

// Node Implementation
Node::Node(const std::string &msg, float radius, float thickness) :
    circle(radius, 100), label(Theme::ibmRegular, msg) {
    
    // setup circle
    circle.setOrigin({radius, radius});
    circle.setFillColor(Theme::getTransparent());
    circle.setOutlineColor(Theme::getPrimary());
    circle.setOutlineThickness(thickness);

    // setup annotation
    float squareSize = std::sqrt(2 * radius * radius);
    label.setAutoCharacterSize(squareSize, squareSize);
    label.setFillColor(sf::Color::Black);
    label.centerOrigin();
}

void Node::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply state transform
    states.transform *= getTransform();

    // show circle and annotation
    target.draw(circle, states);
    target.draw(label, states);
}

float Node::getRadius() const {
    return circle.getRadius();
}

sf::FloatRect Node::getBoundary() const {
    return circle.getGlobalBounds();
}

sf::FloatRect Node::getGlobalBounds() const {
    return getTransform().transformRect(circle.getGlobalBounds());
}

void Node::setColor (const sf::Color &color) {
    circle.setFillColor(color);
}

void Node::handleMousePress(const sf::Vector2f &mousePos) {}
void Node::handleMouseMovement(const sf::Vector2f &mousePos) {}
void Node::handleMouseRelease(const sf::Vector2f &mousePos) {}
void Node::handleTextEntered(const char &unicode) {}

// AnimatedNode Implementation
AnimatedNode::AnimatedNode(const std::string &msg, float radius, float thickness) :
    nodeUI(msg, radius, thickness), 
    targetPosition({randFloat(0, Setting::screenWidth), randFloat(0, Setting::screenHeight)}) {
    nodeUI.setPosition(targetPosition);
}

void AnimatedNode::setTargetPosition(float x, float y) {
    targetPosition = sf::Vector2f({x, y});
}

void AnimatedNode::setTargetX(float x) {
    targetPosition.x = x;
}

void AnimatedNode::setTargetY(float y) {
    targetPosition.y = y;
}

float AnimatedNode::getTargetX() const {
    return targetPosition.x;
}

float AnimatedNode::getTargetY() const {
    return targetPosition.y;
}

void AnimatedNode::timePropagation(float deltaTime) {
    sf::Vector2f displacement = targetPosition - nodeUI.getPosition();
    if (magnitude(displacement) < eps) nodeUI.setPosition(targetPosition);
    else {
        sf::Vector2f newPosition = nodeUI.getPosition() + displacement * Setting::animationFactor * deltaTime;
        nodeUI.setPosition(newPosition);
    }
}

sf::FloatRect AnimatedNode::getGlobalBounds() const {
    return nodeUI.getGlobalBounds();
}

sf::Vector2f AnimatedNode::getPosition() const {
    return nodeUI.getPosition();
}

float AnimatedNode::getRadius() const {
    return nodeUI.getRadius();
}

void swapAnimatedNode(AnimatedNode &a, AnimatedNode &b) {
    std::swap(a.nodeUI, b.nodeUI);
}

void AnimatedNode::highlightNode() {
    nodeUI.setColor(Theme::getFixedColor());
}

void AnimatedNode::unHighlightNode() {
    nodeUI.setColor(Theme::getTransparent());
}

void AnimatedNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    nodeUI.draw(target, states);
}


// Standalone Helper Implementation
void drawEdge(sf::RenderTarget &target, sf::RenderStates state, const AnimatedNode &from, const AnimatedNode &to, float thickness) {
    if (from.getPosition() == to.getPosition()) return;

    // get centers of the endpoints
    sf::Vector2f fromCenter = from.getPosition();
    sf::Vector2f toCenter = to.getPosition();
    sf::Vector2f shift = (toCenter - fromCenter) * from.getRadius() / distance(fromCenter, toCenter);

    // pre calculations
    sf::Vector2f start = fromCenter + shift;
    sf::Vector2f end = toCenter - shift;
    sf::Vector2f delta = end - start;
    float dist = distance(start, end);
    float angle = std::atan2(delta.y, delta.x);

    // setup line
    sf::RectangleShape line({dist, thickness});
    line.setOrigin({0, thickness / 2.0f});
    line.setPosition(start);
    line.setRotation(sf::radians(angle));
    line.setFillColor(sf::Color::Black);

    // draw
    target.draw(line, state);
}

}; // namespace UI