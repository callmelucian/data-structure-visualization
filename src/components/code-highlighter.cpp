#include "../../include/components/code-highlighter.hpp"

namespace UI {

CodeHighlighter::CodeHighlighter() 
    : highlightedRow(-1), lineSpacing(1.2f), padding(21.f), 
      titleBarHeight(30.f), fontSize(16), followMouse(false), titleText(Theme::googleSansBoldItalic) 
{
    background.setFillColor(sf::Color({255, 255, 255, 150}));
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(2.f);

    titleBar.setFillColor(sf::Color::Black);
    titleBar.setOutlineColor(sf::Color::Black);
    titleBar.setOutlineThickness(2.f);
    highlightBar.setFillColor(sf::Color({200, 200, 200, 150})); 

    titleText.setCharacterSize(15);
    titleText.setFillColor(sf::Color::White);
    titleText.setString("SOURCE CODE");
    loadCode({});
}

void CodeHighlighter::loadCode(const std::vector<std::string>& codes) {
    textLines.clear();
    highlightedRow = -1;

    for (const auto& str : codes) {
        UI::Text text(Theme::googleSansItalic, str, fontSize);
        text.setFillColor(sf::Color::Black);
        textLines.push_back(text);
    }
    updateBounds();
}

void CodeHighlighter::highlightCode(int row) {
    if (row >= 0 && row < (int)textLines.size()) {
        highlightedRow = row;
        float lineStep = fontSize * lineSpacing;
        float barY = titleBarHeight + padding + (row * lineStep);
        highlightBar.setPosition({0.f, barY});
    }
    else highlightedRow = -1;
}

void CodeHighlighter::updateBounds() {
    float maxWidth = 0.f;
    for (const UI::Text& line : textLines)
        maxWidth = std::max(maxWidth, line.getWidth());

    float finalWidth = std::max(maxWidth + (padding * 2.f), 250.f);
    float lineStep = fontSize * lineSpacing;
    float totalContentHeight = textLines.size() * lineStep;
    
    background.setSize({finalWidth, totalContentHeight + (padding * 1.5f)});
    background.setPosition({0.f, titleBarHeight});

    titleBar.setSize({finalWidth, titleBarHeight});
    titleBar.setPosition({0.f, 0.f});

    titleText.centerOrigin(); 
    titleText.setPosition({finalWidth / 2.f, titleBarHeight / 2.f});

    highlightBar.setSize({finalWidth, lineStep});
    setOrigin({finalWidth, background.getSize().y + titleBarHeight});

    for (int i = 0; i < (int)textLines.size(); i++) {
        textLines[i].setPosition({padding, titleBarHeight + padding + (i * lineStep)});
    }
}

void CodeHighlighter::copyPosition (const CodeHighlighter &other) {
    setPosition(other.getPosition());
    followMouse = other.followMouse;
    grabOffset = other.grabOffset;
}

void CodeHighlighter::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    target.draw(background, states);
    target.draw(titleBar, states);
    target.draw(titleText, states); // Draw title on top of the bar
    
    if (highlightedRow != -1)
        target.draw(highlightBar, states);
        
    for (const auto& line : textLines)
        target.draw(line, states);
}

sf::FloatRect CodeHighlighter::getBoundary() const {
    return background.getGlobalBounds();
}

void CodeHighlighter::handleMousePress (const sf::Vector2f &mousePos) {
    if (!containPosition(mousePos)) return;
    grabOffset = getOrigin() - this->getInverseTransform().transformPoint(mousePos);
    followMouse = true;
}

void CodeHighlighter::handleMouseRelease (const sf::Vector2f &mousePos) {
    followMouse = false;
}

void CodeHighlighter::handleMouseMovement (const sf::Vector2f &mousePos) {
    if (!followMouse) return;
    // sf::Vector2f localPos = this->getInverseTransform().transformPoint(mousePos);
    setPosition(grabOffset + mousePos);
}

void CodeHighlighter::handleTextEntered (const char &unicode) {}

} // namespace UI