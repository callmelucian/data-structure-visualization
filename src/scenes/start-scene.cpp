#include "../../include/scenes/start-scene.hpp"

StartScene::StartScene (SceneManager &manager) :
    Scene(manager),
    toAVL(300.f, 70.f, 10.f),
    toDijkstra(300.f, 70.f, 10.f),
    toHashMap(300.f, 70.f, 10.f),
    toLinkedList(300.f, 70.f, 10.f),
    toPrim(300.f, 70.f, 10.f),
    toRBTree(300.f, 70.f, 10.f) {

    toAVL.setString("AVL TREE");
    toAVL.setCharacterSize(30);
    toAVL.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f - 250});
    toAVL.setCallback([&]() { manager.changeScene(1); });
    // toAVL.disableButton();

    toDijkstra.setString("RB TREE");
    toDijkstra.setCharacterSize(30);
    toDijkstra.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f - 150});
    toDijkstra.setCallback([&]() { manager.changeScene(2); });
    // toDijkstra.disableButton();

    toHashMap.setString("HASH MAP");
    toHashMap.setCharacterSize(30);
    toHashMap.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f - 50});
    toHashMap.setCallback([&]() { manager.changeScene(3); });
    toHashMap.disableButton();

    toLinkedList.setString("LINKED LIST");
    toLinkedList.setCharacterSize(30);
    toLinkedList.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f + 50});
    toLinkedList.setCallback([&]() { manager.changeScene(4); });
    toLinkedList.disableButton();

    toPrim.setString("MST (PRIM)");
    toPrim.setCharacterSize(30);
    toPrim.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f + 150});
    toPrim.setCallback([&]() { manager.changeScene(5); });
    toPrim.disableButton();

    toRBTree.setString("RB TREE");
    toRBTree.setCharacterSize(30);
    toRBTree.setPosition({Setting::screenWidth / 2.f, Setting::screenHeight / 2.f + 250});
    toRBTree.setCallback([&]() { manager.changeScene(6); });
    toRBTree.disableButton();
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