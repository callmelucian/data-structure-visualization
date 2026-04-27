#include "../../include/scenes/hash-map-scene.hpp"

HashMapScene::HashMapScene(SceneManager &manager) : Scene(manager, 7, 5), updateHold(0) {
    // set callback functions: Hash Map UI
    ui.setCallbackEnableButtons([&](int f) {
        for (UI::Button &button : buttons) {
            if (f) button.enableButton();
            else button.disableButton();
        }
        if (!f) disableFields();
        buttons.back().enableButton();
    });
    ui.setCallbackPlayPause(changePlayButton);
    disableFields();

    // insert button
    buttons[0].setString("INSERT", 20);
    buttons[0].setCallback([&]() {
        if (fields[0].isEnabled()) fields[0].disable();
        else disableFields(), fields[0].enable(), fields[0].focusField();
    });
    fields[0].setLabel("Enter value to be inserted");
    fields[0].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        for (int value : numbers) {
            ui.transformLogic([&] (DS::HashMap &hm) {
                return hm.insert(value), true;
            });
        }
    });

    // erase button
    buttons[1].setString("ERASE", 20);
    buttons[1].setCallback([&]() {
        if (fields[1].isEnabled()) fields[1].disable();
        else disableFields(), fields[1].enable(), fields[1].focusField();
    });
    fields[1].setLabel("Enter value to be erased");
    fields[1].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        for (int value : numbers) {
            ui.transformLogic([&] (DS::HashMap &hm) {
                return hm.erase(value), true;
            });
        }
    });

    // update button
    buttons[2].setString("UPDATE", 20);
    buttons[2].setCallback([&]() {
        if (fields[2].isEnabled() || fields[3].isEnabled()) disableFields();
        else disableFields(), fields[2].enable(), fields[2].focusField();
    });
    fields[2].setLabel("Enter value to be replaced");
    fields[2].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.size() != 1) return;
        updateHold = numbers[0];
        fields[2].disable(), fields[3].enable(), fields[3].focusField();
    });
    fields[3].setLabel("Enter value to replace");
    fields[3].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.size() != 1) return;
        ui.transformLogic([&] (DS::HashMap &hm) {
            return hm.update(updateHold, numbers[0]), true;
        });
        fields[3].disable(), fields[2].enable(), fields[2].focusField();
    });

    // search button
    buttons[3].setString("SEARCH", 20);
    buttons[3].setCallback([&]() {
        if (fields[4].isEnabled()) fields[4].disable();
        else disableFields(), fields[4].enable(), fields[4].focusField();
    });
    fields[4].setLabel("Enter value to search for");
    fields[4].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        for (int value : numbers) {
            ui.transformLogic([&] (DS::HashMap &hm) {
                return hm.find(value), true;
            });
        }
    });

    // import file button
    buttons[4].setString("IMPORT FILE", 20);
    buttons[4].setCallback([&]() {
        std::string filePath = openFileDialog();
        std::vector<int> numbers = fileToNumbers(filePath);
        if (numbers.empty()) return;
        ui.appendEmpty(), ui.nextCompleteState();
        for (int value : numbers) {
            ui.transformLogic([&] (DS::HashMap &hm) {
                return hm.insert(value), true;
            });
            ui.nextCompleteState();
        }
    });

    // clear UI
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

void HashMapScene::handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    baseHandleEvent(window, event);
    ui.getCurrentCode().handleMouseEvents(window, event);
}

void HashMapScene::timePropagation(float delta) {
    baseTimePropagation(delta);
    ui.timePropagation(delta);
}

void HashMapScene::draw(sf::RenderWindow &window) {
    window.draw(ui.getCurrentCode());
    window.draw(ui.getCurrentUI());
    baseDraw(window);
}