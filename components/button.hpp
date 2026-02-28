#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "fonts.hpp"

const sf::Color idleColor = sf::Color({100, 100, 100});

class Button : public sf::Drawable, public sf::Transformable {
private:
    sf::RectangleShape rectangle;
    sf::Text label;
    sf::Color activeColor;
    std::function<void()> onClick;
    bool activation;

public:
    Button (float width, float height, const std::string &labelString, int fontSize,
            const sf::Color &activeColor) :
        label(cmuSerif, labelString), activeColor(activeColor), activation(true) {
        
        // setup rectangle
        rectangle.setSize({width, height});
        rectangle.setOrigin({width / 2.f, height / 2.f});
        rectangle.setFillColor(activeColor);

        // setup label
        label.setCharacterSize(fontSize);
        label.setFillColor(sf::Color::White);

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

    void handleClick (const sf::Vector2f &mousePos) {
        if (!containPosition(mousePos)) return;
        onClick();
    }
};