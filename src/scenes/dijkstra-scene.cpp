#include "../../include/scenes/dijkstra-scene.hpp"

DijkstraScene::DijkstraScene (const sf::RenderWindow &window) :
    Scene(window), editField(150, 30), runField(150, 30),
    insertButton(100, 30), editButton(100, 30), deleteButton(100, 30), runButton(100, 30),
    ui(UI::Graph()),
    prevStepButton(50, 30), prevOperationButton(50, 30),
    nextStepButton(50, 30), nextOperationButton(50, 30) {
    
    // setup button: editing edges' weight
    editField.setPosition({115, 850});
    editButton.setPosition({240, 850});
    editButton.setString("Edit weight");
    editButton.disableButton();
    editField.disable();

    // setup buttons: previous and next states
    prevStepButton.setString("<");
    prevOperationButton.setString("<<");
    nextStepButton.setString(">");
    nextOperationButton.setString(">>");

    prevOperationButton.setPosition({950, 850});
    prevStepButton.setPosition({1010, 850});
    nextStepButton.setPosition({1070, 850});
    nextOperationButton.setPosition({1130, 850});

    // setup button: insert node
    insertButton.setPosition({400, 850});
    insertButton.setString("Add node");

    // setup button: delete nodes/edges
    deleteButton.setPosition({550, 850});
    deleteButton.setString("Delete");
    deleteButton.disableButton();

    // run button: run Dijkstra
    runField.setPosition({725, 850});
    runButton.setPosition({850, 850});
    runButton.setString("Run from");

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
            ui.getCurrentUI().highlightNode(-1);
            return logic.deleteNode(nodeID), true;
        });
        if (edgeID != -1) ui.transformLogic([&] (DS::DijkstraAlgorithm &logic) {
            ui.getCurrentUI().highlightEdge(-1);
            return logic.deleteEdge(edgeID), true;
        });
    });

    // set callback functions: run Dijkstra
    runField.setCallbackFunction([&] (const std::string &msg) {
        int value = convert(msg);
        if (value == -1) {
            std::cerr << "Edit field contains non-numerical value!" << std::endl;
            return;
        }
        ui.transformLogic([value] (DS::DijkstraAlgorithm &logic) {
            return logic.run(value), true;
        });
    });
    runButton.setCallback([&]() {
        runField.releaseText();
    });

    // set callback functions: previous and next states
    prevOperationButton.setCallback([&]() {
        ui.previousCompleteState();
    });
    prevStepButton.setCallback([&]() {
        ui.previousState();
    });
    nextOperationButton.setCallback([&]() {
        ui.nextCompleteState();
    });
    nextStepButton.setCallback([&]() {
        ui.nextState();
    });
}

void DijkstraScene::handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    ui.getCurrentUI().handleMouseEvents(window, event);
    editField.handleMouseEvents(window, event);
    insertButton.handleMouseEvents(window, event);
    editButton.handleMouseEvents(window, event);
    deleteButton.handleMouseEvents(window, event);
    editField.handleMouseEvents(window, event);
    runButton.handleMouseEvents(window, event);
    runField.handleMouseEvents(window, event);

    editField.handleTextEvents(window, event);
    runField.handleTextEvents(window, event);
    prevOperationButton.handleMouseEvents(window, event);
    prevStepButton.handleMouseEvents(window, event);
    nextOperationButton.handleMouseEvents(window, event);
    nextStepButton.handleMouseEvents(window, event);
}

void DijkstraScene::timePropagation (float delta) {
    ui.timePropagation(delta);
    editField.timePropagation();
}

void DijkstraScene::draw (sf::RenderWindow &window) {
    window.draw(ui.getCurrentUI());
    window.draw(ui.getCurrentCode());
    window.draw(editField);
    window.draw(runField);
    window.draw(insertButton);
    window.draw(editButton);
    window.draw(deleteButton);
    window.draw(prevOperationButton);
    window.draw(prevStepButton);
    window.draw(nextOperationButton);
    window.draw(nextStepButton);
    window.draw(runButton);
}