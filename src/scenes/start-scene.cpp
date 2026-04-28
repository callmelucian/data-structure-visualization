#include "../../include/scenes/start-scene.hpp"

const std::vector<std::string> sceneName = {
    "LINKED LIST", "HASH MAP",
    "SHORTEST PATH", "MINIMUM SPANNING TREE",
    "AVL TREE", "RED-BLACK TREE"
};

StartScene::StartScene (SceneManager &manager) :
    Scene(manager), navigation(6, UI::Button(400.f, 70.f, 20.f)),
    title(Theme::googleSansBold), subtitle(Theme::googleSansItalic) {

    for (int i = 0; i < navigation.size(); i++) {
        navigation[i].setString(sceneName[i], 25);
        navigation[i].setOutlineThickness(3);
        navigation[i].setCallback([i, &manager]() { manager.changeScene(i + 1); });
    }

    for (int i = 0, pos = 340; i < navigation.size(); i += 2, pos += 145) {
        navigation[i].setPosition({600, (float)pos});
    }
    for (int i = 1, pos = 340; i < navigation.size(); i += 2, pos += 145) {
        navigation[i].setPosition({1200, (float)pos});
    }

    title.setString("DATA STRUCTURE VISUALIZER");
    title.setCharacterSize(40);
    title.setPosition({Setting::screenWidth / 2.f, 75});
    title.setFillColor(Theme::textPrimary);
    title.centerOrigin();
    
    subtitle.setString("By Tan Minh @ Lucian");
    subtitle.setCharacterSize(24);
    subtitle.setPosition({Setting::screenWidth / 2.f, 110});
    subtitle.setFillColor(Theme::accentDark);
    subtitle.centerOrigin();
}

void StartScene::handleEvent (sf::RenderWindow &window, const std::optional<sf::Event> &event) {
    for (UI::Button &button : navigation)
        button.handleMouseEvents(window, event);
    setting.handleMouseEvents(window, event);
}

void StartScene::timePropagation (float delta) {}

void StartScene::draw (sf::RenderWindow &window) {
    for (UI::Button &button : navigation)
        button.changeColor(), window.draw(button);
    setting.changeColor();
    window.draw(setting);
    title.setFillColor(Theme::textPrimary);
    window.draw(title);
    subtitle.setFillColor(Theme::accentDark);
    window.draw(subtitle);
}