#pragma once
#include <SFML/Graphics.hpp>
#include "../include/utility.hpp"
#include <vector>
#include <memory>
#include <string>
#include <utility>

const sf::Color floatColor = sf::Color({69, 123, 157});
const sf::Color fixedColor = sf::Color({230, 57, 70});

const float STATICFRICTION = 2.f;
const float KINETICFRICTION = 1.5f;
const float STATIONARY = 1e-3f;

enum class NodeState { FLOAT, FIXED };

class FloatingNode : public sf::Drawable, public sf::Transformable {
private:
    sf::CircleShape circle;
    sf::Text annotation;
    sf::Color color;
    NodeState state;
    bool dragMode, waitMouse;

    sf::Vector2f velocity, acceleration;

public:
    // === CONSTRUCTOR ===
    FloatingNode (float radius, const std::string &msgString, const sf::Font &font) :
        annotation(font, msgString), state(NodeState::FLOAT), dragMode(false), waitMouse(false) {
        // setup circle
        circle.setRadius(radius);
        circle.setOrigin({radius, radius});
        circle.setFillColor(floatColor);

        // setup annotation
        annotation.setCharacterSize(50);
        annotation.setFillColor(sf::Color::White);

        sf::FloatRect localRectangle = annotation.getLocalBounds();
        annotation.setOrigin({
            localRectangle.position.x + localRectangle.size.x / 2.f,
            localRectangle.position.y + localRectangle.size.y / 2.f
        });
    }

    // === GEOMETRY HELPER FUNCTION ===
    sf::Vector2f getOrigin() const { return circle.getOrigin(); }

    // === HELPER FUNCTION FOR DRAWING ===
    void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
        // apply state transform
        states.transform *= getTransform();

        // show circle and annotation
        target.draw(circle, states);
        target.draw(annotation, states);
    }

    // === HANDLE FLOW STATE ===
    void applyForce (const FloatingNode &o) {
        sf::Vector2f a = getOrigin(), b = o.getOrigin();
        acceleration += (a - b) / cube(distance(a, b));
    }

    void resetAcceleration() { acceleration = sf::Vector2f(0, 0); }

    void applyFriction() {
        if (magnitude(velocity) <= STATIONARY and magnitude(acceleration) <= STATICFRICTION) { // static friction
            resetAcceleration();
        }
        else { // kinetic friction
            sf::Vector2f friction = velocity * KINETICFRICTION / magnitude(velocity);
            acceleration -= friction;
        }
    }

    // === HANDLE MOUSE EVENTS ===
    bool onClick (const sf::Vector2f &mousePos) const {
        sf::Vector2f localPos = this->getInverseTransform().transformPoint(mousePos);
        return circle.getGlobalBounds().contains(localPos);
    }

    void handleMouseClick (const sf::Vector2f &mousePos) {
        if (!onClick(mousePos)) return;
        waitMouse = true;
    }

    void handleMouseMovement() {
        if (!waitMouse) return;
        if (state == NodeState::FIXED) {
            dragMode = true;
        }
        waitMouse = false;
    }

    void handleMouseRelease (const sf::Vector2f &mousePos) {
        if (waitMouse) { // change state
            if (state == NodeState::FIXED) {
                state = NodeState::FLOAT;
                circle.setFillColor(floatColor);
            }
            else {
                state = NodeState::FIXED;
                circle.setFillColor(fixedColor);
            }
            waitMouse = false;
        }
        else { // release node from drag mode
            dragMode = false;
        }
    }

    bool isDragged() const { return dragMode; }
};

class NodeList : public std::vector<std::unique_ptr<FloatingNode>> {
public:
    void pushNode (float radius, const std::string &msgString, const sf::Font &font) {
        push_back(std::make_unique<FloatingNode>(radius, msgString, font));
    }

    // === HANDLE MOUSE EVENTS ===
    void handleMouseClick (const sf::Vector2f &mousePos) {
        for (auto &node : *this)
            node->handleMouseClick(mousePos);
    }

    void handleMouseRelease (const sf::Vector2f &mousePos) {
        for (auto &node : *this)
            node->handleMouseRelease(mousePos);
    }

    void handleMouseMovement (const sf::Vector2f &mousePos) {
        for (auto &node : *this) {
            node->handleMouseMovement();
            if (node->isDragged()) node->setPosition(mousePos);
        }
    }

    // === HANDLE POSITION ===
    void updatePosition() { return; }
};