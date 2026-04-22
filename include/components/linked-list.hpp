#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>

// ui components
#include "ui-base.hpp"
#include "nodes.hpp"
#include "highlight-circle.hpp"

// utility
#include "../../assets/theme.hpp"
#include "../core/global-setting.hpp"
#include "../core/utility.hpp"

namespace LinkedListConstant {
    extern const float idealDeltaWidth;
    extern const float idealDeltaHeight;
    extern const int hashConstant;
};

namespace UI {

class LinkedList : public UI::Base {
private:
    std::vector<AnimatedNode*> nodeUI;
    UI::HighlightCircle<AnimatedNode> highlighter;
    std::vector<int> nodeLink;
    sf::Vector2f targetOrigin;
    int headNode;

public:
    LinkedList();
    LinkedList (const LinkedList &other);
    ~LinkedList();
    LinkedList& operator= (const LinkedList &other);

    void createNode (const std::string &s, bool isHead = false);
    void deleteNode (int nodeID);
    void colorNode (int nodeID, const sf::Color &color);
    void addEdge (int fromNode, int toNode);
    void setHead (int nodeID);
    void calculatePositions (float maxWidth = Setting::focusX, float maxHeight = Setting::focusY);
    void calculateOrigin();
    void setTargetOrigin (float x, float y);
    void setHighlight (int nodeID);
    void copyPosition (const LinkedList &other);
    
    // overrides
    void handleMousePress(const sf::Vector2f &mousePos) override;
    void handleMouseMovement(const sf::Vector2f &mousePos) override;
    void handleMouseRelease(const sf::Vector2f &mousePos) override;
    void handleTextEntered(const char &unicode) override;
    sf::FloatRect getBoundary() const override;
};

}; // namespace UI