#include "../../include/components/graph.hpp"

// ========== CONSTANTS INITIALIZATION ==========
const float REPULSIVE_CONSTANT = 100'000.f;
const float IDEAL_LENGTH = 100.f;
const float SPRING_CONSTANT = 0.05f;
const float FRICTION_CONSTANT = 0.9f;

// ========== GRAPH IMPLEMENTATION ==========
namespace UI {

Graph::Graph() : targetOrigin({0, 0}) {}

Graph::~Graph() {
    for (UI::FloatingNode* ptr : nodes) delete ptr;
}

Graph::Graph(const Graph &other) : UI::Base(other), edges(other.edges), highlighter(other.highlighter), targetOrigin(other.targetOrigin) {
    nodes.reserve(other.nodes.size());
    for (UI::FloatingNode* node : other.nodes) {
        if (node != nullptr) nodes.push_back(new UI::FloatingNode(*node));
        else nodes.push_back(nullptr);
    }
}

Graph& Graph::operator=(const Graph &other) {
    if (this == &other) return *this;
    UI::Base::operator=(other);

    for (UI::FloatingNode* node : nodes) delete node;
    nodes.clear();

    this->edges = other.edges;
    this->highlighter = other.highlighter;
    this->targetOrigin = other.targetOrigin;

    this->nodes.reserve(other.nodes.size());
    for (UI::FloatingNode* node : other.nodes) {
        if (node != nullptr) this->nodes.push_back(new UI::FloatingNode(*node));
        else this->nodes.push_back(nullptr);
    }
    return *this;
}

void Graph::timePropagation (float deltaTime) {
    // apply repulsive force
    for (UI::FloatingNode* ptr : nodes) {
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
    for (auto [fromID, toID] : edges) {
        sf::Vector2f fromPosition = nodes[fromID]->getPosition();
        sf::Vector2f toPosition = nodes[toID]->getPosition();
        sf::Vector2f springForce = unitVector(fromPosition, toPosition) * SPRING_CONSTANT * (distance(fromPosition, toPosition) - IDEAL_LENGTH);
        nodes[fromID]->addAcceleration(springForce);
        nodes[toID]->addAcceleration(-springForce);
    }

    // move nodes
    for (UI::FloatingNode* ptr : nodes) {
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
    nodes.push_back(new FloatingNode(std::to_string(label)));
    autosetTargetOrigin();
}

void Graph::insertEdge (int fromNode, int toNode) {
    edges.emplace(fromNode, toNode);
}

void Graph::setTargetOrigin (const sf::Vector2f &pos) {
    targetOrigin = pos;
}

void Graph::autosetTargetOrigin() {
    float positionXMin = 0.f, positionXMax = 0.f, positionYMin = 0.f, positionYMax = 0.f;
    for (FloatingNode* ptr : nodes) {
        positionXMin = positionXMax = ptr->getPosition().x;
        positionYMin = positionYMax = ptr->getPosition().y;
    }
    for (FloatingNode* ptr : nodes) {
        positionXMin = std::min(positionXMin, ptr->getPosition().x);
        positionXMax = std::max(positionXMax, ptr->getPosition().x);
        positionYMin = std::min(positionYMin, ptr->getPosition().y);
        positionYMax = std::max(positionYMax, ptr->getPosition().y);
    }
    float originX = (positionXMin + positionXMax) / 2.f;
    float originY = (positionYMin + positionYMax) / 2.f;
    targetOrigin = {originX, originY};
}

sf::FloatRect Graph::getBoundary() const {
    return sf::FloatRect();
}

void Graph::setAnnotation (int nodeID, int value) {
    std::string msg = (value == INT_MAX ? "INF" : std::to_string(value));
    if (nodeID < nodes.size()) nodes[nodeID]->setAnnotation(msg);
}

void Graph::handleMousePress (const sf::Vector2f &mousePos) {}
void Graph::handleMouseRelease (const sf::Vector2f &mousePos) {}
void Graph::handleMouseMovement (const sf::Vector2f &mousePos) {}
void Graph::handleTextEntered (const char &unicode) {}

void Graph::draw (sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (auto [fromID, toID] : edges)
        drawEdge(target, states, nodes[fromID], nodes[toID]);
    for (UI::FloatingNode* ptr : nodes)
        target.draw(*ptr, states);
    
    sf::CircleShape dot(4.f);
    dot.setFillColor(sf::Color::Red);
    dot.setPosition(getOrigin());
    target.draw(dot, states);
}

}; // namespace UI