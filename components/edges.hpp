#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <memory>

#include "../include/utility.hpp"
#include "nodes.hpp"
#include "ui-base.hpp"

class Edges : public UI::Base {
private:
    const std::unique_ptr<Node> &from;
    const std::unique_ptr<Node> &to;
    float thickness;
    bool isDirected;

public:
    // constructors
    Edges() : from(nullptr), to(nullptr), thickness(0), isDirected(false) {}
    Edges (const std::unique_ptr<Node> &from, const std::unique_ptr<Node> &to, float thickness = 2.f, bool isDirected = false) :
        from(from), to(to), thickness(thickness), isDirected(isDirected) {}

    // draw edge onto the screen
    void draw (sf::RenderTarget &target, sf::RenderStates state) const override {
        // get centers of the endpoints
        sf::Vector2f fromCenter = from->getPosition();
        sf::Vector2f toCenter = to->getPosition();
        sf::Vector2f shift = (toCenter - fromCenter) * from->getRadius() / distance(fromCenter, toCenter);

        // pre calculations
        sf::Vector2f start = fromCenter + shift;
        sf::Vector2f end = toCenter - shift;
        sf::Vector2f delta = end - start;
        float dist = distance(start, end);
        float angle = std::atan2(delta.y, delta.x);

        // setup line
        sf::RectangleShape line({dist, thickness});
        line.setOrigin({0, thickness / 2.0f});
        line.setPosition(start);
        line.setRotation(sf::radians(angle));
        line.setFillColor(sf::Color::Black);

        // draw
        target.draw(line, state);
    }

    // override functions
    void handleMousePress (const sf::Vector2f &mousePos) override {}
    void handleMouseMovement (const sf::Vector2f &mousePos) override {}
    void handleMouseRelease (const sf::Vector2f &mousePos) override {}
    void handleTextEntered (const char &unicode) override {}

    sf::FloatRect getBoundary() const override {
        return sf::FloatRect({0, 0}, {0, 0});
    }
};