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
#include "highlight-circle.hpp"
#include "../core/global-setting.hpp"
#include "../../assets/theme.hpp"

extern const float idealDeltaHeight;
extern const float idealDeltaWidth;

namespace UI {

class BinaryTree : public UI::Base {
private:
    std::vector<int> leftChild, rightChild, parent;
    std::vector<AnimatedNode*> nodeUI; // there are reasons for using a pointer here
    std::vector<bool> isDeleted;
    int rootNode, treeSize;
    sf::Vector2f targetOrigin;
    UI::HighlightCircle highlighter;

public:
    BinaryTree();
    BinaryTree(const BinaryTree &o);

    ~BinaryTree();

    BinaryTree& operator= (const BinaryTree &o);

    void createNode(const std::string &s, bool isRoot = false);

    void deleteNode(int nodeID);

    bool addEdge(int parentNode, int childNode, bool isLeft);

    bool setRootNode(int targetNode);

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

    bool setHighlight (int nodeID);

    void lockHighlight();

    void fastForward();

    void changeNodeUI (int pos, AnimatedNode* ptr);

    void copyPosition (const BinaryTree &o);

    void printPosition() const;
};

} // namespace UI