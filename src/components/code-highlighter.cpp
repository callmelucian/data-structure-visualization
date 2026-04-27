#include "../../include/components/code-highlighter.hpp"

namespace UI {

CodeHighlighter::CodeHighlighter() :
    highlightedRow(-1), lineSpacing(20.f), titleBarHeight(0.f), fontSize(16), shown(false) {
    background.setFillColor(Theme::getAccentSecondary());
    highlightBar.setFillColor(Theme::getAccentPrimary());

    background.setSize({Setting::codeWidth, Setting::screenHeight});
    highlightBar.setSize({Setting::codeWidth, lineSpacing}); 
}

// void CodeHighlighter::loadCode(const std::vector<std::string>& codes) {
//     textLines.clear();
//     highlightedRow = -1;

//     sf::Vector2f bgSize = background.getSize();
//     float totalTextHeight = codes.size() * lineSpacing;
//     float startY = (bgSize.y - totalTextHeight) / 2.0f;

//     for (size_t i = 0; i < codes.size(); ++i) {
//         UI::Text text(Theme::googleSansItalic);
//         text.setString(codes[i]);
//         text.setCharacterSize(fontSize);
//         text.setFillColor(Theme::getTextPrimary());
//         text.centerOrigin();

//         float posX = bgSize.x / 2.0f;
//         float posY = startY + (i * lineSpacing) + (lineSpacing / 2.0f);
//         text.setPosition({posX, posY});
//         textLines.push_back(text);
//     }
// }

void CodeHighlighter::loadCode(const std::vector<std::string>& codes) {
    textLines.clear();
    highlightedRow = -1;

    sf::Vector2f bgSize = background.getSize();
    float maxTextWidth = 0.f;

    for (const auto& codeLine : codes) {
        UI::Text text(Theme::googleSansItalic);
        text.setString(codeLine);
        text.setCharacterSize(fontSize);
        text.setFillColor(Theme::getTextPrimary());
        text.centerOrigin();

        float currentWidth = text.getWidth();
        if (currentWidth > maxTextWidth) maxTextWidth = currentWidth;
        textLines.push_back(text);
    }

    float totalTextHeight = codes.size() * lineSpacing;
    float blockStartX = (bgSize.x - maxTextWidth) / 2.0f;
    float blockStartY = (bgSize.y - totalTextHeight) / 2.0f;

    for (size_t i = 0; i < textLines.size(); ++i) {
        float textWidth = textLines[i].getWidth();
        float posX = blockStartX + (textWidth / 2.0f);
        float posY = blockStartY + (i * lineSpacing) + (lineSpacing / 2.0f);

        textLines[i].setPosition({posX, posY});
    }
}

void CodeHighlighter::highlightCode(int row) {
    if (row < 0 || row >= static_cast<int>(textLines.size())) return;
    float totalTextHeight = textLines.size() * lineSpacing;
    float startY = (background.getSize().y - totalTextHeight) / 2.0f;
    
    targetHighlightPos = {0.f, startY + row * lineSpacing};
    if (highlightedRow == -1) highlightBar.setPosition(targetHighlightPos);
    highlightedRow = row;
}

void CodeHighlighter::timePropagation(float deltaTime) {
    // move highlight bar
    if (highlightedRow != -1) {
        sf::Vector2f displacement = targetHighlightPos - highlightBar.getPosition();
        if (magnitude(displacement) < eps) highlightBar.setPosition(targetHighlightPos);
        else {
            sf::Vector2f newPosition = highlightBar.getPosition() + displacement * Setting::animationFactor() * deltaTime * 10.f;
            highlightBar.setPosition(newPosition);
        }
    }

    // move code highlighter
    sf::Vector2f displacement = targetPosition - getPosition();
    if (magnitude(displacement) < eps) setPosition(targetPosition);
    else {
        sf::Vector2f newPosition = getPosition() + displacement * Setting::animationFactor() * deltaTime * 5.f;
        setPosition(newPosition);
    }
}

void CodeHighlighter::copyPosition(const CodeHighlighter &other) {
    highlightBar.setPosition(other.highlightBar.getPosition());
    shown = other.isShown();
    setPosition(other.getPosition());
}

void CodeHighlighter::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform(); 

    target.draw(background, states);
    if (highlightedRow != -1) target.draw(highlightBar, states);
    for (const auto& text : textLines) target.draw(text, states);
}

bool CodeHighlighter::isShown() const {
    return shown;
}

void CodeHighlighter::show() {
    shown = true;
}

void CodeHighlighter::hide() {
    shown = false;
}

void CodeHighlighter::setTargetPosition (float x, float y) {
    targetPosition = {x, y};
}

sf::FloatRect CodeHighlighter::getBoundary() const {
    return sf::FloatRect();
}

void CodeHighlighter::handleMousePress(const sf::Vector2f &mousePos) {}
void CodeHighlighter::handleMouseRelease(const sf::Vector2f &mousePos) {}
void CodeHighlighter::handleMouseMovement(const sf::Vector2f &mousePos) {}
void CodeHighlighter::handleTextEntered(const char &unicode) {}

} // namespace UI