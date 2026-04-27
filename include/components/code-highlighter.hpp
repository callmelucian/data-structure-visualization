#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm>

#include "../../assets/theme.hpp"
#include "ui-base.hpp"
#include "../core/utility.hpp"

namespace UI {

class CodeHighlighter : public UI::Base {
private:
    std::vector<UI::Text> textLines; 
    sf::RectangleShape background, highlightBar;
    int highlightedRow;
    float lineSpacing, titleBarHeight;
    unsigned int fontSize;
    bool shown;
    sf::Vector2f targetHighlightPos, targetPosition;
public:
    CodeHighlighter();

    void loadCode(const std::vector<std::string>& codes);
    void highlightCode(int row);
    void copyPosition(const CodeHighlighter &other);
    void timePropagation(float deltaTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool isShown() const;
    void show();
    void hide();
    void setTargetPosition (float x, float y);

    // overrides
    sf::FloatRect getBoundary() const override;
    void handleMousePress(const sf::Vector2f &mousePos) override;
    void handleMouseRelease(const sf::Vector2f &mousePos) override;
    void handleMouseMovement(const sf::Vector2f &mousePos) override;
    void handleTextEntered(const char &unicode) override;
};

} // namespace UI