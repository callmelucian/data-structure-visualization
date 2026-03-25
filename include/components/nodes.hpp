#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <cmath>
#include <random>
#include <iostream>

#include "ui-base.hpp"
#include "../assets/theme.hpp"
#include "../core/global-setting.hpp"
#include "../core/utility.hpp"

extern const float eps;

namespace UI {

class Node : public UI::Base {
private:
    sf::CircleShape circle;
    UI::Text label;

public:
    Node(const std::string &msg, float radius = 30.f, float thickness = 2.f);

    // draw node onto the screen
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    float getRadius() const;

    sf::FloatRect getBoundary() const override;

    sf::FloatRect getGlobalBounds() const;

    // handle mouse press
    void handleMousePress(const sf::Vector2f &mousePos) override;
    
    // handle mouse movement
    void handleMouseMovement(const sf::Vector2f &mousePos) override;
    
    // handle mouse release
    void handleMouseRelease(const sf::Vector2f &mousePos) override;
    
    // handle text entered
    void handleTextEntered(const char &unicode) override;
};

class AnimatedNode : public sf::Drawable {
private:
    Node nodeUI;
    sf::Vector2f targetPosition;

public:
    AnimatedNode(const std::string &msg, float radius = 30.f, float thickness = 2.f);
    
    void setTargetPosition(float x, float y);

    void setTargetX(float x);

    void setTargetY(float y);

    float getTargetX() const;

    float getTargetY() const;

    void timePropagation(float deltaTime);

    sf::FloatRect getGlobalBounds() const;

    sf::Vector2f getPosition() const;

    float getRadius() const;

    friend void swapAnimatedNode(AnimatedNode &a, AnimatedNode &b);

    // draw node onto the screen
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

void drawEdge(sf::RenderTarget &target, sf::RenderStates state, const AnimatedNode &from, const AnimatedNode &to, float thickness = 2.f);

} // namespace UI