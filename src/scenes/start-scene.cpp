#include "../../include/scenes/start-scene.hpp"

StartScene::StartScene (const sf::RenderWindow &window, SceneManager &manager) :
    Scene(window, manager), toAVL(300, 70), toDijkstra(300, 70), toHashMap(300, 70), toLinkedList(300, 70), toPrim(300, 70), toRBTree(300, 70) {

    toAVL.setString("AVL TREE");
    toAVL.setCharacterSize(30);
    toAVL.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f - 250});
    toAVL.setCallback([&]() { manager.changeScene(1); });

    toDijkstra.setString("DIJKSTRA");
    toDijkstra.setCharacterSize(30);
    toDijkstra.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f - 150});
    toDijkstra.setCallback([&]() { manager.changeScene(2); });

    toHashMap.setString("HASH MAP");
    toHashMap.setCharacterSize(30);
    toHashMap.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f - 50});
    toHashMap.setCallback([&]() { manager.changeScene(3); });

    toLinkedList.setString("LINKED LIST");
    toLinkedList.setCharacterSize(30);
    toLinkedList.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f + 50});
    toLinkedList.setCallback([&]() { manager.changeScene(4); });

    toPrim.setString("AVL TREE");
    toPrim.setCharacterSize(30);
    toPrim.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f + 150});
    toPrim.setCallback([&]() { manager.changeScene(5); });

    toRBTree.setString("RB TREE");
    toRBTree.setCharacterSize(30);
    toRBTree.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f + 250});
    toRBTree.setCallback([&]() { manager.changeScene(6); });
}

void StartScene::handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    toAVL.handleMouseEvents(window, event);
    toDijkstra.handleMouseEvents(window, event);
    toHashMap.handleMouseEvents(window, event);
    toLinkedList.handleMouseEvents(window, event);
    toPrim.handleMouseEvents(window, event);
    toRBTree.handleMouseEvents(window, event);
}

void StartScene::timePropagation (float delta) {}

void StartScene::draw (sf::RenderWindow &window) {
    window.draw(toAVL);
    window.draw(toDijkstra);
    window.draw(toHashMap);
    window.draw(toLinkedList);
    window.draw(toPrim);
    window.draw(toRBTree);
}