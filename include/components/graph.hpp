#include <SFML/Graphics.hpp>
#include <vector>
#include <tuple>
#include <set>
#include <utility>

// utilities
#include "../core/utility.hpp"
#include "../core/global-setting.hpp"

// UI components
#include "ui-base.hpp"
#include "nodes.hpp"
#include "highlight-circle.hpp"

extern const float REPULSIVE_CONSTANT;
extern const float IDEAL_LENGTH;
extern const float SPRING_CONSTANT;
extern const float FRICTION_CONSTANT;

namespace UI {

class Graph : public UI::Base {
private:
    struct Edge {
        int fromNode, toNode, weight;
        bool isDeleted, isActivated, isHovered;

        Edge() : fromNode(0), toNode(0), weight(0), isDeleted(false), isActivated(false), isHovered(false) {}
        Edge (int fromNode, int toNode, int weight) :
            fromNode(fromNode), toNode(toNode), weight(weight), isDeleted(false), isActivated(false), isHovered(false) {}
    };
    std::vector<UI::FloatingNode*> nodes;
    std::vector<bool> isDeleted;
    std::vector<Edge> edges;
    UI::HighlightCircle highlighter;
    sf::Vector2f targetOrigin;
    int activatedNode;

    std::function<void(bool)> callbackAllowEdit;
    std::function<void(bool)> callbackAllowDelete;
    std::function<void(int,int)> callbackTriggerAddEdge;
    std::function<bool(const sf::Vector2f &mousePos)> callbackIsEditing;

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

    void timePropagation (float deltaTime);

    void insertNode (int label);

    void insertEdge (int fromNode, int toNode);

    void setAnnotation (int nodeID, int value);

    void setTargetOrigin (const sf::Vector2f &pos);

    void autosetTargetOrigin();

    void changeWeight (int edgeID, int newWeight);
    
    void deleteEdge (int edgeID);

    void deleteNode (int nodeID);

    sf::FloatRect getBoundary() const override;
    
    int nodeActivated() const;
    int edgeActivated() const;

    void clearAnnotation();
    void markAnnotation();

    void highlightNode (int nodeID);
    void highlightEdge (int edgeID);

    void handleMousePress (const sf::Vector2f &mousePos) override;
    void handleMouseRelease (const sf::Vector2f &mousePos) override;
    void handleMouseMovement (const sf::Vector2f &mousePos) override;
    void handleTextEntered (const char &unicode) override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}; // namespace UI