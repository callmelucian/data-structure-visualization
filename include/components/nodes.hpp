#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <random>
#include <iostream>

#include "ui-base.hpp"
#include "../assets/theme.hpp"
#include "../core/global-setting.hpp"
#include "../core/utility.hpp"

const float eps = 1e-6;

class Node : public UI::Base {
private:
    sf::CircleShape circle;
    UI::Text label;

public:
    Node (const std::string &msg, float radius = 30.f, float thickness = 2.f) :
    circle(radius, 100), label(Theme::ibmRegular, msg) {
        // setup circle
        circle.setOrigin({radius, radius});
        circle.setFillColor(Theme::getFloatColor());
        circle.setOutlineColor(Theme::getPrimary());
        circle.setOutlineThickness(thickness);

        // setup annotation
        float squareSize = std::sqrt(2 * radius * radius);
        label.setAutoCharacterSize(squareSize, squareSize);
        label.setFillColor(sf::Color::White);
        label.centerOrigin();
    }

    // draw node onto the screen
    void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
        // apply state transform
        states.transform *= getTransform();

        // show circle and annotation
        target.draw(circle, states);
        target.draw(label, states);
    }

    float getRadius() const {
        return circle.getRadius();
    }

    sf::FloatRect getBoundary() const override {
        return circle.getGlobalBounds();
    }

    sf::FloatRect getGlobalBounds() const {
        return getTransform().transformRect(circle.getGlobalBounds());
    }

    // handle mouse press
    void handleMousePress (const sf::Vector2f &mousePos) override {}
    void handleMouseMovement (const sf::Vector2f &mousePos) override {}
    void handleMouseRelease (const sf::Vector2f &mousePos) override {}
    void handleTextEntered (const char &unicode) override {}
};

class AnimatedNode : public sf::Drawable {
private:
    Node nodeUI;
    sf::Vector2f targetPosition;

public:
    AnimatedNode (const std::string &msg, float radius = 30.f, float thickness = 2.f) :
        nodeUI(msg, radius, thickness), targetPosition({randFloat(0, Setting::screenWidth), randFloat(0, Setting::screenHeight)}) {
            nodeUI.setPosition(targetPosition);
        }
    
    void setTargetPosition (float x, float y) {
        targetPosition = sf::Vector2f({x, y});
    }

    void setTargetX (float x) {
        targetPosition.x = x;
    }

    void setTargetY (float y) {
        targetPosition.y = y;
    }

    float getTargetX() const {
        return targetPosition.x;
    }

    float getTargetY() const {
        return targetPosition.y;
    }

    void timePropagation (float deltaTime) {
        sf::Vector2f displacement = targetPosition - nodeUI.getPosition();
        if (magnitude(displacement) < eps) nodeUI.setPosition(targetPosition);
        else {
            sf::Vector2f newPosition = nodeUI.getPosition() + displacement * Setting::animationFactor * deltaTime;
            nodeUI.setPosition(newPosition);
        }
    }

    sf::FloatRect getGlobalBounds() const {
        return nodeUI.getGlobalBounds();
    }

    sf::Vector2f getPosition() const {
        return nodeUI.getPosition();
    }

    float getRadius() const {
        return nodeUI.getRadius();
    }

    friend void swapAnimatedNode (AnimatedNode &a, AnimatedNode &b) {
        std::swap(a.nodeUI, b.nodeUI);
    }

    // draw node onto the screen
    void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
        nodeUI.draw(target, states);
    }
};

