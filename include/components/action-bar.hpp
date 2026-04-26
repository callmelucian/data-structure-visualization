// #pragma once
// #include <SFML/Graphics.hpp>
// #include <string>

// #include "../assets/theme.hpp"
// #include "../core/global-setting.hpp"
// #include "ui-base.hpp"

// class ActionBar : public UI::Base {
// private:
//     UI::Text appTitle, subtitle;
//     sf::RectangleShape rectangle;

// public:
//     // constructor
//     ActionBar() : appTitle(Theme::notoCondensedBold, Setting::appName), subtitle(Theme::notoCondensed) {
//         // setup background rectangle for action bar
//         rectangle.setSize({Setting::actionBarWidth, Setting::actionBarHeight});
//         rectangle.setOrigin({Setting::actionBarWidth / 2.f, Setting::actionBarHeight / 2.f});
//         rectangle.setFillColor(Theme::getBackground());

//         // setup annotation
//         appTitle.setAutoCharacterSize(1'000'000, 50, 1.f);
//         appTitle.setFillColor(Theme::getPrimary());
//         appTitle.centerOrigin();
//         appTitle.setPosition({0, -15});

//         subtitle.setAutoCharacterSize(1'000'000, 25, 1.f);
//         subtitle.setFillColor(Theme::getPrimary());
//         subtitle.centerOrigin();
//         subtitle.setPosition({0, 27.5f});
//     }

//     void setSubtitle (const std::string &msg) {
//         subtitle.setString(msg);
//         subtitle.setAutoCharacterSize(1'000'000, 25, 1.f);
//         subtitle.centerOrigin();
//     }

//     // (override) draw action-bar onto the screen
//     void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
//         // apply state transform
//         states.transform *= getTransform();

//         // show rectangle and label
//         target.draw(rectangle, states);
//         target.draw(appTitle, states);
//         target.draw(subtitle, states);
//     }

//     // (override) return the boundary of the action bar
//     sf::FloatRect getBoundary() const override {
//         return rectangle.getGlobalBounds();
//     }

//     // (override) handle mouse events
//     void handleMousePress (const sf::Vector2f &mousePos) override {}
//     void handleMouseRelease (const sf::Vector2f &mousePos) override {}
//     void handleMouseMovement (const sf::Vector2f &mousePos) override {}

//     // (override) handle text events
//     void handleTextEntered (const char &unicode) override {}
// };