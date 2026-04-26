#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

// utilities
#include "scene-manager.hpp"
#include "../core/global-setting.hpp"
#include "../core/utility.hpp"
#include "../ds/dijkstra.hpp"
#include "../animation/animation-manager.hpp"
#include "../animation/events.hpp"

// UI components
#include "../components/button.hpp"
#include "../components/graph.hpp"
#include "../components/input-field.hpp"
#include "../components/slider.hpp"

class DijkstraScene : public Scene {
private:
    AnimationManager<UI::Graph, DS::DijkstraAlgorithm> ui;
    bool canEdit, canDelete;

public:
    DijkstraScene (SceneManager &manager);

    void handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) override;
    void timePropagation(float delta) override;
    void draw(sf::RenderWindow &window) override;
};