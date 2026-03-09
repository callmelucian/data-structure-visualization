#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <cmath>

#include "../assets/theme.hpp"
#include "ui-base.hpp"

const sf::Color holderColor = sf::Color::White;
const sf::Color hoveredColor = sf::Color({0, 0, 0, 55});

class Slider : public UI::Base {
private:
    sf::RectangleShape holder;
    sf::RectangleShape hovered;
    sf::RectangleShape filled;
    UI::Text annotation, label;
    std::function<void(float)> changeValue;
    std::function<std::string(float)> displayValue;
    bool followMouse;

public:
    Slider (float width, float height) :
    annotation(Theme::ibmRegular), label(Theme::notoCondensed), displayValue(floatToPercentage), followMouse(false) {
        // setup holder bar
        holder.setSize({width, height});
        holder.setOutlineColor(Theme::getPrimary());
        holder.setOutlineThickness(2.f);
        holder.setFillColor(holderColor);

        // setup hover bar
        hovered.setSize({0.f, height});
        hovered.setFillColor(hoveredColor);

        // setup filled bar
        filled.setSize({0.f, height});
        filled.setFillColor(Theme::getPressedButton());

        // setup annotation
        annotation.setFillColor(Theme::getPrimary());
        annotation.setAutoCharacterSize(width, height);
        annotation.setPosition({width / 2.f, height / 2.f});
        setString("0%");
        
        // center origin
        setOrigin({width / 2.f, height / 2.f});
    }

    void setString (const std::string &msg) {
        annotation.setString(msg);
        annotation.setAutoCharacterSize(
            holder.getSize().x,
            holder.getSize().y
        );
        annotation.centerOrigin();
    }

    // set change-value function
    void setChangeValueFunction (auto cvf) {
        changeValue = cvf;
    }

    // set display-value function
    void setDisplayValueFunction (auto dvf) {
        displayValue = dvf;
    }

    // (override) draw slider onto the screen
    void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(holder, states);
        target.draw(filled, states);
        target.draw(hovered, states);
        target.draw(annotation, states);
        target.draw(label, states);
    }

    // (override) get boundary
    sf::FloatRect getBoundary() const override {
        return holder.getGlobalBounds();
    }

    // update slider value
    void updateSliderValue() {
        filled.setSize(hovered.getSize());
        float newValue = filled.getSize().x / holder.getSize().x;
        changeValue(newValue);
        setString(displayValue(newValue));
    }

    // (override) handle mouse press
    void handleMousePress (const sf::Vector2f &mousePos) override {
        if (!containPosition(mousePos)) return;
        updateSliderValue();
        followMouse = true;
    }

    // (override) handle mouse release
    void handleMouseRelease (const sf::Vector2f &mousePos) override {
        if (!followMouse) return;
        updateSliderValue();
        followMouse = false;
    }

    // (override) handle mouse movement
    void handleMouseMovement (const sf::Vector2f &mousePos) override {
        if (followMouse or containPosition(mousePos)) {
            float hoverLength = this->getInverseTransform().transformPoint(mousePos).x;
            hoverLength = std::max(0.f, hoverLength);
            hoverLength = std::min(holder.getSize().x, hoverLength);
            hovered.setSize({hoverLength, holder.getSize().y});
            if (followMouse) updateSliderValue();
        }
        else hovered.setSize({0.f, holder.getSize().y});
    }
};