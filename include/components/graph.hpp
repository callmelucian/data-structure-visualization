#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <tuple>
#include <set>
#include <utility>

// utilities
#include "../core/utility.hpp"
#include "../core/global-setting.hpp"
#include "../../assets/theme.hpp"

// UI components
#include "ui-base.hpp"
#include "nodes.hpp"
#include "highlight-circle.hpp"

extern const float REPULSIVE_CONSTANT;
extern const float IDEAL_LENGTH;
extern const float SPRING_CONSTANT;
extern const float FRICTION_CONSTANT;

namespace UI {

struct Edge : public UI::Base {
    int fromID, toID, weight;
    bool isDeleted, isActivated, isHovered, isHighlighted, isDirected;
    sf::Color defaultColor;
    Node *fromObserver, *toObserver;
    float thickness;

    Edge();
    Edge (int fromNode, int toNode, int weight, Node* fromObserver, Node* toObserver, bool isDirected);

    void updateObserver (Node *newFrom, Node* newTo);
    void setDefaultColor (const sf::Color &color);

    sf::Vector2f getFromPosition() const;
    sf::Vector2f getToPosition() const;
    sf::Color getColor() const;

    void drawEdge (sf::RenderTarget &target, sf::RenderStates states, const sf::Vector2f &fromPos, const sf::Vector2f &toPos) const;
    void draw (sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::FloatRect getBoundary() const override;

    void handleMousePress (const sf::Vector2f &mousePos) override;
    void handleMouseRelease (const sf::Vector2f &mousePos) override;
    void handleMouseMovement (const sf::Vector2f &mousePos) override;
    void handleTextEntered (const char &unicode) override;
};

class Graph : public UI::Base {
private:
    std::vector<UI::FloatingNode*> nodes;
    std::vector<bool> isDeleted;
    std::vector<Edge> edges;
    UI::HighlightCircle<FloatingNode> highlighter;
    sf::Vector2f targetOrigin;
    int activatedNode;
    bool isDirected;

    std::function<void(bool)> callbackAllowEdit;
    std::function<void(bool)> callbackAllowDelete;
    std::function<bool(const sf::Vector2f &mousePos)> callbackIsEditing;
    std::function<void(int,int)> callbackTriggerAddEdge;

public:
    Graph();
    Graph (const Graph &other);
    ~Graph();

    void setCallbackAllowEdit (auto func) {
        callbackAllowEdit = func;
    }

    void setCallbackAllowDelete (auto func) {
        callbackAllowDelete = func;
    }

    void setCallbackTriggerAddEdge (auto func) {
        callbackTriggerAddEdge = func;
    }

    void setCallbackIsEditing (auto func) {
        callbackIsEditing = func;
    }

    Graph& operator= (const Graph &other);

    void copyFrom (const Graph &other);

    void timePropagation (float deltaTime, float maxWidth = Setting::tightFocusX, float maxHeight = Setting::focusY);

    void makeDirected();
    
    void insertNode (int label);

    void insertEdge (int fromNode, int toNode, int weight);

    void setEdgeColor (int edgeID, const sf::Color &color);

    void setAnnotation (int nodeID, int value);

    void setTargetOrigin (const sf::Vector2f &pos);

    void autosetTargetOrigin();

    void changeWeight (int edgeID, int newWeight);
    
    void deleteEdge (int edgeID);

    void deleteNode (int nodeID);

    sf::FloatRect getBoundary() const override;
    
    int nodeActivated();
    int edgeActivated();

    void resetNodeActivation();
    void resetEdgeActivation();

    void clearAnnotation();
    void markAnnotation (int nodeID);

    void highlightNode (int nodeID);
    void highlightEdge (int edgeID);

    void copyPosition (const Graph &other);

    void calculatePositions (float maxWidth, float maxHeight);

    void handleMousePress (const sf::Vector2f &mousePos) override;
    void handleMouseRelease (const sf::Vector2f &mousePos) override;
    void handleMouseMovement (const sf::Vector2f &mousePos) override;
    void handleTextEntered (const char &unicode) override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}; // namespace UI