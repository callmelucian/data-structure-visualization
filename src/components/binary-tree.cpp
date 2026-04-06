#include "../../include/components/binary-tree.hpp"

const float idealDeltaHeight = 200.f;
const float idealDeltaWidth = 400.f;

namespace UI {

BinaryTree::BinaryTree() : rootNode(0), treeSize(0), targetOrigin({0, 0}) {}

BinaryTree::BinaryTree (const BinaryTree &o) : UI::Base(o) {
    leftChild = o.leftChild, rightChild = o.rightChild, parent = o.parent;
    highlighter = o.highlighter;
    for (AnimatedNode* tmpElement : o.nodeUI) {
        AnimatedNode* ptr = new AnimatedNode(*tmpElement);
        if (o.highlighter.getAddress() == tmpElement)
            highlighter.setTargetNode(ptr);
        nodeUI.push_back(ptr);
    }
    isDeleted = o.isDeleted;
    rootNode = o.rootNode, treeSize = o.treeSize;
    targetOrigin = o.targetOrigin;
}

BinaryTree::~BinaryTree() {
    for (AnimatedNode* &ptr : nodeUI) delete ptr;
}

BinaryTree& BinaryTree::operator= (const BinaryTree &o) {
    if (this == &o) return *this;
    UI::Base::operator=(o);

    // copy from destructor
    for (AnimatedNode* ptr : nodeUI) delete ptr;
    nodeUI.clear();

    // copy from copy constructor
    leftChild = o.leftChild, rightChild = o.rightChild, parent = o.parent;
    highlighter = o.highlighter;
    for (AnimatedNode* tmpElement : o.nodeUI) {
        AnimatedNode* ptr = new AnimatedNode(*tmpElement);
        if (o.highlighter.getAddress() == tmpElement)
            highlighter.setTargetNode(ptr);
        nodeUI.push_back(ptr);
    }
    isDeleted = o.isDeleted;
    rootNode = o.rootNode, treeSize = o.treeSize;
    targetOrigin = o.targetOrigin;

    // return reference
    return *this;
}

void BinaryTree::createNode(const std::string &s, bool isRoot) {
    nodeUI.push_back(new AnimatedNode(s));
    isDeleted.push_back(false);
    leftChild.push_back(-1);
    rightChild.push_back(-1);
    parent.push_back(-1);
    treeSize++;
    if (isRoot) setRootNode((int)nodeUI.size() - 1);
}

void BinaryTree::deleteNode(int nodeID) {
    if (leftChild[parent[nodeID]] == nodeID) leftChild[parent[nodeID]] = -1;
    if (rightChild[parent[nodeID]] == nodeID) rightChild[parent[nodeID]] = -1;
    leftChild[nodeID] = rightChild[nodeID] = -1, isDeleted[nodeID] = true, treeSize--;
    if (highlighter.getAddress() == nodeUI[nodeID]) highlighter.free();
}

bool BinaryTree::addEdge(int parentNode, int childNode, bool isLeft) {
    int &targetMemory = (isLeft ? leftChild : rightChild)[parentNode];
    if (targetMemory == childNode) return false;
    targetMemory = childNode;
    if (childNode != -1) parent[childNode] = parentNode;
    return true;
}

bool BinaryTree::setRootNode(int targetNode) {
    if (rootNode == targetNode) return false;
    rootNode = targetNode;
    if (targetNode != -1) parent[targetNode] = -1;
    return true;
}

void BinaryTree::swapNode(int nodeA, int nodeB) {
    swapAnimatedNode(*nodeUI[nodeA], *nodeUI[nodeB]);
}

void BinaryTree::calculatePositions(float maxWidth, float maxHeight) {
    // edge case: tree only contains 1 node
    if (treeSize == 0) return;
    if (treeSize == 1) {
        nodeUI[rootNode]->setTargetPosition(0, 0);
        setTargetOrigin(0, 0);
        return;
    }

    // find height and precalculate distance between leaf nodes
    std::function<int(int)> getHeight = [&](int u) {
        int height = 1;
        if (leftChild[u] != -1) height = std::max(height, 1 + getHeight(leftChild[u]));
        if (rightChild[u] != -1) height = std::max(height, 1 + getHeight(rightChild[u]));
        return height;
    };
    int treeHeight = getHeight(rootNode);
    int lastLayer = (1 << (treeHeight - 1));

    // precalculations
    float deltaWidth = std::min(idealDeltaWidth, maxWidth / std::max(1, lastLayer - 1));
    float deltaHeight = std::min(idealDeltaHeight, maxHeight / std::max(1, treeHeight - 1));
    float xMin = (maxWidth - deltaWidth * (lastLayer - 1)) / 2.f;
    float yMin = (maxHeight - deltaHeight * (treeHeight - 1)) / 2.f;

    // run DFS to assign positions
    std::function<void(int, int, float, float)> dfsTree = [&](int u, int depth, float width, float height) {
        assert(u != -1);
        int maxLeafCount = (1 << (treeHeight - depth));
        nodeUI[u]->setTargetPosition(width + deltaWidth * (maxLeafCount - 1) / 2.f, height);
        if (leftChild[u] != -1) dfsTree(leftChild[u], depth + 1, width, height + deltaHeight);
        if (rightChild[u] != -1) dfsTree(rightChild[u], depth + 1, width + deltaWidth * (maxLeafCount / 2), height + deltaHeight);
    };
    dfsTree(rootNode, 1, xMin, yMin);

    // center origin
    float targetXMin = nodeUI[rootNode]->getTargetX(), targetXMax = nodeUI[rootNode]->getTargetX();
    float targetYMin = nodeUI[rootNode]->getTargetY(), targetYMax = nodeUI[rootNode]->getTargetY();
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

void BinaryTree::handleMousePress(const sf::Vector2f &mousePos) {}

void BinaryTree::handleMouseMovement(const sf::Vector2f &mousePos) {}

void BinaryTree::handleMouseRelease(const sf::Vector2f &mousePos) {}

void BinaryTree::handleTextEntered(const char &unicode) {}

sf::FloatRect BinaryTree::getBoundary() const {
    return sf::FloatRect();
}

void BinaryTree::setTargetOrigin(float x, float y) {
    targetOrigin.x = x;
    targetOrigin.y = y;
}

void BinaryTree::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply state transform
    states.transform *= getTransform();

    // show circle and annotation
    for (int i = 0; i < (int)parent.size(); i++) {
        if (leftChild[i] != -1) drawEdge(target, states, nodeUI[i], nodeUI[leftChild[i]]);
        if (rightChild[i] != -1) drawEdge(target, states, nodeUI[i], nodeUI[rightChild[i]]);
    }
    for (int i = 0; i < (int)nodeUI.size(); i++)
        if (!isDeleted[i]) target.draw(*nodeUI[i], states);
    
    // show highlight circle
    target.draw(highlighter, states);
}

void BinaryTree::timePropagation (float deltaTime) {
    // call time-propagation for member components
    for (AnimatedNode* node : nodeUI) node->timePropagation(deltaTime);
    highlighter.timePropagation(deltaTime);
    
    // re-align origin
    sf::Vector2f displacement = targetOrigin - getOrigin();
    if (magnitude(displacement) < eps) setOrigin(targetOrigin);
    else {
        sf::Vector2f newPosition = getOrigin() + displacement * Setting::animationFactor * deltaTime;
        setOrigin(newPosition);
    }
}

bool BinaryTree::setHighlight (int nodeID) {
    if (nodeID == -2) return highlighter.free(), true;
    if (nodeID == -1) {
        if (highlighter.getAddress() == nullptr) return false;
        return highlighter.free(), true;
    }
    else {
        if (highlighter.getAddress() == nodeUI[nodeID]) return false;
        highlighter.setTargetNode(nodeUI[nodeID]);
        return true;
    }
}

void BinaryTree::lockHighlight() {
    highlighter.lockHighlight();
}

void BinaryTree::fastForward() {
    // call time-propagation for member components
    for (AnimatedNode* node : nodeUI) node->fastForward();
    highlighter.fastForward();
    // re-align origin
    setOrigin(targetOrigin);
}

void BinaryTree::changeNodeUI (int pos, AnimatedNode* ptr) {
    nodeUI[pos] = ptr;
}

void BinaryTree::copyPosition (const BinaryTree &o) {
    for (int i = 0; i < nodeUI.size(); i++) {
        if (i < o.nodeUI.size())
            nodeUI[i]->setPosition(o.nodeUI[i]->getPosition());
        else nodeUI[i]->randomPosition();
    }
    highlighter.setPosition(o.highlighter.getPosition());
    setOrigin(o.getOrigin());
}

void BinaryTree::printPosition() const {
    for (int i = 0; i < nodeUI.size(); i++)
        std::cerr << nodeUI[i]->getPosition() << " ";
    std::cerr << "\n";
}

} // namespace UI