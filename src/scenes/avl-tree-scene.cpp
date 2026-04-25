#include "../../include/scenes/avl-tree-scene.hpp"

AVLTreeScene::AVLTreeScene (SceneManager &manager) :
    Scene(manager, 5),
    insertField(1000, 40), eraseField(1000, 40), searchField(1000, 40) {
    
    // initialize buttons
    featureButtons[0].setString("INSERT");
    featureButtons[0].setCharacterSize(20);
    featureButtons[1].setString("ERASE");
    featureButtons[1].setCharacterSize(20);
    featureButtons[2].setString("SEARCH");
    featureButtons[2].setCharacterSize(20);
    featureButtons[3].setString("FILE");
    featureButtons[3].setCharacterSize(20);
    featureButtons[4].setString("CLEAR");
    featureButtons[4].setCharacterSize(20);

    // intialize fields
    insertField.setPosition({Setting::screenWidth / 2.f, 85});
    insertField.setLabel("Insert/Init");
    insertField.disable();
    eraseField.setPosition({Setting::screenWidth / 2.f, 85});
    eraseField.setLabel("Erase");
    eraseField.disable();
    searchField.setPosition({Setting::screenWidth / 2.f, 85});
    searchField.setLabel("Search");
    searchField.disable();
    
    // set callback functions: AVL Tree UI
    ui.setCallbackEnableButtons([&](int f) {
        for (UI::Button &button : featureButtons) {
            if (f) button.enableButton();
            else button.disableButton();
        }
        if (!f) {
            insertField.disable();
            eraseField.disable();
            searchField.disable();
        }
    });
    ui.setCallbackPlayPause(changePlayButton);

    // set callback functions: INSERT
    insertField.setCallbackFunction([&](const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        for (int value : numbers) {
            ui.transformLogic([&](DS::AVLTree &avl) {
                return avl.insert(value), true;
            });
        }
    });
    featureButtons[0].setCallback([&]() {
        if (insertField.isEnabled()) insertField.disable();
        else insertField.enable(), eraseField.disable(), searchField.disable();
    });

    // set callback functions: ERASE
    eraseField.setCallbackFunction([&](const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        for (int value : numbers) {
            ui.transformLogic([&](DS::AVLTree &avl) {
                return avl.erase(value);
            });
        }
    });
    featureButtons[1].setCallback([&]() {
        if (eraseField.isEnabled()) eraseField.disable();
        else eraseField.enable(), insertField.disable(), searchField.disable();
    });

    // set callback functions: SEARCH
    searchField.setCallbackFunction([&](const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.empty() || numbers.size() >= 2) return;
        for (int value : numbers) {
            ui.transformLogic([&](DS::AVLTree &avl) {
                return avl.search(value);
            });
        }
    });
    featureButtons[2].setCallback([&]() {
        if (searchField.isEnabled()) searchField.disable();
        else searchField.enable(), insertField.disable(), eraseField.disable();
    });

    featureButtons[3].setCallback([&]() {
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

    featureButtons[4].setCallback([&]() {
        ui.appendEmpty(), ui.nextCompleteState();
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
    insertField.handleMouseEvents(window, event);
    insertField.handleTextEvents(window, event);
    eraseField.handleMouseEvents(window, event);
    eraseField.handleTextEvents(window, event);
    searchField.handleMouseEvents(window, event);
    searchField.handleTextEvents(window, event);
    handleButtons(window, event);
    ui.getCurrentCode().handleMouseEvents(window, event);
}

void AVLTreeScene::timePropagation(float delta) {
    insertField.timePropagation();
    eraseField.timePropagation();
    searchField.timePropagation();
    ui.timePropagation(delta);
}

void AVLTreeScene::draw(sf::RenderWindow &window) {
    window.draw(ui.getCurrentUI());
    window.draw(insertField);
    window.draw(eraseField);
    window.draw(searchField);
    drawButtons(window);
    window.draw(ui.getCurrentCode());
}