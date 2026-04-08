#include "../../include/components/graph.hpp"

// ========== CONSTANTS INITIALIZATION ==========
const float REPULSIVE_CONSTANT = 10'000.f;
const float IDEAL_LENGTH = 100.f;
const float SPRING_CONSTANT = 0.05f;
const float FRICTION_CONSTANT = 0.9f;

void Graph::timePropagation (float deltaTime) {
    for (UI::FloatingNode* ptr : nodes) {
        sf::Vector2f currentPosition = ptr->getPosition();
        ptr->resetAcceleration();
        for (UI::FloatingNode* otherPtr : nodes) {
            if (ptr == otherPtr) continue;
            sf::Vector2f otherPosition = otherPtr->getPosition();
            sf::Vector2f repulsiveForce = unitVector(otherPosition, currentPosition) * REPULSIVE_CONSTANT / squaredDistance(currentPosition, otherPosition);
            ptr->addAcceleration(repulsiveForce);
        }
    }
    for (auto [fromID, toID] : edges) {
        sf::Vector2f fromPosition = nodes[fromID]->getPosition();
        sf::Vector2f toPosition = nodes[toID]->getPosition();
        sf::Vector2f springForce = unitVector(fromPosition, toPosition) * SPRING_CONSTANT * (distance(fromPosition, toPosition) - IDEAL_LENGTH);
        nodes[fromID]->addAcceleration(springForce);
        nodes[toID]->addAcceleration(-springForce);
    }
    for (UI::FloatingNode* ptr : nodes) ptr->timePropagation(deltaTime);
}