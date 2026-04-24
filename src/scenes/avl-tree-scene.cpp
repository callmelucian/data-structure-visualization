#include "../../include/scenes/avl-tree-scene.hpp"

AVLTreeScene::AVLTreeScene (SceneManager &manager) :
    Scene(manager, 3), insertField(1000, 50), eraseField(1000, 50), treeUI(UI::BinaryTree()) {
    
    // initialize buttons
    featureButtons[0].setString("INSERT");
    featureButtons[0].setCharacterSize(20);
    featureButtons[1].setString("ERASE");
    featureButtons[1].setCharacterSize(20);
    featureButtons[2].setString("FILE");
    featureButtons[2].setCharacterSize(20);

    // intialize fields
    insertField.setPosition({Setting::screenWidth / 2.f, 85});
    insertField.setLabel("Insert/Init");
    insertField.disable();
    eraseField.setPosition({Setting::screenWidth / 2.f, 85});
    eraseField.setLabel("Erase");
    eraseField.disable();
    
    // set callback functions: AVL Tree UI
    treeUI.setCallbackEnableButtons([&](int f) {
        if (f) {
            featureButtons[0].enableButton();
            featureButtons[1].enableButton();
        }
        else {
            featureButtons[0].disableButton();
            featureButtons[1].disableButton();
            insertField.disable();
            eraseField.disable();
        }
    });
    treeUI.setCallbackPlayPause(changePlayButton);

    // set callback functions: input field and button for insertion
    insertField.setCallbackFunction([&](const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.empty()) return;
        bool skip = (numbers.size() >= 2);
        for (int value : numbers) {
            treeUI.transformLogic([&](DS::AVLTree &avl) {
                return avl.insert(value), true;
            });
            if (skip) treeUI.nextCompleteState();
        }
    });
    featureButtons[0].setCallback([&]() {
        if (insertField.isEnabled()) insertField.disable();
        else insertField.enable();
    });

    // set callback functions: input field and button for deletion
    eraseField.setCallbackFunction([&](const std::string &msg) {
        std::vector<int> numbers = stringToNumbers(msg);
        if (numbers.empty()) return;
        bool skip = numbers.size() >= 2;
        for (int value : numbers) {
            treeUI.transformLogic([&](DS::AVLTree &avl) {
                return avl.erase(value);
            });
            if (skip) treeUI.nextCompleteState();
        }
    });
    featureButtons[1].setCallback([&]() {
        if (eraseField.isEnabled()) eraseField.disable();
        else eraseField.disable();
    });

    featureButtons[2].setCallback([&]() {
        std::cerr << "Read file " << openFileDialog() << std::endl;
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

void AVLTreeScene::handleEvent(sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    treeUI.getCurrentCode().handleMouseEvents(window, event);
    insertField.handleMouseEvents(window, event);
    insertField.handleTextEvents(window, event);
    eraseField.handleMouseEvents(window, event);
    eraseField.handleTextEvents(window, event);
    handleButtons(window, event);
}

void AVLTreeScene::timePropagation(float delta) {
    insertField.timePropagation();
    eraseField.timePropagation();
    treeUI.timePropagation(delta);
}

void AVLTreeScene::draw(sf::RenderWindow &window) {
    window.draw(treeUI.getCurrentUI());
    window.draw(treeUI.getCurrentCode());
    window.draw(insertField);
    window.draw(eraseField);
    drawButtons(window);
}