#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm>

#include "../../assets/theme.hpp"
#include "ui-base.hpp"

namespace UI {

class CodeHighlighter : public UI::Base {
private:
    std::vector<UI::Text> textLines;
    sf::RectangleShape background, highlightBar, titleBar;
    UI::Text titleText;

    int highlightedRow;
    float lineSpacing, padding, titleBarHeight;
    unsigned int fontSize;
    bool followMouse;
    sf::Vector2f grabOffset;

    void updateBounds();
public:
    CodeHighlighter();

    void loadCode(const std::vector<std::string>& codes);
    void highlightCode(int row);
    void copyPosition (const CodeHighlighter &other);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // overwritten functions
    sf::FloatRect getBoundary() const override;
    void handleMousePress (const sf::Vector2f &mousePos) override;
    void handleMouseRelease (const sf::Vector2f &mousePos) override;
    void handleMouseMovement (const sf::Vector2f &mousePos) override;
    void handleTextEntered (const char &unicode) override;
};

} // namespace UI