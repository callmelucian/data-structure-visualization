#pragma once
#include "../assets/theme.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <cmath>

const sf::Color holderColor = sf::Color::White;
const sf::Color hoveredColor = sf::Color({0, 0, 0, 55});
const sf::Color filledColor = sf::Color({100, 100, 100});

std::function<std::string(float)> floatToPercentage = [] (float value) {
    int percentage = static_cast<int>(std::round(value * 100.0f));
    return std::to_string(percentage) + "%";
};

class Slider : public sf::Drawable, public sf::Transformable {
private:
    sf::RectangleShape holder;
    sf::RectangleShape hovered;
    sf::RectangleShape filled;
    sf::Text annotation, label;
    std::function<void(float)> changeValue;
    std::function<std::string(float)> displayValue;

public:
    Slider (float width, float height, const std::string &msgString) :
    annotation(Theme::ibmRegular), label(Theme::notoCondensed, msgString), displayValue(floatToPercentage) {
        holder.setSize({width, height});
        setOrigin({width / 2.f, height / 2.f});

        hovered.setSize({0.f, height});
        filled.setSize({0.f, height});

        holder.setPosition({0.f, 0.f});
        hovered.setPosition({0.f, 0.f});

        holder.setOutlineColor(sf::Color::Black);
        holder.setOutlineThickness(2.0f);
        holder.setFillColor(holderColor);
        hovered.setFillColor(hoveredColor);
        filled.setFillColor(filledColor);

        annotation.setFillColor(sf::Color::Black);
        annotation.setCharacterSize(36);
        annotation.setPosition({width / 2.f, height / 2.f});
        setText("0%");

        label.setFillColor(sf::Color::Black);
        label.setCharacterSize(18);
        label.setPosition({0, -26});
    }

    void setText (const std::string &s) {
        annotation.setString(s);
        sf::FloatRect localRectangle = annotation.getLocalBounds();
        annotation.setOrigin({
            localRectangle.position.x + localRectangle.size.x / 2.f,
            localRectangle.position.y + localRectangle.size.y / 2.f
        });
    }

    void setChangeValueFunction (auto cpf) { changeValue = cpf; }
    void setDisplayValueFunction (auto dvf) { displayValue = dvf; }

    void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
        // apply state transform
        states.transform *= getTransform();

        // show holder, hovered and filled
        target.draw(holder, states);
        target.draw(filled, states);
        target.draw(hovered, states);
        target.draw(annotation, states);
        target.draw(label, states);
    }

    bool containPosition (const sf::Vector2f &position) const {
        sf::Vector2f localPos = this->getInverseTransform().transformPoint(position);
        return holder.getGlobalBounds().contains(localPos);
    }

    void handleMouseHover (const sf::Vector2f &mousePos) {
        if (containPosition(mousePos)) {
            sf::Vector2f localPos = this->getInverseTransform().transformPoint(mousePos);
            hovered.setSize({localPos.x, filled.getSize().y});
        }
        else hovered.setSize({0, filled.getSize().y});
    }

    void handleMouseClick (const sf::Vector2f &mousePos) {
        if (!containPosition(mousePos)) return;
        filled.setSize(hovered.getSize());
        float newValue = filled.getSize().x / holder.getSize().x;
        changeValue(newValue), setText(displayValue(newValue));
    }
};