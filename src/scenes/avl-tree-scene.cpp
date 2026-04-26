#include "../../include/scenes/avl-tree-scene.hpp"

AVLTreeScene::AVLTreeScene (SceneManager &manager) : Scene(manager, 6, 3) {
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
    buttons[0].setString("INSERT", 20);
    fields[0].setCallbackFunction([&](const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        for (int value : numbers) {
            ui.transformLogic([&](DS::AVLTree &avl) {
                return avl.insert(value), true;
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
    fields[1].setCallbackFunction([&](const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        for (int value : numbers) {
            ui.transformLogic([&](DS::AVLTree &avl) {
                return avl.erase(value);
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
    fields[2].setCallbackFunction([&](const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.empty() || numbers.size() >= 2) return;
        for (int value : numbers) {
            ui.transformLogic([&](DS::AVLTree &avl) {
                return avl.search(value);
            });
        }
    });
    buttons[2].setCallback([&]() {
        if (fields[2].isEnabled()) fields[2].disable();
        else disableFields(), fields[2].enable();
    });

    // import file
    buttons[3].setString("IMPORT FILE");
    buttons[3].setCharacterSize(20);
    buttons[3].setCallback([&]() {
        std::string filePath = openFileDialog();
        std::vector<int> numbers = fileToNumbers(filePath);
        if (numbers.empty()) return;
        ui.appendEmpty(), ui.nextCompleteState();
        for (int value : numbers) {
            ui.transformLogic([&](DS::AVLTree &avl) {
                return avl.insert(value), true;
            });
            ui.nextCompleteState();
        }
    });

    // clear
    buttons[4].setCallback([&]() {
        ui.appendEmpty(), ui.nextCompleteState();
    });
    buttons[4].setString("CLEAR");
    buttons[4].setCharacterSize(20);

    // code
    buttons[5].setString("SHOW CODE", 20);
    buttons[5].setCallback([&]() {
        if (ui.getCurrentCode().isShown())
            ui.getCurrentCode().hide(), ui.centerUI();
        else ui.getCurrentCode().show(), ui.offcenterUI();
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

void AVLTreeScene::handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    baseHandleEvent(window, event);
    ui.getCurrentCode().handleMouseEvents(window, event);
}

void AVLTreeScene::timePropagation(float delta) {
    baseTimePropagation(delta);
    ui.timePropagation(delta);
}

void AVLTreeScene::draw(sf::RenderWindow &window) {
    window.draw(ui.getCurrentCode());
    window.draw(ui.getCurrentUI());
    baseDraw(window);
}