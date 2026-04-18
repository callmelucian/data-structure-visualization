/**
 * BIG TODO: RE-ORGANIZE THE DRAW EDGE FUNCTION !!
 */

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <cmath>
#include <random>
#include <iostream>
#include <utility>

#include "ui-base.hpp"
#include "../assets/theme.hpp"
#include "../core/global-setting.hpp"
#include "../core/utility.hpp"

extern const float eps;

namespace UI {

class Node : public Base {
private:
    sf::CircleShape circle;
    Text label, annotation;

public:
    Node(const std::string &msg, float radius = 30.f, float thickness = 2.f);

    // draw node onto the screen
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    float getRadius() const;

    sf::FloatRect getBoundary() const override;

    sf::FloatRect getGlobalBounds() const;

    void setColor (const sf::Color &color);

    void setAnnotationColor (const sf::Color &color);

    void randomPosition();

    void setAnnotation (const std::string &msg);

    void clampPosition (const sf::Vector2f &pos, float maxWidth, float maxHeight);

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

    sf::Vector2f getTargetPosition() const;

    void timePropagation(float deltaTime);

    void fastForward();

    sf::FloatRect getGlobalBounds() const;
    
    float getRadius() const;
    
    friend void swapAnimatedNode(AnimatedNode &a, AnimatedNode &b);
    
    void highlightNode();
    
    void unHighlightNode();
    
    sf::Vector2f getPosition() const;

    void setPosition (const sf::Vector2f vec);
    void randomPosition();

    // draw node onto the screen
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class FloatingNode : public Node {
private:
    sf::Vector2f velocity, acceleration;
    bool isClicked, isActivated;
    std::function<void()> callbackOnClick;

public:
    FloatingNode(const std::string &msg, float radius = 30.f, float thickness = 2.f);

    void setCallbackOnClick (auto func) {
        callbackOnClick = func;
    }

    void applyAcceleration (sf::Vector2f targetAcceleration);

    void addAcceleration (sf::Vector2f addedAcceleration);

    void resetAcceleration();

    void applyDamping(float coefficient);

    void timePropagation (float deltaTime);

    void activateNode();

    void deactivateNode();

    void copyPosition (const FloatingNode &other);

    // handle mouse press
    void handleMousePress(const sf::Vector2f &mousePos) override;
    
    // handle mouse movement
    void handleMouseMovement(const sf::Vector2f &mousePos) override;
    
    // handle mouse release
    void handleMouseRelease(const sf::Vector2f &mousePos) override;
};

void drawEdge (sf::RenderTarget &target, sf::RenderStates state, const AnimatedNode* from, const AnimatedNode* to, float thickness = 2.f);

} // namespace UI