#include "../../include/scenes/dijkstra-scene.hpp"

DijkstraScene::DijkstraScene (const sf::RenderWindow &window) : Scene(window) {
    graph.setPosition({
        Setting::screenWidth / 2.f,
        Setting::screenHeight / 2.f
    });

    for (int i = 0; i < 7; i++) graph.insertNode(i);
    graph.insertEdge(0, 2);
    graph.insertEdge(0, 3);
    graph.insertEdge(0, 5);
    graph.insertEdge(1, 4);
    graph.insertEdge(1, 6);
    graph.insertEdge(2, 3);
    graph.insertEdge(2, 6);
    graph.insertEdge(3, 5);
    graph.insertEdge(5, 6);
}

void DijkstraScene::handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    graph.handleMouseEvents(window, event);
}

void DijkstraScene::timePropagation (float delta) {
    graph.timePropagation(delta);
}

void DijkstraScene::draw (sf::RenderWindow &window) {
    window.draw(graph);
}