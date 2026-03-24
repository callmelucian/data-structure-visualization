#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <random>
#include <functional>
#include <string>

#include "ui-base.hpp"
#include "nodes.hpp"
#include "../core/global-setting.hpp"

const float idealDeltaHeight = 200.f;
const float idealDeltaWidth = 400.f;

namespace UI {
class BinaryTree : public UI::Base {
private:
    std::vector<int> leftChild, rightChild, parent;
    std::vector<AnimatedNode> nodeUI;
    std::vector<bool> isDeleted;
    int rootNode, treeSize;
    sf::Vector2f targetOrigin;

public:
    BinaryTree() : rootNode(0), treeSize(0), targetOrigin({0, 0}) {}

    int createNode (const std::string &s, bool isRoot = false) {
        nodeUI.emplace_back(s), isDeleted.push_back(false), treeSize++;
        leftChild.push_back(0), rightChild.push_back(0), parent.push_back(0);
        if (isRoot) setRootNode(treeSize - 1);
        return treeSize - 1;
    }

    void setRootNode (int targetNode) {
        rootNode = targetNode;
        calculatePositions();
    }

    void addEdge (int parentNode, int childNode, bool isLeft) {
        (isLeft ? leftChild[parentNode] : rightChild[parentNode]) = childNode;
        parent[childNode] = parentNode;
        calculatePositions();
    }

    void deleteNode (int nodeID) {
        if (leftChild[parent[nodeID]] == nodeID) leftChild[parent[nodeID]] = 0;
        if (rightChild[parent[nodeID]] == nodeID) rightChild[parent[nodeID]] = 0;
        leftChild[nodeID] = rightChild[nodeID] = 0, isDeleted[nodeID] = true;
    }

    void calculatePositions (float maxWidth = Setting::focusX, float maxHeight = Setting::focusY) {
        // edge case: tree only contains 1 node
        if (treeSize == 0) return;
        if (treeSize == 1) {
            nodeUI[rootNode].setTargetPosition(0, 0);
            setTargetOrigin(0, 0);
            return;
        }

        // find height and precalculate distance between leaf nodes
        std::function<int(int)> getHeight = [&] (int u) {
            int height = 1;
            if (leftChild[u]) height = std::max(height, 1 + getHeight(leftChild[u]));
            if (rightChild[u]) height = std::max(height, 1 + getHeight(rightChild[u]));
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
        std::function<void(int, int, float, float)> dfsTree = [&] (int u, int depth, float width, float height) {
            int maxLeafCount = (1 << (treeHeight - depth));
            nodeUI[u].setTargetPosition(width + deltaWidth * (maxLeafCount - 1) / 2.f, height);

            if (leftChild[u]) dfsTree(leftChild[u], depth + 1, width, height + deltaHeight);
            if (rightChild[u]) dfsTree(rightChild[u], depth + 1, width + deltaWidth * (maxLeafCount / 2), height + deltaHeight);
        };
        dfsTree(rootNode, 1, xMin, yMin);

        // center origin
        float targetXMin = nodeUI[0].getTargetX(), targetXMax = nodeUI[0].getTargetX();
        float targetYMin = nodeUI[0].getTargetY(), targetYMax = nodeUI[0].getTargetY();
        for (const AnimatedNode &circle : nodeUI) {
            targetXMin = std::min(targetXMin, circle.getTargetX());
            targetXMax = std::max(targetXMax, circle.getTargetX());
            targetYMin = std::min(targetYMin, circle.getTargetY());
            targetYMax = std::max(targetYMax, circle.getTargetY());
        }
        float originX = (targetXMin + targetXMax) / 2.f;
        float originY = (targetYMin + targetYMax) / 2.f;
        setTargetOrigin(originX, originY);
    }

    void handleMousePress (const sf::Vector2f &mousePos) override {}
    void handleMouseMovement (const sf::Vector2f &mousePos) override {}
    void handleMouseRelease (const sf::Vector2f &mousePos) override {}
    void handleTextEntered (const char &unicode) override {}

    sf::FloatRect getBoundary() const override {
        return sf::FloatRect();
    }

    sf::FloatRect getLocalBounds() const {
        if (nodeUI.empty()) return sf::FloatRect();
        sf::FloatRect totalBounds = nodeUI[0].getGlobalBounds();
        for (const auto& circle : nodeUI) {
            sf::FloatRect itemBounds = circle.getGlobalBounds();
            float left = std::min(totalBounds.position.x, itemBounds.position.x);
            float top = std::min(totalBounds.position.y, itemBounds.position.y);
            float right = std::max(totalBounds.position.x + totalBounds.size.x, itemBounds.position.x + itemBounds.size.x);
            float bottom = std::max(totalBounds.position.y + totalBounds.size.y, itemBounds.position.y + itemBounds.size.y);

            totalBounds.position = {left, top};
            totalBounds.size = {right - left, bottom - top};
        }
        return totalBounds;
    }

    void setTargetOrigin (float x, float y) {
        targetOrigin.x = x;
        targetOrigin.y = y;
    }

    void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
        // apply state transform
        states.transform *= getTransform();

        // show circle and annotation
        for (const AnimatedNode &node : nodeUI) target.draw(node, states);
        for (int i = 0; i < parent.size(); i++) {
            if (leftChild[i]) drawEdge(target, states, nodeUI[i], nodeUI[leftChild[i]]);
            if (rightChild[i]) drawEdge(target, states, nodeUI[i], nodeUI[rightChild[i]]);
        }
    }

    void timePropagation (float deltaTime) {
        for (AnimatedNode &node : nodeUI) node.timePropagation(deltaTime);
        sf::Vector2f displacement = targetOrigin - getOrigin();
        if (magnitude(displacement) < eps) setOrigin(targetOrigin);
        else {
            sf::Vector2f newPosition = getOrigin() + displacement * Setting::animationFactor * deltaTime;
            setOrigin(newPosition);
        }
    }
};
};