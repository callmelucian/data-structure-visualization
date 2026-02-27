#pragma once
#include <SFML/Graphics.hpp>
#include "../include/utility.hpp"
#include <vector>
#include <memory>
#include <string>
#include <utility>

const sf::Color floatColor = sf::Color({69, 123, 157});
const sf::Color fixedColor = sf::Color({230, 57, 70});
const sf::Vector2f zeroVector = sf::Vector2(0.f, 0.f);

const float STATICFRICTION = .01f;
const float KINETICFRICTION = .008f;
const float STATIONARY = 1e-9f;
const float eps = 1e-6f;

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
    sf::Vector2f getPosition() const {
        return getTransform().transformPoint(circle.getPosition());
    }

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
        sf::Vector2f a = getPosition(), b = o.getPosition();
        if (distance(a, b) > eps) acceleration += (a - b) * 1'000.f / cube(distance(a, b));
    }

    void resetAcceleration() { acceleration = sf::Vector2f(0, 0); }

    void applyFriction() {
        if (magnitude(velocity) <= STATIONARY and magnitude(acceleration) <= STATICFRICTION) { // static friction
            resetAcceleration();
        }
        else if (magnitude(velocity) > eps) { // kinetic friction
            sf::Vector2f friction = velocity * KINETICFRICTION / magnitude(velocity);
            acceleration -= friction;
        }
    }

    void applyAcceleration (float timeInterval) {
        velocity += acceleration * timeInterval;
    }

    sf::Vector2f getVelocity() const { return velocity; }
    sf::Vector2f getAcceleration() const { return acceleration; }

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
                velocity = zeroVector;
            }
            waitMouse = false;
        }
        else { // release node from drag mode
            dragMode = false;
        }
    }

    bool isDragged() const { return dragMode; }
    bool isFloating() const { return state == NodeState::FLOAT; }
    bool isFixed() const { return state == NodeState::FIXED; }
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
    void updatePosition (float timeInterval) {
        for (auto &node : *this) {
            node->resetAcceleration();
            if (node->isFixed()) continue;
            for (const auto &otherNode : *this) node->applyForce(*otherNode);
            node->applyFriction();
        }

        for (auto &node : *this) {
            if (node->isFixed()) continue;
            // std::cerr << "Node " << node->getVelocity() << " " << node->getAcceleration() << std::endl;
            node->applyAcceleration(timeInterval);
            node->move(node->getVelocity());
        }
    }
};