void drawEdge (sf::RenderTarget &target, sf::RenderStates state, const AnimatedNode &from, const AnimatedNode &to, float thickness = 2.f) {
    if (from.getPosition() == to.getPosition()) return;

    // get centers of the endpoints
    sf::Vector2f fromCenter = from.getPosition();
    sf::Vector2f toCenter = to.getPosition();
    sf::Vector2f shift = (toCenter - fromCenter) * from.getRadius() / distance(fromCenter, toCenter);

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

namespace OldCode {
    // #pragma once
    // #include <SFML/Graphics.hpp>
    // #include "../include/utility.hpp"
    // #include "../assets/theme.hpp"
    // #include "text.hpp"
    // #include <vector>
    // #include <memory>
    // #include <string>
    // #include <utility>

    // const sf::Vector2f zeroVector = sf::Vector2f(0.f, 0.f);

    // const float STATICFRICTION = 0.1f;
    // const float KINETICFRICTION = 0.08f;
    // const float STATIONARY = 0.05f;
    // const float eps = 1e-2f;

    // enum class NodeState { FLOAT, FIXED };

    // class FloatingNode : public sf::Drawable, public sf::Transformable {
    // private:
    //     sf::CircleShape circle;
    //     // sf::Text annotation;
    //     UIText annotation;
    //     sf::Color color;
    //     NodeState state;
    //     bool dragMode, waitMouse;

    //     sf::Vector2f velocity, acceleration;

    // public:
    //     // constructor
    //     FloatingNode (float radius, const std::string &msgString, const sf::Font &font, float fontSize, float thickness) :
    //     circle(radius, 100), annotation(font, msgString), state(NodeState::FLOAT), dragMode(false), waitMouse(false) {
    //         // setup circle
    //         circle.setOrigin({radius, radius});
    //         circle.setFillColor(Theme::getFloatColor());
    //         circle.setOutlineColor(Theme::getPrimary());
    //         circle.setOutlineThickness(thickness);

    //         // setup annotation
    //         annotation.setCharacterSize(fontSize);
    //         annotation.setFillColor(sf::Color::White);
    //         annotation.centerOrigin();
    //     }

    //     // draw node onto the screen
    //     void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
    //         // apply state transform
    //         states.transform *= getTransform();

    //         // show circle and annotation
    //         target.draw(circle, states);
    //         target.draw(annotation, states);
    //     }

    //     // === HANDLE FLOW STATE ===
    //     void applyForce (const FloatingNode &o) {
    //         sf::Vector2f a = getPosition(), b = o.getPosition();
    //         acceleration += (a - b) * 1'500.f / cube(std::max(eps, distance(a, b)));
    //     }

    //     void resetAcceleration() { acceleration = sf::Vector2f(0, 0); }

    //     void applyFriction() {
    //         if (magnitude(velocity) <= STATIONARY and magnitude(acceleration) <= STATICFRICTION) { // static friction
    //             resetAcceleration();
    //         }
    //         else if (magnitude(velocity) > STATIONARY) { // kinetic friction
    //             sf::Vector2f friction = velocity * KINETICFRICTION / magnitude(velocity);
    //             acceleration -= friction;
    //         }
    //     }

    //     void applyAcceleration (float timeInterval) {
    //         velocity += acceleration * timeInterval;
    //     }

    //     sf::Vector2f getVelocity() const {
    //         return magnitude(velocity) <= STATIONARY ? zeroVector : velocity;
    //     }

    //     sf::Vector2f getAcceleration() const { return acceleration; }

    //     // check if the position is contained in the rectangle
    //     bool containPosition (const sf::Vector2f &mousePos) const {
    //         sf::Vector2f localPos = this->getInverseTransform().transformPoint(mousePos);
    //         return circle.getGlobalBounds().contains(localPos);
    //     }

    //     void handleMousePress (const sf::Vector2f &mousePos) {
    //         if (!onClick(mousePos)) return;
    //         waitMouse = true;
    //     }

    //     void handleMouseMovement() {
    //         if (!waitMouse) return;
    //         if (state == NodeState::FIXED) {
    //             dragMode = true;
    //         }
    //         waitMouse = false;
    //     }

    //     void handleMouseRelease (const sf::Vector2f &mousePos) {
    //         if (waitMouse) { // change state
    //             if (state == NodeState::FIXED) {
    //                 state = NodeState::FLOAT;
    //                 circle.setFillColor(Theme::getFloatColor());
    //             }
    //             else {
    //                 state = NodeState::FIXED;
    //                 circle.setFillColor(Theme::getFixedColor());
    //                 velocity = zeroVector;
    //             }
    //             waitMouse = false;
    //         }
    //         else { // release node from drag mode
    //             dragMode = false;
    //         }
    //     }

    //     bool isDragged() const { return dragMode; }
    //     bool isFloating() const { return state == NodeState::FLOAT; }
    //     bool isFixed() const { return state == NodeState::FIXED; }

    //     float getRadius() const { return circle.getRadius(); }
    // };

    // class NodeList : public std::vector<std::unique_ptr<FloatingNode>> {
    // public:
    //     void pushNode (float radius, const std::string &msgString, const sf::Font &font, float fontSize, float thickness) {
    //         push_back(std::make_unique<FloatingNode>(radius, msgString, font, fontSize, thickness));
    //     }

    //     // === HANDLE MOUSE EVENTS ===
    //     void handleMouseClick (const sf::Vector2f &mousePos) {
    //         for (auto &node : *this)
    //             node->handleMouseClick(mousePos);
    //     }

    //     void handleMouseRelease (const sf::Vector2f &mousePos) {
    //         for (auto &node : *this)
    //             node->handleMouseRelease(mousePos);
    //     }

    //     void handleMouseMovement (const sf::Vector2f &mousePos) {
    //         for (auto &node : *this) {
    //             node->handleMouseMovement();
    //             if (node->isDragged()) node->setPosition(mousePos);
    //         }
    //     }

    //     // === HANDLE POSITION ===
    //     void updatePosition (float timeInterval, float boundL, float boundR, float boundT, float boundB) {
    //         for (auto &node : *this) {
    //             node->resetAcceleration();
    //             if (node->isFixed()) continue;
    //             for (const auto &otherNode : *this) node->applyForce(*otherNode);
    //             node->applyFriction();
    //         }

    //         for (auto &node : *this) {
    //             if (node->isFixed()) continue;
    //             node->applyAcceleration(timeInterval);
    //             node->move(node->getVelocity());
    //             sf::Vector2f pos = node->getPosition();
    //             float size = node->getRadius();
    //             node->setPosition({std::max(boundL + size, std::min(boundR - size, pos.x)),
    //                                std::max(boundT + size, std::min(boundB - size, pos.y))});
    //         }
    //     }
    // };
};