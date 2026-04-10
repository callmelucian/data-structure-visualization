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
    std::vector<UI::FloatingNode*> nodes;
    std::set<std::pair<int,int>> edges;
    UI::HighlightCircle highlighter;
    sf::Vector2f targetOrigin;
public:
    Graph();
    Graph (const Graph &other);
    ~Graph();

    Graph& operator= (const Graph &other);

    void timePropagation (float deltaTime);

    void insertNode (int label);

    void insertEdge (int fromNode, int toNode);

    sf::FloatRect getBoundary() const override;

    void handleMousePress (const sf::Vector2f &mousePos) override;
    void handleMouseRelease (const sf::Vector2f &mousePos) override;
    void handleMouseMovement (const sf::Vector2f &mousePos) override;
    void handleTextEntered (const char &unicode) override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}; // namespace UI