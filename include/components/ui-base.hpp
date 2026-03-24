#pragma once
#include <SFML/Graphics.hpp>

namespace UI {
    class Base : public sf::Drawable, public sf::Transformable {
    public:
        bool containPosition (const sf::Vector2f &position) const {
            sf::Vector2f localPos = this->getInverseTransform().transformPoint(position);
            return getBoundary().contains(localPos);
        }

        // return the rectangular boundary of the UI Base
        virtual sf::FloatRect getBoundary() const = 0;

        // helper functions to handle mouse events
        virtual void handleMousePress (const sf::Vector2f &mousePos) = 0;
        virtual void handleMouseRelease (const sf::Vector2f &mousePos)  = 0;
        virtual void handleMouseMovement (const sf::Vector2f &mousePos) = 0;

        // helper functions to handle text events
        virtual void handleTextEntered (const char &unicode) = 0;

        // handle mouse events
        void handleMouseEvents (sf::RenderWindow &window, const std::optional<sf::Event> &event) {
            // event: mouse moved
            if (const auto *mouseMovement = event->getIf<sf::Event::MouseMoved>()) {
                sf::Vector2f mousePos = window.mapPixelToCoords(mouseMovement->position);
                handleMouseMovement(mousePos);
            }
            // event: left-mouse pressed
            if (const auto *mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);
                    handleMousePress(mousePos);
                }
            }
            // event: left-mouse released 
            if (const auto *mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseReleased->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(mouseReleased->position);
                    handleMouseRelease(mousePos);
                }
            }
        }

        // handle text events
        void handleTextEvents (sf::RenderWindow &window, const std::optional<sf::Event> &event) {
            if (const auto *textEntered = event->getIf<sf::Event::TextEntered>()) {
                handleTextEntered(textEntered->unicode);
            }
        }
    };

    class Text : public sf::Text {
    public:
        // using sf::Text's constructor
        using sf::Text::Text;

        // set the origin to the center the bounding box of the text
        void centerOrigin() {
            auto localRectangle = getLocalBounds();
            setOrigin({
                localRectangle.position.x + localRectangle.size.x / 2.f,
                localRectangle.position.y + localRectangle.size.y / 2.f
            });
        }

        float getWidth() const {
            auto localRectangle = getLocalBounds();
            return localRectangle.size.x;
        }

        // automatically find the bestt font size to fill the container
        void setAutoCharacterSize (float boxW, float boxH, float textFill = 0.8f) {
            setCharacterSize(100); // set a reference size
            auto localRectangle = getLocalBounds();

            float targetW = boxW * textFill;
            float scaleW = targetW / localRectangle.size.x;
            float targetH = boxH * textFill;
            float scaleH = targetH / localRectangle.size.y;

            float scale = std::min(scaleW, scaleH);
            float fontSize = std::max(1.f, 100.f * scale);
            setCharacterSize(static_cast<unsigned int>(fontSize));
        }
    };
};