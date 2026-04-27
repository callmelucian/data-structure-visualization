#include "../../include/components/graph.hpp"

// ========== CONSTANTS INITIALIZATION ==========
const float REPULSIVE_CONSTANT = 100'000.f;
const float IDEAL_LENGTH = 100.f;
const float SPRING_CONSTANT = 0.05f;
const float FRICTION_CONSTANT = 0.9f;

namespace UI {

// ========== EDGE IMPLEMENTATION ==========
Edge::Edge() :
    fromID(0), toID(0), weight(0),
    isDeleted(false), isActivated(false), isHovered(false), isHighlighted(false), isDirected(false),
    defaultColor(Theme::getTextPrimary()),
    fromObserver(nullptr), toObserver(nullptr),
    thickness(0.f) {}

Edge::Edge (int fromNode, int toNode, int weight, Node* fromObserver, Node* toObserver, bool isDirected) :
    fromID(fromNode), toID(toNode), weight(weight),
    isDeleted(false), isActivated(false), isHovered(false), isHighlighted(false), isDirected(isDirected),
    defaultColor(Theme::getTextPrimary()),
    fromObserver(fromObserver), toObserver(toObserver),
    thickness(2.f) {}

void Edge::updateObserver (Node* newFrom, Node* newTo) {
    fromObserver = newFrom;
    toObserver = newTo;
}

void Edge::setDefaultColor (const sf::Color &color) {
    defaultColor = color;
}

sf::Vector2f Edge::getFromPosition() const {
    return fromObserver ? fromObserver->getPosition() : sf::Vector2f({0, 0});
}

sf::Vector2f Edge::getToPosition() const {
    return toObserver ? toObserver->getPosition() : sf::Vector2f({0, 0});
}

sf::Color Edge::getColor() const {
    if (isHighlighted || isActivated) return Theme::getAccentPrimary();
    if (isHovered) return Theme::getTextSecondary();
    return defaultColor;
}

void Edge::drawEdge (sf::RenderTarget &target, sf::RenderStates states, const sf::Vector2f &fromPos, const sf::Vector2f &toPos) const {
    // pre-calculations
    sf::Vector2f delta = toPos - fromPos;
    float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    float angle = std::atan2(delta.y, delta.x);

    // draw line
    sf::RectangleShape line({dist, thickness});
    line.setOrigin({0, thickness / 2.0f});
    line.setPosition(fromPos);
    line.setRotation(sf::radians(angle));
    line.setFillColor(getColor());
    target.draw(line, states);

    // draw arrow head
    if (isDirected) {
        float arrowSize = thickness * 3.0f;
        sf::ConvexShape arrowhead;
        arrowhead.setPointCount(3);
        arrowhead.setPoint(0, {0, 0});
        arrowhead.setPoint(1, {-arrowSize, -arrowSize * 0.8f});
        arrowhead.setPoint(2, {-arrowSize,  arrowSize * 0.8f});
        
        arrowhead.setPosition(toPos);
        arrowhead.setRotation(sf::radians(angle));
        arrowhead.setFillColor(getColor());
        target.draw(arrowhead, states);
    }
}

void Edge::draw (sf::RenderTarget &target, sf::RenderStates states) const {
    if (getFromPosition() == getToPosition() || isDeleted) return;
    float d = distance(getFromPosition(), getToPosition());
    sf::Vector2f shiftFrom = (getToPosition() - getFromPosition()) * fromObserver->getRadius() / d;
    sf::Vector2f shiftTo = (getFromPosition() - getToPosition()) * toObserver->getRadius() / d;
    drawEdge(target, states, getFromPosition() + shiftFrom, getToPosition() + shiftTo);

    sf::Vector2f delta = getToPosition() - getFromPosition();

    Text text(Theme::ibmRegular, std::to_string(weight), 15);
    text.setFillColor(getColor());
    text.centerOrigin();
    sf::Vector2f midpoint = (getFromPosition() + getToPosition()) / 2.0f;
    sf::Vector2f unitNormal(-delta.y / d, delta.x / d);
    float textOffset = 15.0f + thickness; 
    text.setPosition(midpoint + unitNormal * textOffset);

    target.draw(text, states);
}

sf::FloatRect Edge::getBoundary() const {
    return sf::FloatRect();
}

void Edge::handleMousePress (const sf::Vector2f &mousePos) {}
void Edge::handleMouseRelease (const sf::Vector2f &mousePos) {}
void Edge::handleMouseMovement (const sf::Vector2f &mousePos) {}
void Edge::handleTextEntered (const char &unicode) {}
    
// ========== GRAPH IMPLEMENTATION ==========
Graph::Graph() : targetOrigin({0, 0}), activatedNode(-1), isDirected(false), canInteract(true) {}

Graph::~Graph() {
    for (UI::FloatingNode* ptr : nodes) delete ptr;
}

Graph::Graph(const Graph &other) : UI::Base(other) {
    copyFrom(other);
}

Graph& Graph::operator=(const Graph &other) {
    if (this == &other) return *this;
    UI::Base::operator=(other);
    for (UI::FloatingNode* node : nodes) delete node;
    nodes.clear(), copyFrom(other);
    return *this;
}

void Graph::copyFrom (const Graph &other) {
    this->isDeleted = other.isDeleted;
    this->edges = other.edges;
    this->highlighter = other.highlighter;
    this->targetOrigin = other.targetOrigin;
    this->activatedNode = other.activatedNode;
    this->isDirected = other.isDirected;

    this->callbackAllowEdit = other.callbackAllowEdit;
    this->callbackAllowDelete = other.callbackAllowDelete;
    this->callbackIsEditing = other.callbackIsEditing;
    this->callbackTriggerAddEdge = other.callbackTriggerAddEdge;

    this->nodes.reserve(other.nodes.size());
    for (int i = 0; i < other.nodes.size(); i++) {
        FloatingNode* node = other.nodes[i];
        if (node != nullptr) {
            nodes.push_back(new UI::FloatingNode(*node));
            // nodes.back()->setCallbackOnClick([this, i]() {
            //     std::cerr << "what " << activatedNode << std::endl;
            //     if (activatedNode == -1) activatedNode = i;
            //     else {
            //         callbackTriggerAddEdge(activatedNode, i);
            //         activatedNode = -1;
            //     }
            // });
            if (highlighter.getAddress() == node)
                highlighter.setTargetNode(nodes.back());
        }
        else nodes.push_back(nullptr);
    }

    for (int i = 0; i < other.nodes.size(); i++)
        assert(other.nodes[i]->isClickedCheck() == nodes[i]->isClickedCheck());

    for (Edge &curr : edges)
        curr.updateObserver(this->nodes[curr.fromID], this->nodes[curr.toID]);
}

void Graph::timePropagation (float deltaTime, float maxWidth, float maxHeight) {
    // apply repulsive force
    for (int i = 0; i < nodes.size(); i++) {
        if (isDeleted[i]) continue;
        FloatingNode* ptr = nodes[i];
        sf::Vector2f currentPosition = ptr->getPosition();
        ptr->resetAcceleration();
        for (UI::FloatingNode* otherPtr : nodes) {
            if (ptr == otherPtr) continue;
            sf::Vector2f otherPosition = otherPtr->getPosition();
            sf::Vector2f repulsiveForce = unitVector(otherPosition, currentPosition) * REPULSIVE_CONSTANT / std::max(0.001f, squaredDistance(currentPosition, otherPosition));
            ptr->addAcceleration(repulsiveForce);
        }
    }
    
    // apply spring force
    for (Edge item : edges) {
        if (item.isDeleted) continue;
        sf::Vector2f fromPosition = item.getFromPosition();
        sf::Vector2f toPosition = item.getToPosition();
        sf::Vector2f springForce = unitVector(fromPosition, toPosition) * SPRING_CONSTANT * (distance(fromPosition, toPosition) - IDEAL_LENGTH);
        nodes[item.fromID]->addAcceleration(springForce);
        nodes[item.toID]->addAcceleration(-springForce);
    }
    
    // move nodes
    for (int i = 0; i < nodes.size(); i++) {
        if (isDeleted[i]) continue;
        FloatingNode* ptr = nodes[i];
        ptr->applyDamping(FRICTION_CONSTANT);
        ptr->timePropagation(deltaTime);
        ptr->clampPosition(getOrigin(), maxWidth, maxHeight);
    }
    highlighter.timePropagation(deltaTime);

    // re-align origin
    if (getOrigin() == targetOrigin) {
        autosetTargetOrigin();
        setOrigin(targetOrigin);
    }
    else {
        autosetTargetOrigin();
        sf::Vector2f displacement = targetOrigin - getOrigin();
        if (magnitude(displacement) < eps) setOrigin(targetOrigin);
        else {
            sf::Vector2f newPosition = getOrigin() + displacement * Setting::animationFactor() * deltaTime;
            setOrigin(newPosition);
        }
    }
}

void Graph::makeDirected() {
    isDirected = true;
}

void Graph::insertNode (int label) {
    int curr = nodes.size();
    nodes.push_back(new FloatingNode(std::to_string(label)));
    isDeleted.push_back(false);
    autosetTargetOrigin();
}

void Graph::insertEdge (int fromNode, int toNode, int weight) {
    edges.emplace_back(fromNode, toNode, weight, nodes[fromNode], nodes[toNode], isDirected);
}

void Graph::setEdgeColor (int edgeID, const sf::Color &color) {
    edges[edgeID].setDefaultColor(color);
}

void Graph::setTargetOrigin (const sf::Vector2f &pos) {
    targetOrigin = pos;
}

void Graph::autosetTargetOrigin() {
    float positionXMin = 0.f, positionXMax = 0.f, positionYMin = 0.f, positionYMax = 0.f;
    for (int i = 0; i < nodes.size(); i++) {
        if (isDeleted[i]) continue;
        positionXMin = positionXMax = nodes[i]->getPosition().x;
        positionYMin = positionYMax = nodes[i]->getPosition().y;
    }
    for (int i = 0; i < nodes.size(); i++) {
        if (isDeleted[i]) continue;
        FloatingNode* ptr = nodes[i];
        positionXMin = std::min(positionXMin, ptr->getPosition().x);
        positionXMax = std::max(positionXMax, ptr->getPosition().x);
        positionYMin = std::min(positionYMin, ptr->getPosition().y);
        positionYMax = std::max(positionYMax, ptr->getPosition().y);
    }
    float originX = (positionXMin + positionXMax) / 2.f;
    float originY = (positionYMin + positionYMax) / 2.f;
    targetOrigin = {originX, originY};
}

void Graph::changeWeight (int edgeID, int newWeight) {
    edges[edgeID].weight = newWeight;
    edges[edgeID].isActivated = edges[edgeID].isHovered = false;
    callbackAllowEdit(false);
    callbackAllowDelete(false);
}

void Graph::deleteEdge (int edgeID) {
    edges[edgeID].isDeleted = true;
    edges[edgeID].isActivated = edges[edgeID].isHovered = false;
    callbackAllowEdit(false);
    callbackAllowDelete(false);
}

void Graph::deleteNode (int nodeID) {
    isDeleted[nodeID] = true;
    for (Edge &curr : edges) {
        if (curr.fromID != nodeID && curr.toID != nodeID) continue;
        curr.isDeleted = true, curr.isActivated = curr.isHovered = false;
    }
    callbackAllowDelete(false);
}

sf::FloatRect Graph::getBoundary() const {
    return sf::FloatRect();
}

int Graph::nodeActivated() {
    if (isDeleted[activatedNode]) activatedNode = -1;
    return activatedNode;
}

int Graph::edgeActivated() {
    for (int i = 0; i < edges.size(); i++)
        if (!edges[i].isDeleted && edges[i].isActivated) return i;
    return -1;
}

void Graph::resetNodeActivation() {
    activatedNode = -1;
}

void Graph::resetEdgeActivation() {
    for (Edge &curr : edges)
        curr.isActivated = false;
}

void Graph::clearAnnotation() {
    for (FloatingNode* ptr : nodes) {
        ptr->setAnnotation("");
        ptr->setAnnotationColor(Theme::getAccentDark());
    }
}

void Graph::markAnnotation (int nodeID) {
    nodes[nodeID]->setAnnotationColor(Theme::getTextPrimary());
}

void Graph::highlightNode (int nodeID) {
    if (nodeID == -1) return highlighter.free(), void();
    highlighter.setTargetNode(nodes[nodeID]);
}

void Graph::highlightEdge (int edgeID) {
    for (Edge &curr : edges) curr.isHighlighted = false;
    if (edgeID != -1) edges[edgeID].isHighlighted = true;
}

void Graph::copyPosition (const Graph &other) {
    for (int i = 0; i < nodes.size(); i++) {
        if (isDeleted[i]) continue;
        if (i < other.nodes.size() && !other.isDeleted[i])
            nodes[i]->copyPosition(*other.nodes[i]);
        else nodes[i]->randomPosition();
    }
    setOrigin(other.getOrigin());
}

void Graph::setAnnotation (int nodeID, int value) {
    std::string msg = (value == INT_MAX ? "INF" : std::to_string(value));
    if (nodeID < nodes.size()) nodes[nodeID]->setAnnotation(msg);
}

void Graph::calculatePositions (float maxWidth, float maxHeight) {}

void Graph::enableInteractions() {
    canInteract = true;
}

void Graph::disableInteractions() {
    canInteract = false;
}

void Graph::handleMousePress (const sf::Vector2f &mousePos) {
    if (!canInteract) return;
    if (callbackIsEditing(mousePos)) return;
    sf::Vector2f localPos = this->getInverseTransform().transformPoint(mousePos);
    bool clicked = false;
    for (int i = 0; i < nodes.size(); i++) {
        if (isDeleted[i]) continue;
        FloatingNode* ptr = nodes[i];
        ptr->handleMousePress(localPos);
        if (ptr->containPosition(localPos)) clicked = true;
    }
    if (!clicked && activatedNode != -1) {
        nodes[activatedNode]->deactivateNode();
        activatedNode = -1;
    }
    
    for (Edge &curr : edges) curr.isActivated = false;
    if (!clicked) {
        for (int edgeID = (int)edges.size() - 1; edgeID >= 0; edgeID--) {
            Edge &curr = edges[edgeID];
            if (curr.isDeleted) continue;
            sf::Vector2f fromPos = curr.getFromPosition();
            sf::Vector2f toPos = curr.getToPosition();
            if (isPointOnSegment(fromPos, toPos, localPos)) {
                curr.isActivated = true;
                callbackAllowEdit(true), callbackAllowDelete(true);
                return;
            }
        }
    }
    callbackAllowEdit(false), callbackAllowDelete(false);
}

void Graph::handleMouseRelease (const sf::Vector2f &mousePos) {
    if (!canInteract) return;
    sf::Vector2f localPos = this->getInverseTransform().transformPoint(mousePos);
    bool callback = false;
    int a, b;
    for (int i = 0; i < nodes.size(); i++) {
        if (isDeleted[i]) continue;
        FloatingNode* ptr = nodes[i];
        if (ptr->needCallback()) {
            if (activatedNode == -1) activatedNode = i;
            else {
                callback = true, a = activatedNode, b = i;
                activatedNode = -1;
            }
        }
        ptr->handleMouseRelease(localPos);
        ptr->deactivateNode();
    }
    if (activatedNode != -1) {
        nodes[activatedNode]->activateNode();
        callbackAllowDelete(true);
    }
    if (callback) callbackTriggerAddEdge(a, b);
}

void Graph::handleMouseMovement (const sf::Vector2f &mousePos) {
    if (!canInteract) return;
    bool hovered = false;
    sf::Vector2f localPos = this->getInverseTransform().transformPoint(mousePos);
    for (UI::FloatingNode* ptr : nodes) {
        ptr->handleMouseMovement(localPos);
        if (ptr->containPosition(localPos)) hovered = true;
    }
    
    for (Edge &curr : edges) curr.isHovered = false;
    if (hovered) return;
    for (int edgeID = (int)edges.size() - 1; edgeID >= 0; edgeID--) {
        Edge &curr = edges[edgeID];
        if (curr.isDeleted) continue;
        sf::Vector2f fromPos = curr.getFromPosition();
        sf::Vector2f toPos = curr.getToPosition();
        if (isPointOnSegment(fromPos, toPos, localPos)) {
            curr.isHovered = true;
            return;
        }
    }
}

void Graph::handleTextEntered (const char &unicode) {}

void Graph::draw (sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (Edge item : edges) target.draw(item, states);
    for (int i = 0; i < nodes.size(); i++)
        if (!isDeleted[i]) target.draw(*nodes[i], states);
    target.draw(highlighter, states);
}

}; // namespace UI