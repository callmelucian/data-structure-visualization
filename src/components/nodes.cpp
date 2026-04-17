#include "../../include/components/nodes.hpp"

const float eps = 1e-6;

namespace UI {

// ========== NODE ==========
Node::Node(const std::string &msg, float radius, float thickness) :
    circle(radius, 100), label(Theme::ibmRegular, msg), annotation(Theme::ibmRegular) {
    
    // setup circle
    circle.setOrigin({radius, radius});
    circle.setFillColor(Theme::getTransparent());
    circle.setOutlineColor(Theme::getPrimary());
    circle.setOutlineThickness(thickness);

    // setup label
    float squareSize = std::sqrt(2 * radius * radius);
    label.setAutoCharacterSize(squareSize, squareSize);
    label.setFillColor(sf::Color::Black);
    label.centerOrigin();

    // setup annotation
    annotation.setCharacterSize(20);
    annotation.setFillColor(sf::Color::Red);
    annotation.setPosition({0, -radius - 15.f});
}

void Node::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply state transform
    states.transform *= getTransform();

    // show circle and annotation
    target.draw(circle, states);
    target.draw(label, states);
    target.draw(annotation, states);
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

void Node::setAnnotationColor (const sf::Color &color) {
    annotation.setFillColor(color);
}

void Node::randomPosition() {
    setPosition({
        randFloat(0, Setting::screenWidth),
        randFloat(0, Setting::screenHeight)
    });
}

void Node::clampPosition (const sf::Vector2f &pos, float maxWidth, float maxHeight) {
    float x = getPosition().x, y = getPosition().y;
    x = std::max(x, pos.x - maxWidth / 2.f + getRadius());
    x = std::min(x, pos.x + maxWidth / 2.f - getRadius());
    y = std::max(y, pos.y - maxHeight / 2.f + getRadius());
    y = std::min(y, pos.y + maxHeight / 2.f - getRadius());
    setPosition({x, y});
}

void Node::setAnnotation (const std::string &msg) {
    annotation.setString(msg);
    annotation.centerOrigin();
}

void Node::handleMousePress(const sf::Vector2f &mousePos) {}
void Node::handleMouseMovement(const sf::Vector2f &mousePos) {}
void Node::handleMouseRelease(const sf::Vector2f &mousePos) {}
void Node::handleTextEntered(const char &unicode) {}

// ========== ANIMATED NODE ==========
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


sf::Vector2f AnimatedNode::getTargetPosition() const {
    return targetPosition;
}

void AnimatedNode::timePropagation(float deltaTime) {
    sf::Vector2f displacement = targetPosition - nodeUI.getPosition();
    if (magnitude(displacement) < eps) nodeUI.setPosition(targetPosition);
    else {
        sf::Vector2f newPosition = nodeUI.getPosition() + displacement * Setting::animationFactor * deltaTime;
        nodeUI.setPosition(newPosition);
    }
}

void AnimatedNode::fastForward() {
    nodeUI.setPosition(targetPosition);
}

sf::FloatRect AnimatedNode::getGlobalBounds() const {
    return nodeUI.getGlobalBounds();
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

sf::Vector2f AnimatedNode::getPosition() const {
    return nodeUI.getPosition();
}

void AnimatedNode::setPosition (const sf::Vector2f vec) {
    nodeUI.setPosition(vec);
}

void AnimatedNode::randomPosition() {
    nodeUI.randomPosition();
}

void AnimatedNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    nodeUI.draw(target, states);
}

// ========== FLOATING NODE ==========
FloatingNode::FloatingNode(const std::string &msg, float radius, float thickness) : Node(msg, radius, thickness), isClicked(false), isActivated(false) {
    randomPosition();
}

void FloatingNode::applyAcceleration (sf::Vector2f targetAcceleration) {
    acceleration = targetAcceleration;
}

void FloatingNode::addAcceleration (sf::Vector2f addedAcceleration) {
    acceleration += addedAcceleration;
}

void FloatingNode::resetAcceleration() {
    acceleration = {0, 0};
}

void FloatingNode::applyDamping (float coefficient) {
    velocity *= coefficient;
}

void FloatingNode::timePropagation (float deltaTime) {
    velocity += acceleration * deltaTime;
    setPosition(getPosition() + velocity * deltaTime * 50.f);
}

// sf::Vector2f FloatingNode::getPosition() const {
//     return nodeUI.getPosition();
// }

// float FloatingNode::getRadius() const {
//     return nodeUI.getRadius();
// }

// void FloatingNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
//     target.draw(nodeUI, states);
// }

void FloatingNode::activateNode() {
    isActivated = true;
    setColor(sf::Color::Green);
}

void FloatingNode::deactivateNode() {
    isActivated = false;
    setColor(Theme::getButton());
}

void FloatingNode::copyPosition (const FloatingNode &other) {
    setPosition(other.getPosition());
    velocity = other.velocity;
    acceleration = other.acceleration;
}

void FloatingNode::handleMousePress (const sf::Vector2f &mousePos) {
    if (!containPosition(mousePos) || isActivated) return;
    setColor(Theme::getPressedButton());
    setScale({0.97f, 0.97f});
    isClicked = true;
}

void FloatingNode::handleMouseRelease (const sf::Vector2f &mousePos) {
    if (!isClicked || isActivated) return;
    setColor(containPosition(mousePos) ?
        Theme::getHoveredButton() : Theme::getButton()
    );
    setScale({1.f, 1.f});
    isClicked = false;
    callbackOnClick();
}

void FloatingNode::handleMouseMovement (const sf::Vector2f &mousePos) {
    if (isClicked || isActivated) return;
    setColor(containPosition(mousePos) ?
        Theme::getHoveredButton() : Theme::getButton()
    );
}

// ========== HELPER FUNCTIONS ==========
void drawEdge(sf::RenderTarget &target, sf::RenderStates state, const AnimatedNode* from, const AnimatedNode* to, float thickness) {
    if (from->getPosition() == to->getPosition()) return;

    // get centers of the endpoints
    sf::Vector2f fromCenter = from->getPosition();
    sf::Vector2f toCenter = to->getPosition();
    sf::Vector2f shift = (toCenter - fromCenter) * from->getRadius() / distance(fromCenter, toCenter);

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

// void drawEdge(sf::RenderTarget &target, sf::RenderStates state, const FloatingNode* from, const FloatingNode* to, sf::Color edgeColor, float thickness) {
//     if (from->getPosition() == to->getPosition()) return;

//     // get centers of the endpoints
//     sf::Vector2f fromCenter = from->getPosition();
//     sf::Vector2f toCenter = to->getPosition();
//     sf::Vector2f shift = (toCenter - fromCenter) * from->getRadius() / distance(fromCenter, toCenter);

//     // pre calculations
//     sf::Vector2f start = fromCenter + shift;
//     sf::Vector2f end = toCenter - shift;
//     sf::Vector2f delta = end - start;
//     float dist = distance(start, end);
//     float angle = std::atan2(delta.y, delta.x);

//     // setup line
//     sf::RectangleShape line({dist, thickness});
//     line.setOrigin({0, thickness / 2.0f});
//     line.setPosition(start);
//     line.setRotation(sf::radians(angle));
//     line.setFillColor(edgeColor);

//     // draw
//     target.draw(line, state);
// }

void drawEdge(sf::RenderTarget &target, sf::RenderStates state, 
              const FloatingNode* from, const FloatingNode* to,
              int weight, sf::Color edgeColor, float thickness) {
    
    if (from->getPosition() == to->getPosition()) return;

    sf::Vector2f fromCenter = from->getPosition();
    sf::Vector2f toCenter = to->getPosition();
    float d = distance(fromCenter, toCenter);
    
    sf::Vector2f shift = (toCenter - fromCenter) * from->getRadius() / d;
    sf::Vector2f start = fromCenter + shift;
    sf::Vector2f end = toCenter - shift;
    
    sf::Vector2f delta = end - start;
    float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    float angle = std::atan2(delta.y, delta.x);

    sf::RectangleShape line({dist, thickness});
    line.setOrigin({0, thickness / 2.0f});
    line.setPosition(start);
    line.setRotation(sf::radians(angle));
    line.setFillColor(edgeColor);
    target.draw(line, state);

    float arrowSize = thickness * 3.0f;
    sf::ConvexShape arrowhead;
    arrowhead.setPointCount(3);
    arrowhead.setPoint(0, {0, 0});                          // Tip of arrow
    arrowhead.setPoint(1, {-arrowSize, -arrowSize * 0.8f}); // Back top
    arrowhead.setPoint(2, {-arrowSize,  arrowSize * 0.8f}); // Back bottom
    
    arrowhead.setPosition(end);
    arrowhead.setRotation(sf::radians(angle));
    arrowhead.setFillColor(edgeColor);
    target.draw(arrowhead, state);

    Text text(Theme::ibmRegular, std::to_string(weight), 15);
    text.setFillColor(edgeColor);
    text.centerOrigin();

    // position text at midpoint
    sf::Vector2f midpoint = (start + end) / 2.0f;
    sf::Vector2f unitNormal(-delta.y / dist, delta.x / dist);
    float textOffset = 15.0f + thickness; 
    text.setPosition(midpoint + unitNormal * textOffset);

    target.draw(text, state);
}

}; // namespace UI