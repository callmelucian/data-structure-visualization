#include "../../include/components/code-highlighter.hpp"

namespace UI {

CodeHighlighter::CodeHighlighter() : highlightedRow(-1), lineSpacing(1.2f), padding(21.f), fontSize(18) {
    background.setFillColor(sf::Color::White);
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(2.f);

    highlightBar.setFillColor(sf::Color(200, 200, 200, 150)); 
}

void CodeHighlighter::loadCode(const std::vector<std::string>& codes) {
    textLines.clear();
    highlightedRow = -1;

    for (const auto& str : codes) {
        UI::Text text(Theme::ibmRegular, str, fontSize);
        text.setFillColor(sf::Color::Black);
        textLines.push_back(text);
    }
    updateBounds();
}

void CodeHighlighter::highlightCode(int row) {
    if (row >= 0 && row < (int)textLines.size()) {
        highlightedRow = row;
        float lineStep = fontSize * lineSpacing;
        float barY = padding + (row * lineStep);
        highlightBar.setPosition({0.f, barY});
    }
    else highlightedRow = -1;
}

void CodeHighlighter::updateBounds() {
    if (textLines.empty())
        return background.setSize({0.f, 0.f}), void();

    float maxWidth = 0.f;
    for (const UI::Text& line : textLines)
        maxWidth = std::max(maxWidth, line.getWidth());

    float lineStep = fontSize * lineSpacing;
    float totalContentHeight = textLines.size() * lineStep;

    sf::Vector2f boxSize(maxWidth + (padding * 2.f), totalContentHeight + (padding * 1.5f));
    background.setSize(boxSize);
    highlightBar.setSize({boxSize.x, lineStep});
    setOrigin({boxSize.x, boxSize.y});
    for (int i = 0; i < textLines.size(); i++)
        textLines[i].setPosition({padding, padding + (i * lineStep)});
}

void CodeHighlighter::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(background, states);
    if (highlightedRow != -1)
        target.draw(highlightBar, states);
    for (const auto& line : textLines)
        target.draw(line, states);
}

sf::FloatRect CodeHighlighter::getBoundary() const {
    return sf::FloatRect();
}

void CodeHighlighter::handleMousePress (const sf::Vector2f &mousePos) {}
void CodeHighlighter::handleMouseRelease (const sf::Vector2f &mousePos) {}
void CodeHighlighter::handleMouseMovement (const sf::Vector2f &mousePos) {}
void CodeHighlighter::handleTextEntered (const char &unicode) {}

} // namespace UI