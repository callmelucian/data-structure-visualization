#include "../../include/scenes/rb-tree-scene.hpp"

RBTreeScene::RBTreeScene (SceneManager &manager) : Scene(manager, 7, 5), updateHold(0) {
    // set callback functions: AVL Tree UI
    ui.setCallbackEnableButtons([&](int f) {
        for (UI::Button &button : buttons) {
            if (f) button.enableButton();
            else button.disableButton();
        }
        if (!f) disableFields();
    });
    ui.setCallbackPlayPause(changePlayButton);
    disableFields();

    // insert button
    buttons[0].setString("INSERT");
    buttons[0].setCharacterSize(20);
    fields[0].setLabel("Insert/Init");
    fields[0].setCallbackFunction([&](const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        for (int value : numbers) {
            ui.transformLogic([&](DS::RedBlackTree &rb) {
                return rb.insert(value), true;
            });
        }
    });
    buttons[0].setCallback([&]() {
        if (fields[0].isEnabled()) fields[0].disable();
        else disableFields(), fields[0].enable();
    });

    // erase button
    buttons[1].setString("ERASE");
    buttons[1].setCharacterSize(20);
    fields[1].setLabel("Erase");
    fields[1].setCallbackFunction([&](const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        for (int value : numbers) {
            ui.transformLogic([&](DS::RedBlackTree &rb) {
                return rb.erase(value);
            });
        }
    });
    buttons[1].setCallback([&]() {
        if (fields[1].isEnabled()) fields[1].disable();
        else disableFields(), fields[1].enable();
    });

    // search button
    buttons[2].setString("SEARCH");
    buttons[2].setCharacterSize(20);
    fields[2].setLabel("Search");
    fields[2].setCallbackFunction([&](const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.empty() || numbers.size() >= 2) return;
        for (int value : numbers) {
            ui.transformLogic([&](DS::RedBlackTree &rb) {
                return rb.search(value);
            });
        }
    });
    buttons[2].setCallback([&]() {
        if (fields[2].isEnabled()) fields[2].disable();
        else disableFields(), fields[2].enable();
    });

        // update button
    buttons[3].setString("UPDATE", 20);
    buttons[3].setCallback([&]() {
        if (fields[3].isEnabled() || fields[4].isEnabled()) disableFields();
        else disableFields(), fields[3].enable();
    });
    fields[3].setLabel("Enter value to be replaced");
    fields[3].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.size() != 1) return;
        updateHold = numbers[0];
        fields[3].disable(), fields[4].enable(), fields[4].focusField();
    });
    fields[4].setLabel("Enter value to replace");
    fields[4].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.size() != 1) return;
        ui.transformLogic([&] (DS::RedBlackTree &rb) {
            return rb.update(updateHold, numbers[0]);
        });
        fields[4].disable(), fields[3].enable(), fields[3].focusField();
    });

    // import file
    buttons[4].setString("IMPORT FILE", 20);
    buttons[4].setCallback([&]() {
        std::string filePath = openFileDialog();
        std::vector<int> numbers = fileToNumbers(filePath);
        if (numbers.empty()) return;
        ui.appendEmpty(), ui.nextCompleteState();
        for (int value : numbers) {
            ui.transformLogic([&](DS::RedBlackTree &rb) {
                return rb.insert(value), true;
            });
            ui.nextCompleteState();
        }
    });

    // clear
    buttons[5].setString("CLEAR", 20);
    buttons[5].setCallback([&]() {
        ui.appendEmpty(), ui.nextCompleteState();
    });

    // code
    buttons[6].setString("SHOW CODE", 20);
    buttons[6].setCallback([&]() {
        if (ui.getCurrentCode().isShown()) {
            ui.getCurrentCode().hide(), ui.centerUI();
            buttons[6].setString("SHOW CODE", 20);
        }
        else {
            ui.getCurrentCode().show(), ui.offcenterUI();
            buttons[6].setString("HIDE CODE", 20);
        }
    });

    // set callback functions: previous/next buttons
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

void RBTreeScene::handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    ui.getCurrentCode().handleMouseEvents(window, event);
    baseHandleEvent(window, event);
}

void RBTreeScene::timePropagation(float delta) {
    baseTimePropagation(delta);
    ui.timePropagation(delta);
}

void RBTreeScene::draw(sf::RenderWindow &window) {
    window.draw(ui.getCurrentCode());
    window.draw(ui.getCurrentUI());
    baseDraw(window);
}