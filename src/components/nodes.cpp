#include "../../include/components/nodes.hpp"

namespace UI {

// ========== NODE ==========
Node::Node(const std::string &msg, float radius, float thickness) :
    circle(radius, 100), label(Theme::googleSansRegular, msg), annotation(Theme::googleSansItalic) {
    
    // setup circle
    circle.setOrigin({radius, radius});
    circle.setFillColor(Theme::getButton());
    circle.setOutlineColor(Theme::getTextPrimary());
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
        randFloat(0, Setting::focusX),
        randFloat(0, Setting::focusY)
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

void Node::setString (const std::string &s) {
    label.setString(s);
    float squareSize = std::sqrt(2 * getRadius() * getRadius());
    label.setAutoCharacterSize(squareSize, squareSize);
    label.centerOrigin();
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

void AnimatedNode::setColor (const sf::Color &color) {
    nodeUI.setColor(color);
}

sf::Vector2f AnimatedNode::getTargetPosition() const {
    return targetPosition;
}

void AnimatedNode::timePropagation(float deltaTime) {
    sf::Vector2f displacement = targetPosition - nodeUI.getPosition();
    if (magnitude(displacement) < eps) nodeUI.setPosition(targetPosition);
    else {
        sf::Vector2f newPosition = nodeUI.getPosition() + displacement * Setting::animationFactor() * deltaTime;
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

// void AnimatedNode::highlightNode() {
//     nodeUI.setColor(Theme::getFixedColor());
// }

// void AnimatedNode::unHighlightNode() {
//     nodeUI.setColor(Theme::getTransparent());
// }

sf::Vector2f AnimatedNode::getPosition() const {
    return nodeUI.getPosition();
}

void AnimatedNode::setPosition (const sf::Vector2f vec) {
    nodeUI.setPosition(vec);
}

void AnimatedNode::randomPosition() {
    nodeUI.randomPosition();
}

void AnimatedNode::copyPosition (const AnimatedNode &other) {
    setPosition(other.getPosition());
}

void AnimatedNode::setString (const std::string &s) {
    nodeUI.setString(s);
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

void FloatingNode::activateNode() {
    isActivated = true;
    setColor(Theme::getAccentPrimary());
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

bool FloatingNode::needCallback() const {
    return isClicked && !isActivated;
}

bool FloatingNode::isClickedCheck() const {
    return isClicked;
}


void FloatingNode::handleMouseRelease (const sf::Vector2f &mousePos) {
    if (!isClicked || isActivated) return;
    setColor(containPosition(mousePos) ?
        Theme::getHoveredButton() : Theme::getButton()
    );
    setScale({1.f, 1.f});
    isClicked = false;
    // callbackOnClick();
}

void FloatingNode::handleMouseMovement (const sf::Vector2f &mousePos) {
    if (isClicked || isActivated) return;
    setColor(containPosition(mousePos) ?
        Theme::getHoveredButton() : Theme::getButton()
    );
}

// ========== HELPER FUNCTIONS ==========
void drawEdge(sf::RenderTarget &target, sf::RenderStates states, const AnimatedNode* from, const AnimatedNode* to, bool isDirected, float thickness) {
    if (from->getPosition() == to->getPosition()) return;

    // get centers of the endpoints
    sf::Vector2f fromCenter = from->getPosition();
    sf::Vector2f toCenter = to->getPosition();
    sf::Vector2f shiftFrom = (toCenter - fromCenter) * from->getRadius() / distance(fromCenter, toCenter);
    sf::Vector2f shiftTo = (fromCenter - toCenter) * to->getRadius() / distance(fromCenter, toCenter);

    // pre calculations
    sf::Vector2f start = fromCenter + shiftFrom;
    sf::Vector2f end = toCenter + shiftTo;
    sf::Vector2f delta = end - start;
    float dist = distance(start, end);
    float angle = std::atan2(delta.y, delta.x);

    // setup line
    sf::RectangleShape line({dist, thickness});
    line.setOrigin({0, thickness / 2.0f});
    line.setPosition(start);
    line.setRotation(sf::radians(angle));
    line.setFillColor(Theme::getTextPrimary());

        // draw arrow head
    if (isDirected) {
        float arrowSize = thickness * 3.0f;
        sf::ConvexShape arrowhead;
        arrowhead.setPointCount(3);
        arrowhead.setPoint(0, {0, 0});
        arrowhead.setPoint(1, {-arrowSize, -arrowSize * 0.8f});
        arrowhead.setPoint(2, {-arrowSize,  arrowSize * 0.8f});
        
        arrowhead.setPosition(end);
        arrowhead.setRotation(sf::radians(angle));
        arrowhead.setFillColor(Theme::getTextPrimary());
        target.draw(arrowhead, states);
    }

    // draw
    target.draw(line, states);
}

}; // namespace UI