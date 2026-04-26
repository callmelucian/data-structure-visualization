// https://gemini.google.com/share/dc25c97c303f

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

namespace UI {

class Base : public sf::Drawable, public sf::Transformable {
public:
    bool containPosition(const sf::Vector2f &position) const;

    // return the rectangular boundary of the UI Base
    virtual sf::FloatRect getBoundary() const = 0;

    // helper functions to handle mouse events
    virtual void handleMousePress(const sf::Vector2f &mousePos) = 0;
    virtual void handleMouseRelease(const sf::Vector2f &mousePos) = 0;
    virtual void handleMouseMovement(const sf::Vector2f &mousePos) = 0;

    // helper functions to handle text events
    virtual void handleTextEntered(const char &unicode) = 0;

    // handle mouse events
    void handleMouseEvents(sf::RenderWindow &window, const std::optional<sf::Event> &event);

    // handle text events
    void handleTextEvents(sf::RenderWindow &window, const std::optional<sf::Event> &event);
};

class Text : public sf::Text {
public:
    // using sf::Text's constructor
    using sf::Text::Text;

    // set the origin to the center the bounding box of the text
    void centerOrigin();

    float getWidth() const;

    // automatically find the best font size to fill the container
    void setAutoCharacterSize(float boxW, float boxH, float textFill = 0.8f);
};

class RoundedRectangle : public sf::Shape {
private:
    sf::Vector2f sizeVector;
    float radius;
    unsigned int cornerPointCount;

public:
    // constructor
    RoundedRectangle(float width, float height, float radius, unsigned int cornerPointCount = 20);

    // update the size and radius
    void setProperties(float width, float height, float radius);

    sf::Vector2f getSize() const;

    void centerOrigin();
    void setWidth (float newWidth);

    float getWidth() const;

    // required overrides for sf::Shape
    virtual std::size_t getPointCount() const override;
    virtual sf::Vector2f getPoint(std::size_t index) const override;
};

}; // namespace UI