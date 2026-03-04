#pragma once
#include <SFML/Graphics.hpp>
#include "../assets/theme.hpp"
#include <string>

const std::string appName = "CS163 Visualization App";

class ActionBar : public sf::Drawable, public sf::Transformable {
private:
    sf::Text appTitle, subtitle;
    sf::RectangleShape rectangle;

public:
    ActionBar (float width, float height, const sf::Color &fillColor, const sf::Color &textColor, const std::string &msg) :
    appTitle(Theme::notoCondensedBold, appName), subtitle(Theme::notoCondensed, msg) {
        // setup background rectangle for action bar
        rectangle.setSize({width, height});
        rectangle.setOrigin({width / 2.f, height / 2.f});
        rectangle.setFillColor(fillColor);

        // setup annotation
        appTitle.setCharacterSize(48);
        appTitle.setFillColor(textColor);
        subtitle.setCharacterSize(28);
        subtitle.setFillColor(textColor);

        sf::FloatRect localRectangle = appTitle.getLocalBounds();
        appTitle.setOrigin({
            localRectangle.position.x + localRectangle.size.x / 2.f,
            localRectangle.position.y + localRectangle.size.y / 2.f
        });

        localRectangle = subtitle.getLocalBounds();
        subtitle.setOrigin({
            localRectangle.position.x + localRectangle.size.x / 2.f,
            localRectangle.position.y + localRectangle.size.y / 2.f
        });
        
        appTitle.setPosition({0, -12});
        subtitle.setPosition({0, 28});
    }

    void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
        // apply state transform
        states.transform *= getTransform();

        // show rectangle and label
        target.draw(rectangle, states);
        target.draw(appTitle, states);
        target.draw(subtitle, states);
    }
};