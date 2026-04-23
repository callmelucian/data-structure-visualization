#include "../../include/scenes/hash-map-scene.hpp"

HashMapScene::HashMapScene(const sf::RenderWindow &window, SceneManager &manager) :
    Scene(window, manager), backButton(100, 30), settingButton(100, 30),
    insertButton(100, 30), eraseButton(100, 30),
    insertField(150, 30), eraseField(150, 30),
    treeUI(UI::HashMap()), playButton(50, 30),
    prevStepButton(50, 30), prevOperationButton(50, 30),
    nextStepButton(50, 30), nextOperationButton(50, 30) {
    
    // intialize back button
    backButton.setString("BACK");
    backButton.setCharacterSize(20);
    backButton.setPosition({100, 40});
    backButton.setCallback([&]() {
        manager.changeScene(0);
    });

    // intialize setting button
    settingButton.setString("SETTING");
    settingButton.setCharacterSize(20);
    settingButton.setPosition({Setting::screenWidth - 100, 40});
    settingButton.setCallback([&]() {
        manager.changeScene(7);
    });

    // initialize buttons
    insertButton.setString("INSERT");
    insertButton.setCharacterSize(20);
    eraseButton.setString("ERASE");
    eraseButton.setCharacterSize(20);

    prevStepButton.setString("<");
    prevStepButton.setCharacterSize(25);
    prevOperationButton.setString("<<");
    prevOperationButton.setCharacterSize(25);
    playButton.setString("|>");
    playButton.setCharacterSize(25);
    nextStepButton.setString(">");
    nextStepButton.setCharacterSize(25);
    nextOperationButton.setString(">>");
    nextOperationButton.setCharacterSize(25);

    // set positions for objects
    insertButton.setPosition({240, 850});
    insertField.setPosition({115, 850});
    eraseButton.setPosition({525, 850});
    eraseField.setPosition({400, 850});
    
    prevOperationButton.setPosition({65, 800});
    prevStepButton.setPosition({125, 800});
    playButton.setPosition({185, 800});
    nextStepButton.setPosition({245, 800});
    nextOperationButton.setPosition({305, 800});

    // set callback functions: AVL Tree UI
    treeUI.setCallbackEnableButtons([&](int f) {
        if (f) {
            insertButton.enableButton();
            eraseButton.enableButton();
            insertField.enable();
            eraseField.enable();
        }
        else {
            insertButton.disableButton();
            eraseButton.disableButton();
            insertField.disable();
            eraseField.disable();
        }
    });
    treeUI.setCallbackPlayPause([&] (bool f) {
        playButton.setString(f ? "||" : "|>");
        playButton.setCharacterSize(25);
    });

    // set callback functions: input field and button for insertion
    insertField.setCallbackFunction([&](const std::string &msg) {
        int value = convert(msg);
        if (value == -1) {
            std::cerr << "Input field contains non-numerical value!" << std::endl;
            return;
        }
        treeUI.transformLogic([&](DS::HashMap &hm) {
            return hm.insert(value), true;
        });
    });
    insertButton.setCallback([&]() {
        insertField.releaseText();
    });

    // set callback functions: input field and button for deletion
    eraseField.setCallbackFunction([&](const std::string &msg) {
        int value = convert(msg);
        if (value == -1) {
            std::cerr << "Input field contains non-numerical value!" << std::endl;
            return;
        }
        treeUI.transformLogic([&](DS::HashMap &hm) {
            return hm.erase(value), true;
        });
    });
    eraseButton.setCallback([&]() {
        eraseField.releaseText();
    });

    // set callback functions: previous/next buttons
    prevOperationButton.setCallback([&]() {
        treeUI.previousCompleteState();
    });
    prevStepButton.setCallback([&]() {
        treeUI.previousState();
    });
    nextOperationButton.setCallback([&]() {
        treeUI.nextCompleteState();
    });
    nextStepButton.setCallback([&]() {
        treeUI.nextState();
    });
    playButton.setCallback([&]() {
        if (treeUI.checkIsPlaying()) treeUI.pause();
        else treeUI.play();
    });
}

void HashMapScene::handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    insertField.handleMouseEvents(window, event);
    insertField.handleTextEvents(window, event);
    insertButton.handleMouseEvents(window, event);
    treeUI.getCurrentCode().handleMouseEvents(window, event);

    eraseField.handleMouseEvents(window, event);
    eraseField.handleTextEvents(window, event);
    eraseButton.handleMouseEvents(window, event);

    prevOperationButton.handleMouseEvents(window, event);
    prevStepButton.handleMouseEvents(window, event);
    nextOperationButton.handleMouseEvents(window, event);
    nextStepButton.handleMouseEvents(window, event);
    playButton.handleMouseEvents(window, event);

    backButton.handleMouseEvents(window, event);
    settingButton.handleMouseEvents(window, event);
}

void HashMapScene::timePropagation(float delta) {
    insertField.timePropagation();
    eraseField.timePropagation();
    treeUI.timePropagation(delta);
}

void HashMapScene::draw(sf::RenderWindow &window) {
    window.draw(treeUI.getCurrentUI());
    window.draw(treeUI.getCurrentCode());
    window.draw(backButton);
    window.draw(settingButton);
    window.draw(insertField);
    window.draw(insertButton);
    window.draw(eraseField);
    window.draw(eraseButton);
    window.draw(prevOperationButton);
    window.draw(prevStepButton);
    window.draw(nextOperationButton);
    window.draw(nextStepButton);
    window.draw(playButton);
}