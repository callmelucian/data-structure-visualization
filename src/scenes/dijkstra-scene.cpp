#include "../../include/scenes/dijkstra-scene.hpp"

DijkstraScene::DijkstraScene (SceneManager &manager) :
    Scene(manager, 6, 2), canEdit(false), canDelete(false), fastPropagation(false) {
    // set callback functions for button-UI communications
    ui.getCurrentUI().setCallbackAllowEdit([&] (bool f) {
        if (f) buttons[0].enableButton();
        else buttons[0].disableButton(), fields[0].disable();
        canEdit = f;
    });
    ui.getCurrentUI().setCallbackAllowDelete([&] (bool f) {
        if (f) buttons[1].enableButton();
        else buttons[1].disableButton();
        canDelete = f;
    });
    ui.getCurrentUI().setCallbackIsEditing([&] (const sf::Vector2f &mousePos) {
        for (int i = 0; i < 2; i++)
            if (buttons[i].containPosition(mousePos)) return true;
        return fields[0].containPosition(mousePos);
    });

    // make edges directed
    ui.getCurrentUI().makeDirected();
    ui.getCurrentLogic().makeDirected();

    // set callback functions: Graph UI
    ui.setCallbackEnableButtons([&](int f) {
        for (UI::Button &button : buttons) {
            if (f) button.enableButton();
            else button.disableButton();
        }
        if (!canEdit) buttons[0].disableButton();
        if (!canDelete) buttons[1].disableButton();
        if (!f) disableFields();
    });
    ui.setCallbackPlayPause(changePlayButton);
    disableFields();

    // edit
    buttons[0].setString("EDIT", 20);
    buttons[0].setCallback([&]() {
        if (fields[0].isEnabled()) fields[0].disable();
        else disableFields(), fields[0].enable();
    });
    fields[0].setLabel("Enter new edge weight");
    fields[0].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.size() != 1) return;
        ui.transformLogic([&] (DS::DijkstraAlgorithm &g) {
            return g.editEdge(ui.getCurrentUI().edgeActivated(), numbers[0]);
        });
    });

    // delete
    buttons[1].setString("DELETE", 20);
    buttons[1].setCallback([&]() {
        int nodeID = ui.getCurrentUI().nodeActivated();
        int edgeID = ui.getCurrentUI().edgeActivated();
        ui.getCurrentUI().resetNodeActivation();
        ui.getCurrentUI().resetEdgeActivation();
        if (nodeID != -1) ui.transformLogic([&] (DS::DijkstraAlgorithm &logic) {
            ui.getCurrentUI().highlightNode(-1);
            return logic.deleteNode(nodeID), true;
        }); 
        if (edgeID != -1) ui.transformLogic([&] (DS::DijkstraAlgorithm &logic) {
            ui.getCurrentUI().highlightEdge(-1);
            return logic.deleteEdge(edgeID), true;
        });
    });

    // create node
    buttons[2].setString("ADD NODE", 20);
    buttons[2].setCallback([&]() {
        ui.transformLogic([] (DS::DijkstraAlgorithm &g) {
            return g.createNode(), true;
        });
    });

    // run from
    buttons[3].setString("RUN", 20);
    buttons[3].setCallback([&]() {
        if (fields[1].isEnabled()) fields[1].disable();
        else disableFields(), fields[1].enable();
    });
    fields[1].setLabel("Enter source node(s)");
    fields[1].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.size() != 1) return; // will allow multicore Dijkstra if i have time
        ui.transformLogic([&] (DS::DijkstraAlgorithm &g) {
            return g.run(numbers[0]), true;
        });
    });

    // import file
    buttons[4].setString("IMPORT FILE");
    buttons[4].setCharacterSize(20);
    buttons[4].setCallback([&]() {
        std::string filePath = openFileDialog();
        std::vector<int> numbers = fileToNumbers(filePath);
        if (!checkValidGraph(numbers)) return;
        ui.appendEmpty(), ui.nextCompleteState();
        
        int n = numbers[0], m = numbers[1];
        for (int i = 0; i < n; i++) {
            ui.transformLogic([&] (DS::DijkstraAlgorithm &g) {
                return g.createNode(), true;
            });
            ui.nextCompleteState();
        }
        for (int i = 0; i < m; i++) {
            int a = numbers[2 + 3 * i], b = numbers[3 + 3 * i], w = numbers[4 + 3 * i];
            ui.transformLogic([&] (DS::DijkstraAlgorithm &g) {
                return g.createEdge(a, b, w), true;
            });
            ui.nextCompleteState();
        }
        ui.getCurrentUI().setOrigin({Setting::focusX / 2.f, Setting::focusY / 2.f});
    });

    // clear
    buttons[5].setCallback([&]() {
        ui.appendEmpty(), ui.nextCompleteState();
    });
    buttons[5].setString("CLEAR", 20);

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
    playButton.setCallback([&]() {
        if (ui.checkIsPlaying()) ui.pause();
        else ui.play();
    });
}

void DijkstraScene::handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    ui.getCurrentCode().handleMouseEvents(window, event);
    ui.getCurrentUI().handleMouseEvents(window, event);
    baseHandleEvent(window, event);
}

void DijkstraScene::timePropagation (float delta) {
    baseTimePropagation(delta);
    ui.timePropagation(delta);
}

void DijkstraScene::draw (sf::RenderWindow &window) {
    baseDraw(window);
    window.draw(ui.getCurrentUI());
    window.draw(ui.getCurrentCode());
}