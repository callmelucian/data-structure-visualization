#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <utility>

#include "ui-base.hpp"
#include "nodes.hpp"
#include "../core/utility.hpp"

extern const float HIGHLIGHT_SMALL_DISTANCE;

namespace UI {

class HighlightCircle : public UI::Base {
private:
    sf::CircleShape circle;
    bool highlightLocked;
    const AnimatedNode* nodeObserver;

public:
    HighlightCircle();

    void setTargetNode (AnimatedNode* ptr);

    void timePropagation (float deltaTime);

    void fastForward();
    
    void free();

    const AnimatedNode* getAddress() const;

    sf::FloatRect getBoundary() const override;

    void handleMousePress (const sf::Vector2f &mousePos) override;
    void handleMouseRelease (const sf::Vector2f &mousePos) override;
    void handleMouseMovement (const sf::Vector2f &mousePos) override;
    void handleTextEntered (const char &unicode) override;

    void draw (sf::RenderTarget &target, sf::RenderStates states) const override;

    void lockHighlight();
};

}; // namespace UI

