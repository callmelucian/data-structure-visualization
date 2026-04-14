#include "../../include/components/graph.hpp"

// ========== CONSTANTS INITIALIZATION ==========
const float REPULSIVE_CONSTANT = 100'000.f;
const float IDEAL_LENGTH = 100.f;
const float SPRING_CONSTANT = 0.05f;
const float FRICTION_CONSTANT = 0.9f;

// ========== GRAPH IMPLEMENTATION ==========
namespace UI {

Graph::Graph() : targetOrigin({0, 0}), activatedNode(-1) {}

Graph::~Graph() {
    for (UI::FloatingNode* ptr : nodes) delete ptr;
}

Graph::Graph(const Graph &other) : UI::Base(other), edges(other.edges), highlighter(other.highlighter), targetOrigin(other.targetOrigin), activatedNode(activatedNode) {
    nodes.reserve(other.nodes.size());
    for (UI::FloatingNode* node : other.nodes) {
        if (node != nullptr) nodes.push_back(new UI::FloatingNode(*node));
        else nodes.push_back(nullptr);
    }
    this->callbackAllowEdit = other.callbackAllowEdit;
}

Graph& Graph::operator=(const Graph &other) {
    if (this == &other) return *this;
    UI::Base::operator=(other);

    for (UI::FloatingNode* node : nodes) delete node;
    nodes.clear();

    this->edges = other.edges;
    this->highlighter = other.highlighter;
    this->targetOrigin = other.targetOrigin;
    this->activatedNode = other.activatedNode;
    this->callbackAllowEdit = other.callbackAllowEdit;

    this->nodes.reserve(other.nodes.size());
    for (UI::FloatingNode* node : other.nodes) {
        if (node != nullptr) this->nodes.push_back(new UI::FloatingNode(*node));
        else this->nodes.push_back(nullptr);
    }
    return *this;
}

void Graph::timePropagation (float deltaTime) {
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
        sf::Vector2f fromPosition = nodes[item.fromNode]->getPosition();
        sf::Vector2f toPosition = nodes[item.toNode]->getPosition();
        sf::Vector2f springForce = unitVector(fromPosition, toPosition) * SPRING_CONSTANT * (distance(fromPosition, toPosition) - IDEAL_LENGTH);
        nodes[item.fromNode]->addAcceleration(springForce);
        nodes[item.toNode]->addAcceleration(-springForce);
    }

    // move nodes
    for (int i = 0; i < nodes.size(); i++) {
        if (isDeleted[i]) continue;
        FloatingNode* ptr = nodes[i];
        ptr->applyDamping(FRICTION_CONSTANT);
        ptr->timePropagation(deltaTime);
    }

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
            sf::Vector2f newPosition = getOrigin() + displacement * Setting::animationFactor * deltaTime;
            setOrigin(newPosition);
        }
    }
}

void Graph::insertNode (int label) {
    int curr = nodes.size();
    nodes.push_back(new FloatingNode(std::to_string(label)));
    nodes.back()->setCallbackOnClick([this, curr]() {
        if (activatedNode == -1) activatedNode = curr;
        else {
            insertEdge(activatedNode, curr);
            activatedNode = -1;
        }
    });
    isDeleted.push_back(false);
    autosetTargetOrigin();
}

void Graph::insertEdge (int fromNode, int toNode) {
    edges.emplace_back(fromNode, toNode, randInt(0, 99));
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

void Graph::changeWeight (int newWeight) {
    for (Edge &curr : edges) {
        if (curr.isActivated) curr.weight = newWeight;
        curr.isActivated = curr.isHovered = false;
    }
    callbackAllowEdit(false);
}

void Graph::deleteEdge() {
    for (Edge &curr : edges) {
        if (curr.isActivated) curr.isDeleted = true;
        curr.isActivated = curr.isHovered = false;
    }
    callbackAllowEdit(false);
    callbackAllowDelete(false);
}

void Graph::deleteNode() {
    if (activatedNode == -1) return;
    isDeleted[activatedNode] = true;
    for (Edge &curr : edges) {
        if (curr.fromNode != activatedNode && curr.toNode != activatedNode) continue;
        curr.isDeleted = true, curr.isActivated = curr.isHovered = false;
    }
    activatedNode = -1;
    callbackAllowDelete(false);
}

sf::FloatRect Graph::getBoundary() const {
    return sf::FloatRect();
}

bool Graph::nodeActivated() const {
    return activatedNode != -1;
}

bool Graph::edgeActivated() const {
    for (Edge item : edges)
        if (!item.isDeleted && item.isActivated) return true;
    return false;
}

void Graph::setAnnotation (int nodeID, int value) {
    std::string msg = (value == INT_MAX ? "INF" : std::to_string(value));
    if (nodeID < nodes.size()) nodes[nodeID]->setAnnotation(msg);
}

void Graph::handleMousePress (const sf::Vector2f &mousePos) {
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
            sf::Vector2f fromPos = nodes[curr.fromNode]->getPosition();
            sf::Vector2f toPos = nodes[curr.toNode]->getPosition();
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
    sf::Vector2f localPos = this->getInverseTransform().transformPoint(mousePos);
    for (UI::FloatingNode* ptr : nodes) {
        ptr->handleMouseRelease(localPos);
        ptr->deactivateNode();
    }
    if (activatedNode != -1) {
        nodes[activatedNode]->activateNode();
        callbackAllowDelete(true);
    }
}

void Graph::handleMouseMovement (const sf::Vector2f &mousePos) {
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
        sf::Vector2f fromPos = nodes[curr.fromNode]->getPosition();
        sf::Vector2f toPos = nodes[curr.toNode]->getPosition();
        if (isPointOnSegment(fromPos, toPos, localPos)) {
            curr.isHovered = true;
            return;
        }
    }
}

void Graph::handleTextEntered (const char &unicode) {}

void Graph::draw (sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (Edge item : edges) {
        if (item.isDeleted) continue;
        sf::Color edgeColor = (item.isActivated ?
            sf::Color::Red : (item.isHovered ?
                Theme::getHoveredButton() : sf::Color::Black));
        drawEdge(target, states, nodes[item.fromNode], nodes[item.toNode], item.weight, edgeColor);
    }
    for (int i = 0; i < nodes.size(); i++)
        if (!isDeleted[i]) target.draw(*nodes[i], states);
    
    // sf::CircleShape dot(4.f);
    // dot.setFillColor(sf::Color::Red);
    // dot.setPosition(getOrigin());
    // target.draw(dot, states);
}

}; // namespace UI