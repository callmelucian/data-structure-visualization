#pragma once
#include <SFML/Graphics.hpp>
#include "nodes.hpp"
#include "../include/utility.hpp"
#include <cmath>
#include <memory>

class Edges : public sf::Drawable, public sf::Transformable {
private:
    const std::unique_ptr<FloatingNode> &from;
    const std::unique_ptr<FloatingNode> &to;
    float thickness;
    bool isDirected;

public:
    // constructors
    Edges() : from(nullptr), to(nullptr), thickness(0), isDirected(false) {}
    Edges (const std::unique_ptr<FloatingNode> &from, const std::unique_ptr<FloatingNode> &to, float thickness, bool isDirected) :
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
};