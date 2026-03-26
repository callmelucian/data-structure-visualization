#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <set>
#include <random>
#include <functional>
#include <string>
#include <algorithm>

#include "ui-base.hpp"
#include "nodes.hpp"
#include "../core/global-setting.hpp"
#include "../../assets/theme.hpp"

extern const float idealDeltaHeight;
extern const float idealDeltaWidth;

namespace UI {

class BinaryTree : public UI::Base {
private:
    std::vector<int> leftChild, rightChild, parent;
    std::vector<AnimatedNode> nodeUI;
    std::vector<bool> isDeleted;
    int rootNode, treeSize, highlightID;
    sf::Vector2f targetOrigin, targetHighlight;
    sf::CircleShape highlightCircle;

public:
    BinaryTree();

    int createNode(const std::string &s, bool isRoot = false);

    void deleteNode(int nodeID);

    void addEdge(int parentNode, int childNode, bool isLeft);

    void setRootNode(int targetNode);

    void swapNode(int nodeA, int nodeB);

    void calculatePositions(float maxWidth = Setting::focusX, float maxHeight = Setting::focusY);

    void handleMousePress(const sf::Vector2f &mousePos) override;

    void handleMouseMovement(const sf::Vector2f &mousePos) override;

    void handleMouseRelease(const sf::Vector2f &mousePos) override;

    void handleTextEntered(const char &unicode) override;

    sf::FloatRect getBoundary() const override;

    void setTargetOrigin(float x, float y);

    void timePropagation(float deltaTime);
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setHighlight (int nodeID);
};

} // namespace UI