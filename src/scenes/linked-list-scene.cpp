#include "../../include/scenes/linked-list-scene.hpp"

LinkedListScene::LinkedListScene(SceneManager &manager) : Scene(manager, 8, 7), insertHold(0), updateHold(0) {
    // animation manager
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

    // append button
    buttons[0].setString("APPEND", 20);
    fields[0].setLabel("Enter value of new node");
    fields[0].setCallbackFunction([&](const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        for (int value : numbers) {
            ui.transformLogic([&](DS::LinkedList &ll) {
                return ll.append(value), true;
            });
        }
    });
    buttons[0].setCallback([&]() {
        if (fields[0].isEnabled()) fields[0].disable();
        else disableFields(), fields[0].enable(), fields[0].focusField();
    });

    // insert button
    buttons[1].setString("INSERT", 20);
    fields[1].setLabel("Enter position to be inserted (0-indexed)");
    fields[1].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.size() != 1) return;
        insertHold = numbers[0];
        fields[1].disable(), fields[2].enable(), fields[2].focusField();
    });
    fields[2].setLabel("Enter value of new node");
    fields[2].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.size() != 1) return;
        ui.transformLogic([&] (DS::LinkedList &ll) {
            return ll.insert(insertHold, numbers[0]);
        });
        fields[2].disable(), fields[1].enable(), fields[1].focusField();
    });
    buttons[1].setCallback([&]() {
        if (fields[1].isEnabled() || fields[2].isEnabled())
            disableFields(), insertHold = false;
        else disableFields(), fields[1].enable();
    });

    // erase button
    buttons[2].setString("ERASE", 20);
    fields[3].setLabel("Enter value of to erase");
    fields[3].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        for (int value : numbers) {
            ui.transformLogic([&] (DS::LinkedList &ll) {
                return ll.erase(value), true;
            });
        }
    });
    buttons[2].setCallback([&]() {
        if (fields[3].isEnabled()) fields[3].disable();
        else disableFields(), fields[3].enable(), fields[3].focusField();
    });

    // update button
    buttons[3].setString("UPDATE", 20);
    fields[4].setLabel("Enter value to be replaced");
    fields[4].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.size() != 1) return;
        updateHold = numbers[0];
        fields[4].disable(), fields[5].enable(), fields[5].focusField();
    });
    fields[5].setLabel("Enter value to replace");
    fields[5].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.size() != 1) return;
        ui.transformLogic([&] (DS::LinkedList &ll) {
            return ll.update(updateHold, numbers[0]);
        });
        fields[5].disable(), fields[4].enable(), fields[4].focusField();
    });
    buttons[3].setCallback([&]() {
        if (fields[4].isEnabled() || fields[5].isEnabled()) disableFields();
        else disableFields(), fields[4].enable(), fields[4].focusField();
    });

    // update search
    buttons[4].setString("SEARCH", 20);
    fields[6].setLabel("Enter value to search for");
    fields[6].setCallbackFunction([&] (const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        for (int value : numbers) {
            ui.transformLogic([&] (DS::LinkedList &ll) {
                return ll.search(value);
            });
        }
    });
    buttons[4].setCallback([&]() {
        if (fields[6].isEnabled()) fields[6].disable();
        else disableFields(), fields[6].enable(), fields[6].focusField();
    });

    // import file
    buttons[5].setString("IMPORT FILE", 20);
    buttons[5].setCallback([&]() {
        std::string filePath = openFileDialog();
        std::vector<int> numbers = fileToNumbers(filePath);
        if (numbers.empty()) return;
        ui.appendEmpty(), ui.nextCompleteState();
        for (int value : numbers) {
            ui.transformLogic([&] (DS::LinkedList &ll) {
                return ll.append(value), true;
            });
            ui.nextCompleteState();
        }
    });

    // clear UI
    buttons[6].setString("CLEAR", 20);
    buttons[6].setCallback([&]() {
        ui.appendEmpty(), ui.nextCompleteState();
    });

    // code
    buttons[7].setString("SHOW CODE", 20);
    buttons[7].setCallback([&]() {
        if (ui.getCurrentCode().isShown()) {
            ui.getCurrentCode().hide(), ui.centerUI();
            buttons[7].setString("SHOW CODE", 20);
        }
        else {
            ui.getCurrentCode().show(), ui.offcenterUI();
            buttons[7].setString("HIDE CODE", 20);
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

void LinkedListScene::handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    ui.getCurrentCode().handleMouseEvents(window, event);
    baseHandleEvent(window, event);
}

void LinkedListScene::timePropagation(float delta) {
    baseTimePropagation(delta);
    ui.timePropagation(delta);
}

void LinkedListScene::draw(sf::RenderWindow &window) {
    ui.getCurrentCode().changeColor();
    window.draw(ui.getCurrentCode());
    ui.getCurrentUI().changeColor();
    window.draw(ui.getCurrentUI());
    baseDraw(window);
}