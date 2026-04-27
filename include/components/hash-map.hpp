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
#include "../core/global-setting.hpp"
#include "../core/utility.hpp"

namespace HashConstant {
    extern const float idealDeltaWidth;
    extern const float idealDeltaHeight;
    extern const int hashConstant;
};

namespace UI {

class HashMap : public UI::Base {
private:
    std::vector<AnimatedNode*> rootNode, nodeUI;
    std::vector<int> rootLink, nodeLink;
    std::vector<bool> isDeleted;
    sf::Vector2f targetOrigin;
    HighlightCircle<AnimatedNode> highlighter;

public:
    HashMap();
    HashMap (const HashMap &other);
    ~HashMap();

    HashMap& operator= (const HashMap &other);
    void copyFrom (const HashMap &other);

    void createNode (int value);

    void deletedNode (int nodeID);

    bool addEdge (int fromID, int toID);

    bool attachRoot (int slot, int nodeID);

    void calculatePositions (float maxWidth = Setting::narrowFocusX, float maxHeight = Setting::focusY);

    void updateOrigin();

    void setTargetOrigin (float x, float y);
    
    void timePropagation (float deltaTime);

    void setHighlight (int nodeID);

    void copyPosition (const HashMap &other);
    
    void colorNode (int nodeID, const sf::Color &color);


    // overrides
    void draw (sf::RenderTarget &target, sf::RenderStates states) const override;

    void handleMousePress(const sf::Vector2f &mousePos) override;
    void handleMouseMovement(const sf::Vector2f &mousePos) override;
    void handleMouseRelease(const sf::Vector2f &mousePos) override;
    void handleTextEntered(const char &unicode) override;
    sf::FloatRect getBoundary() const override;
};

}; // namespace UI