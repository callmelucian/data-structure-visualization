#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include <utility>

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
};

namespace UI {

class LinkedList : public UI::Base {
private:
    std::vector<AnimatedNode*> nodeUI;
    std::vector<int> nodeLink;
    std::vector<bool> isDeleted;
    sf::Vector2f targetOrigin;
    UI::HighlightCircle<AnimatedNode> highlighter;
    int headNode;

public:
    LinkedList();
    LinkedList (const LinkedList &other);
    ~LinkedList();
    LinkedList& operator= (const LinkedList &other);

    void createNode (const std::string &s, bool isHead = false);
    void deleteNode (int nodeID);
    void colorNode (int nodeID, sf::Color &color);
    void addEdge (int fromNode, int toNode);
    void setHead (int nodeID);
    void calculatePositions (float maxWidth = Setting::narrowFocusX, float maxHeight = Setting::focusY);
    void updateOrigin();
    void setTargetOrigin (float x, float y);
    void setHighlight (int nodeID);
    void copyPosition (const LinkedList &other);
    void copyFrom (const LinkedList &other);
    void timePropagation (float deltaTime);
    void changeValue (int nodeID, const std::string &s);
    void changeColor();
    
    // overrides
    void draw (sf::RenderTarget &target, sf::RenderStates states) const override;
    void handleMousePress(const sf::Vector2f &mousePos) override;
    void handleMouseMovement(const sf::Vector2f &mousePos) override;
    void handleMouseRelease(const sf::Vector2f &mousePos) override;
    void handleTextEntered(const char &unicode) override;
    sf::FloatRect getBoundary() const override;
};

}; // namespace UI