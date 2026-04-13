#include "../../include/scenes/dijkstra-scene.hpp"

DijkstraScene::DijkstraScene (const sf::RenderWindow &window) : Scene(window), editField(150, 30), insertButton(100, 30), editButton(100, 30) {
    graph.setPosition({
        Setting::screenWidth / 2.f,
        Setting::screenHeight / 2.f
    });
    graph.setCallbackAllowEdit([&] (bool f) {
        if (f) editButton.enableButton();
        else editButton.disableButton();
    });
    graph.setCallbackIsEditing([&] (const sf::Vector2f &mousePos) {
        return editField.containPosition(mousePos) || editButton.containPosition(mousePos);
    });

    editField.setPosition({115, 850});
    editButton.setPosition({240, 850});
    editButton.setString("Edit weight");
    editButton.disableButton();
    editField.setCallbackFunction([&] (const std::string &msg) {
        int value = convert(msg);
        if (value == -1) {
            std::cerr << "Edit field contains non-numerical value!" << std::endl;
            return;
        }
        if (value < 0 || value >= 1000) {
            std::cerr << "Out of bounds" << std::endl;
            return;
        }
        graph.changeWeight(value);
    });
    editButton.setCallback([&]() {
        editField.releaseText();
    });

    insertButton.setPosition({400, 850});
    insertButton.setString("Add node");
    insertButton.setCallback([&]() {
        graph.insertNode(counter);
        counter++;
    });
}

void DijkstraScene::handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    graph.handleMouseEvents(window, event);
    editField.handleMouseEvents(window, event);
    insertButton.handleMouseEvents(window, event);
    editButton.handleMouseEvents(window, event);

    editField.handleTextEvents(window, event);
}

void DijkstraScene::timePropagation (float delta) {
    graph.timePropagation(delta);
    editField.timePropagation();
}

void DijkstraScene::draw (sf::RenderWindow &window) {
    window.draw(graph);
    window.draw(editField);
    window.draw(insertButton);
    window.draw(editButton);
}