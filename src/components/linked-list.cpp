#include "../../include/components/linked-list.hpp"

namespace LinkedListConstant {
    const float idealDeltaWidth = 200.f;
    const float idealDeltaHeight = 50.f;
};

namespace UI {

LinkedList::LinkedList() : headNode(-1) {}

LinkedList::LinkedList (const LinkedList &other) : UI::Base(other) {
    copyFrom(other);
}

LinkedList::~LinkedList() {
    for (AnimatedNode* ptr : nodeUI) delete ptr;
}

LinkedList& LinkedList::operator= (const LinkedList &other) {
    if (this == &other) return *this;
    UI::Base::operator=(other);
    for (AnimatedNode* ptr : nodeUI) delete ptr;
    nodeUI.clear();
    return copyFrom(other), *this;
}

void LinkedList::createNode (const std::string &s, bool isHead) {
    if (isHead) headNode = nodeUI.size();
    nodeUI.push_back(new AnimatedNode(s));
    nodeLink.push_back(-1);
    isDeleted.push_back(false);
}

void LinkedList::deleteNode (int nodeID) {
    if (nodeID >= isDeleted.size()) return;
    isDeleted[nodeID] = true, nodeLink[nodeID] = -1;
    for (int &u : nodeLink)
        if (u == nodeID) u = -1;
}

void LinkedList::colorNode (int nodeID, const sf::Color &color) {
    if (nodeID < nodeUI.size())
        nodeUI[nodeID]->setColor(color);
}

void LinkedList::addEdge (int fromNode, int toNode) {
    if (fromNode < nodeLink.size() && toNode < nodeLink.size())
        nodeLink[fromNode] = toNode;
}

void LinkedList::setHead (int nodeID) {
    headNode = nodeID;
}

void LinkedList::calculatePositions (float maxWidth, float maxHeight) {
    if (headNode == -1) return;
    std::function<int()> getLength = [&]() {
        int counter = 0;
        for (int curNode = headNode; curNode != -1; curNode = nodeLink[curNode]) counter++;
        return counter;
    };

    float deltaWidth = std::min(
        LinkedListConstant::idealDeltaWidth,
        maxWidth / std::max(1, getLength())
    );

    for (int curNode = headNode, counter = 0; curNode != -1; curNode = nodeLink[curNode], counter++)
        nodeUI[curNode]->setTargetPosition(counter * deltaWidth, 0);
    updateOrigin();
}

void LinkedList::updateOrigin() {
    float targetXMin = nodeUI[headNode]->getTargetX(), targetXMax = nodeUI[headNode]->getTargetX();
    float targetYMin = nodeUI[headNode]->getTargetY(), targetYMax = nodeUI[headNode]->getTargetY();
    for (int i = 0; i < nodeUI.size(); i++) {
        if (isDeleted[i]) continue;
        targetXMin = std::min(targetXMin, nodeUI[i]->getTargetX());
        targetXMax = std::max(targetXMax, nodeUI[i]->getTargetX());
        targetYMin = std::min(targetYMin, nodeUI[i]->getTargetY());
        targetYMax = std::max(targetYMax, nodeUI[i]->getTargetY());
    }
    float originX = (targetXMin + targetXMax) / 2.f;
    float originY = (targetYMin + targetYMax) / 2.f;
    setTargetOrigin(originX, originY);
}

void LinkedList::setTargetOrigin (float x, float y) {
    targetOrigin = {x, y};
}

void LinkedList::setHighlight (int nodeID) {
    if (nodeID == -1) highlighter.free();
    else highlighter.setTargetNode(nodeUI[nodeID]);
}

void LinkedList::copyPosition (const LinkedList &other) {
    for (int i = 0; i < nodeUI.size(); i++) {
        if (i < other.nodeUI.size())
            nodeUI[i]->setPosition(other.nodeUI[i]->getPosition());
        else nodeUI[i]->randomPosition();
    }
    setOrigin(other.getOrigin());
}

void LinkedList::copyFrom (const LinkedList &other) {
    this->nodeLink = other.nodeLink;
    this->isDeleted = other.isDeleted;
    this->targetOrigin = other.targetOrigin;
    this->highlighter = other.highlighter;
    this->headNode = other.headNode;

    nodeUI.reserve(other.nodeUI.size());
    for (int i = 0; i < other.nodeUI.size(); i++) {
        if (other.nodeUI[i]) {
            AnimatedNode* ptr = new AnimatedNode(*other.nodeUI[i]);
            if (highlighter.getAddress() == other.nodeUI[i])
                highlighter.setTargetNode(ptr);
            nodeUI.push_back(ptr);
        }
        else nodeUI.push_back(nullptr);
    }
}

void LinkedList::timePropagation (float deltaTime) {
    for (AnimatedNode* ptr : nodeUI) ptr->timePropagation(deltaTime);
    highlighter.timePropagation(deltaTime);

    // re-align origin
    sf::Vector2f displacement = targetOrigin - getOrigin();
    if (magnitude(displacement) < eps) setOrigin(targetOrigin);
    else {
        sf::Vector2f newPosition = getOrigin() + displacement * Setting::animationFactor() * deltaTime;
        setOrigin(newPosition);
    }
}

void LinkedList::changeValue (int nodeID, const std::string &s) {
    if (nodeID < nodeUI.size()) nodeUI[nodeID]->setString(s);
}

// overrides
void LinkedList::draw (sf::RenderTarget &target, sf::RenderStates states) const {
    // apply state transform
    states.transform *= getTransform();

    // draw edges
    for (int nodeID = 0; nodeID < nodeUI.size(); nodeID++) {
        if (isDeleted[nodeID]) continue;
        int nextID = nodeLink[nodeID];
        if (nextID != -1) drawEdge(target, states, nodeUI[nodeID], nodeUI[nextID], true);
    }

    // draw nodes
    for (int nodeID = 0; nodeID < nodeUI.size(); nodeID++)
        if (!isDeleted[nodeID]) target.draw(*nodeUI[nodeID], states);
    
    // draw highlighting circle
    target.draw(highlighter, states);
}

void LinkedList::handleMousePress (const sf::Vector2f &mousePos) {}
void LinkedList::handleMouseMovement (const sf::Vector2f &mousePos) {}
void LinkedList::handleMouseRelease(const sf::Vector2f &mousePos) {}
void LinkedList::handleTextEntered (const char &unicode) {}
sf::FloatRect LinkedList::getBoundary() const {
    return sf::FloatRect();
}

}; // namespace UI