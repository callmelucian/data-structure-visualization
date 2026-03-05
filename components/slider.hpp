#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

const sf::Color holderColor = sf::Color::White;
const sf::Color hoveredColor = sf::Color({200, 200, 200, 128});
const sf::Color filledColor = sf::Color({100, 100, 100});

class Slider : public sf::Drawable, public sf::Transformable {
private:
    sf::RectangleShape holder;
    sf::RectangleShape hovered;
    sf::RectangleShape filled;
    std::function<void(float)> changePercent;
    bool hovering;

public:
    Slider (float width, float height) : hovering(false) {
        holder.setSize({width, height});
        setOrigin({width / 2.f, height / 2.f});

        hovered.setSize({0.f, height});
        filled.setSize({0.f, height});

        holder.setPosition({0.f, 0.f});
        hovered.setPosition({0.f, 0.f});

        holder.setFillColor(holderColor);
        hovered.setFillColor(hoveredColor);
        filled.setFillColor(filledColor);
    }

    void setChangePercentFunction (auto cpf) { changePercent = cpf; }

    void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
        // apply state transform
        states.transform *= getTransform();

        // show holder, hovered and filled
        target.draw(holder, states);
        target.draw(filled, states);
        target.draw(hovered, states);
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
        changePercent(filled.getSize().x / holder.getSize().x);
    }
};