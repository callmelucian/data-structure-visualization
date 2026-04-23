#include "../../include/components/hash-map.hpp"

namespace HashConstant {
    const float idealDeltaWidth = 80.f;
    const float idealDeltaHeight = 100.f;
    const int hashConstant = 17;
};

namespace UI {

HashMap::HashMap() : rootNode(HashConstant::hashConstant), rootLink(HashConstant::hashConstant, -1) {
    for (int i = 0; i < HashConstant::hashConstant; i++)
        rootNode[i] = new AnimatedNode(std::to_string(i), 15.0F);
    calculatePositions();
}

HashMap::HashMap (const HashMap &other) : UI::Base(other) {
    copyFrom(other);
}

HashMap::~HashMap() {
    for (AnimatedNode* ptr : rootNode) delete ptr;
    for (AnimatedNode* ptr : nodeUI) delete ptr;
}

HashMap& HashMap::operator= (const HashMap &other) {
    if (this == &other) return *this;
    UI::Base::operator=(other);
    for (AnimatedNode* ptr : rootNode) delete ptr;
    for (AnimatedNode* ptr : nodeUI) delete ptr;
    rootNode.clear(), nodeUI.clear();
    return copyFrom(other), *this;
}

void HashMap::copyFrom (const HashMap &other) {
    this->rootLink = other.rootLink;
    this->nodeLink = other.nodeLink;
    this->isDeleted = other.isDeleted;
    this->targetOrigin = other.targetOrigin;
    this->highlighter = other.highlighter;

    rootNode.reserve(other.rootNode.size());
    nodeUI.reserve(other.nodeUI.size());

    for (int i = 0; i < other.rootNode.size(); i++) {
        if (other.rootNode[i]) rootNode.push_back(new AnimatedNode(*other.rootNode[i]));
        else rootNode.push_back(nullptr);
    }
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

void HashMap::createNode (int value) {
    nodeUI.push_back(new AnimatedNode(std::to_string(value)));
    nodeLink.push_back(-1);
    isDeleted.push_back(false);
}

void HashMap::deletedNode (int nodeID) {
    if (nodeID >= isDeleted.size()) return;
    isDeleted[nodeID] = true, nodeLink[nodeID] = -1;
    for (int &u : nodeLink)
        if (u == nodeID) u = -1;
    for (int &u : rootLink)
        if (u == nodeID) u = -1;
}

bool HashMap::addEdge (int fromID, int toID) {
    if (fromID >= nodeLink.size() || toID >= nodeLink.size()) return false;
    if (nodeLink[fromID] != toID) return nodeLink[fromID] = toID, true;
    return false;
}

bool HashMap::attachRoot (int slot, int nodeID) {
    if (slot >= rootLink.size() || nodeID >= nodeLink.size()) return false;
    if (rootLink[slot] != nodeID) return rootLink[slot] = nodeID, true;
    return false;
}

void HashMap::calculatePositions (float maxWidth, float maxHeight) {
    // compute actual vertical and horizontal spacing between nodes
    std::function<int()> getDepth = [&]() {
        int ans = 0;
        for (int slot = 0; slot < HashConstant::hashConstant; slot++) {
            int curr = 1;
            for (int currentNode = rootLink[slot]; currentNode != -1; currentNode = nodeLink[currentNode], curr++);
            ans = std::max(ans, curr);
        }
        return ans;
    };
    float deltaWidth = std::min(
        maxWidth / std::max(1, HashConstant::hashConstant - 1),
        HashConstant::idealDeltaWidth
    );
    float deltaHeight = std::min(
        maxHeight / std::max(1, getDepth() - 1),
        HashConstant::idealDeltaHeight  
    );

    // set position
    for (int slot = 0; slot < HashConstant::hashConstant; slot++) {
        rootNode[slot]->setTargetPosition(slot * deltaWidth, 0);
        for (int curNode = rootLink[slot], counter = 1; curNode != -1; curNode = nodeLink[curNode], counter++)
            nodeUI[curNode]->setTargetPosition(slot * deltaWidth, counter * deltaHeight);
    }

    // update origin
    updateOrigin();
}

void HashMap::updateOrigin() {
    float targetXMin = rootNode.front()->getTargetX(), targetXMax = rootNode.back()->getTargetX();
    float targetYMin = rootNode.front()->getTargetY(), targetYMax = rootNode.back()->getTargetY();
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

void HashMap::setTargetOrigin (float x, float y) {
    targetOrigin = {x, y};
}

void HashMap::timePropagation (float deltaTime) {
    // std::cerr << getOrigin() << " " << targetOrigin << std::endl;
    for (AnimatedNode* ptr : rootNode) ptr->timePropagation(deltaTime);
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

void HashMap::setHighlight (int nodeID) {
    if (nodeID == -1) highlighter.free();
    else highlighter.setTargetNode(nodeUI[nodeID]);
}

void HashMap::copyPosition (const HashMap &other) {
    for (int i = 0; i < rootNode.size(); i++)
        rootNode[i]->copyPosition(*other.rootNode[i]);
    for (int i = 0; i < nodeUI.size(); i++) {
        if (i < other.nodeUI.size())
            nodeUI[i]->copyPosition(*other.nodeUI[i]);
        else nodeUI[i]->randomPosition();
    }
    setOrigin(other.getOrigin());
}

void HashMap::draw (sf::RenderTarget &target, sf::RenderStates states) const {
    // apply state transform
    states.transform *= getTransform();

    // draw edges
    for (int slot = 0; slot < HashConstant::hashConstant; slot++) {
        int nodeID = rootLink[slot];
        if (nodeID != -1) drawEdge(target, states, rootNode[slot], nodeUI[nodeID]);
    }
    for (int nodeID = 0; nodeID < nodeUI.size(); nodeID++) {
        if (isDeleted[nodeID]) continue;
        int nextID = nodeLink[nodeID];
        if (nextID != -1) drawEdge(target, states, nodeUI[nodeID], nodeUI[nextID], true);
    }

    // draw nodes
    for (AnimatedNode* ptr : rootNode)
        target.draw(*ptr, states);
    for (int nodeID = 0; nodeID < nodeUI.size(); nodeID++)
        if (!isDeleted[nodeID]) target.draw(*nodeUI[nodeID], states);
    
    // draw highlighting circle
    target.draw(highlighter, states);
}

void HashMap::handleMousePress(const sf::Vector2f &mousePos) {}
void HashMap::handleMouseMovement(const sf::Vector2f &mousePos) {}
void HashMap::handleMouseRelease(const sf::Vector2f &mousePos) {}
void HashMap::handleTextEntered(const char &unicode) {}

sf::FloatRect HashMap::getBoundary() const {
    return sf::FloatRect();
}

}; // namespace UI