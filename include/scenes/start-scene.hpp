#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>

// utilities
#include "scene-manager.hpp"
#include "avl-tree-scene.hpp"
#include "dijkstra-scene.hpp"
#include "hash-map-scene.hpp"
#include "linked-list-scene.hpp"
#include "prim-scene.hpp"
#include "rb-tree-scene.hpp"

// UI components
#include "../components/button.hpp"

extern const std::vector<std::string> sceneName;

class StartScene : public Scene {
private:
    // UI::Button toAVL, toDijkstra, toHashMap, toLinkedList, toPrim, toRBTree;
    std::vector<UI::Button> navigation;
    UI::Text title, subtitle;

public:
    StartScene (SceneManager &manager);
    
    void handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) override;
    void timePropagation(float delta) override;
    void draw(sf::RenderWindow &window) override;
};