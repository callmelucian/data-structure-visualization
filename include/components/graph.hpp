#include <SFML/Graphics.hpp>
#include <vector>
#include <tuple>
#include <set>

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

class Graph : public UI::Base {
private:
    std::vector<UI::FloatingNode*> nodes;
    std::set<std::pair<int,int>> edges;
    UI::HighlightCircle highlighter;
public:
    Graph();
    Graph (const Graph &o);
    ~Graph();

    Graph& operator= (const Graph &o);

    void timePropagation (float deltaTime);
};