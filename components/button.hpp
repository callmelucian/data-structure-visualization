#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "../assets/theme.hpp"
#include <iostream>

const sf::Color idleColor = sf::Color({100, 100, 100});

class Button : public sf::Drawable, public sf::Transformable {
private:
    sf::RectangleShape rectangle;
    sf::Text label;
    std::function<void()> onClick;
    bool activation;

public:
    Button (float width, float height, const std::string &labelString, float fontSize, float thickness) :
        label(Theme::notoRegular, labelString), activation(false) {
        
        // setup rectangle
        rectangle.setSize({width, height});
        rectangle.setOrigin({width / 2.f, height / 2.f});
        rectangle.setFillColor(sf::Color::White);
        rectangle.setOutlineThickness(thickness);
        rectangle.setOutlineColor(sf::Color::Black);

        // setup label
        label.setCharacterSize(fontSize);
        label.setFillColor(sf::Color::Black);

        sf::FloatRect localRectangle = label.getLocalBounds();
        label.setOrigin({
            localRectangle.position.x + localRectangle.size.x / 2.f,
            localRectangle.position.y + localRectangle.size.y / 2.f
        });
    }

    void setCallback (std::function<void()> onClickFunction) {
        onClick = onClickFunction;
    }

    void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
        // apply state transform
        states.transform *= getTransform();

        // show rectangle and label
        target.draw(rectangle, states);
        target.draw(label, states);
    }

    // === HANDLE MOUSE EVENTS ===
    bool containPosition (const sf::Vector2f &position) const {
        sf::Vector2f localPos = this->getInverseTransform().transformPoint(position);
        return rectangle.getGlobalBounds().contains(localPos);
    }

    void handleHover (const sf::Vector2f &mousePos) {
        if (!activation)
            rectangle.setFillColor(containPosition(mousePos) ? sf::Color({220, 220, 220}) : sf::Color::White);
    }

    void handleMousePress (const sf::Vector2f &mousePos) {
        if (!containPosition(mousePos)) return;
        rectangle.setFillColor(sf::Color({200, 200, 200}));
        rectangle.setScale({0.97f, 0.97f});
        label.setScale({0.97f, 0.97f});
        activation = true;
    }

    void handleMouseRelease (const sf::Vector2f &mousePos) {
        if (!activation) return;
        rectangle.setFillColor(containPosition(mousePos) ? sf::Color({220, 220, 220}) : sf::Color::White);
        rectangle.setScale({1.f, 1.f});
        label.setScale({1.f, 1.f});
        activation = false;
        onClick();
    }
};