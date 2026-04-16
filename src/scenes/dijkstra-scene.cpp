#include "../../include/scenes/dijkstra-scene.hpp"

DijkstraScene::DijkstraScene (const sf::RenderWindow &window) :
    Scene(window), editField(150, 30),
    insertButton(100, 30), editButton(100, 30), deleteButton(100, 30),
    ui(UI::Graph()) {
    
    // setup button: editing edges' weight
    editField.setPosition({115, 850});
    editButton.setPosition({240, 850});
    editButton.setString("Edit weight");
    editButton.disableButton();
    editField.disable();

    // setup button: insert node
    insertButton.setPosition({400, 850});
    insertButton.setString("Add node");

    // setup button: delete nodes/edges
    deleteButton.setPosition({550, 850});
    deleteButton.setString("Delete");
    deleteButton.disableButton();

    // set callback functions for button-UI communications
    ui.getCurrentUI().setCallbackAllowEdit([&] (bool f) {
        if (f) editButton.enableButton(), editField.enable();
        else editButton.disableButton(), editField.disable();
    });
    ui.getCurrentUI().setCallbackAllowDelete([&] (bool f) {
        if (f) deleteButton.enableButton();
        else deleteButton.disableButton();
    });
    ui.getCurrentUI().setCallbackIsEditing([&] (const sf::Vector2f &mousePos) {
        return editField.containPosition(mousePos) || editButton.containPosition(mousePos) || deleteButton.containPosition(mousePos);
    });

    // set callback functions: Graph UI
    ui.setCallbackEnableButtons([&](int f) {
        if (f) insertButton.enableButton();
        else {
            insertButton.disableButton();
            editField.disable();
            editButton.disableButton();
            deleteButton.disableButton();
        }
    });

    // set callback function: editing edges' weight
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
        ui.transformLogic([&] (DS::DijkstraAlgorithm &logic) {
            return logic.editEdge(ui.getCurrentUI().edgeActivated(), value);
        });
    });
    editButton.setCallback([&]() {
        editField.releaseText();
    });


    // set callback functions: insert node
    insertButton.setCallback([&]() {
        ui.transformLogic([] (DS::DijkstraAlgorithm &logic) {
            return logic.createNode(), true;
        });
    });

    // set callback functions: delete nodes/edges
    deleteButton.setCallback([&]() {
        int nodeID = ui.getCurrentUI().nodeActivated();
        int edgeID = ui.getCurrentUI().edgeActivated();
        if (nodeID != -1) ui.transformLogic([&] (DS::DijkstraAlgorithm &logic) {
            return logic.deleteNode(nodeID), true;
        });
        if (edgeID != -1) ui.transformLogic([&] (DS::DijkstraAlgorithm &logic) {
            return logic.deleteEdge(edgeID), true;
        });
    });
}

void DijkstraScene::handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    ui.getCurrentUI().handleMouseEvents(window, event);
    editField.handleMouseEvents(window, event);
    insertButton.handleMouseEvents(window, event);
    editButton.handleMouseEvents(window, event);
    deleteButton.handleMouseEvents(window, event);

    editField.handleTextEvents(window, event);
    // std::cerr << "Very done" << std::endl;
}

void DijkstraScene::timePropagation (float delta) {
    ui.timePropagation(delta);
    editField.timePropagation();
}

void DijkstraScene::draw (sf::RenderWindow &window) {
    window.draw(ui.getCurrentUI());
    window.draw(editField);
    window.draw(insertButton);
    window.draw(editButton);
    window.draw(deleteButton);
}