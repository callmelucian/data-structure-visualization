#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class FloatingNode : public sf::Drawable, public sf::Transformable {
private:
    sf::CircleShape circle;
    sf::Text annotation;
    sf::Color color;

public:
    FloatingNode (float radius, const std::string &msgString, const sf::Font &font, const sf::Color &initColor) :
        annotation(font, msgString) {
        // setup circle
        circle.setRadius(radius);
        circle.setOrigin({radius, radius});
        circle.setFillColor(initColor);

        // setup annotation
        annotation.setCharacterSize(30);
        annotation.setFillColor(sf::Color::White);

        sf::FloatRect localRectangle = annotation.getLocalBounds();
        annotation.setOrigin({
            localRectangle.position.x + localRectangle.size.x / 2.f,
            localRectangle.position.y + localRectangle.size.y / 2.f
        });
    }

    void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
        // apply state transform
        states.transform *= getTransform();

        // show circle and annotation
        target.draw(circle, states);
        target.draw(annotation, states);
    }
};