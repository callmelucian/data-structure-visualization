#pragma once
#include <SFML/Graphics.hpp>
#include "../assets/theme.hpp"
#include "nodes.hpp"
#include "edges.hpp"
#include "ui-base.hpp"

#include <vector>
#include <utility>
#include <set>
#include <memory>
#include <string>
#include <random>
#include <iostream>

const float delta = 400.f;
const float deltaHeight = 200.f;
std::mt19937 rng(21);

float randFloat (float L, float R) {
    return std::uniform_real_distribution<float>(L, R)(rng);
}

class TreeNode {
private:
    std::vector<float> leftBound, rightBound;
    std::set<int> childList;
    float subtreeWidth, x, y;
    int depth;
    
public:
    TreeNode() : subtreeWidth(0.0f), depth(0), x(0), y(0) {}
    
    int subtreeHeight() const { return leftBound.size(); }
    float getLeftBound (int d) const { return leftBound[d]; }
    float getRightBound (int d) const { return rightBound[d]; }
    void setNodeDepth (int d) { depth = d; }

    float getWidth() const { return subtreeWidth; }
    float getX() const { return x; }
    float getY() const { return y; }

    void setX (float _x) { x = _x; }
    void setY (float _y) { y = _y; }

    void offsetX (float delta) { x += delta; }
    void offsetY (float delta) { y += delta; }

    void addChild (int u) { childList.insert(u); }
    void removeChild (int u) { childList.erase(u); }
    
    friend float calculateOffset (const TreeNode &a, const TreeNode &b) {
        float offset = 0.f;
        for (int i = 0; i < std::min(a.subtreeHeight(), b.subtreeHeight()); i++)
            offset = std::max(offset, a.getRightBound(i) + delta - b.getLeftBound(i));
        return offset;
    }

    float mergeNode (const TreeNode &o) {
        float offset = calculateOffset(*this, o);
        for (int i = 0; i < o.subtreeHeight(); i++) {
            if (i < subtreeHeight()) {
                leftBound[i] = std::min(leftBound[i], o.getLeftBound(i) + offset);
                rightBound[i] = std::max(rightBound[i], o.getRightBound(i) + offset);
            }
            else {
                leftBound.push_back(o.getLeftBound(i) + offset);
                rightBound.push_back(o.getRightBound(i) + offset);
            }
        }
        subtreeWidth = std::max(subtreeWidth, *std::max_element(rightBound.begin(), rightBound.end()));
        return offset;
    }

    void prependRoot() {
        x = (childList.empty() ? 0.0 : (float)subtreeWidth / 2.0);
        leftBound.insert(leftBound.begin(), x);
        rightBound.insert(rightBound.begin(), x);
    }

    void resetPosition() { leftBound.clear(), rightBound.clear(), subtreeWidth = 0.f; }
    std::set<int>& getChildrenList() { return childList; }
};

class GraphicTree : public UI::Base {
private:
    std::vector<TreeNode> nodeData;
    std::vector<Node> nodeUI;
    std::set<std::pair<int,int>> edges;
    int rootNode, counter;

public:
    GraphicTree() : rootNode(0), counter(0) {}

    void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
        // apply state transform
        states.transform *= getTransform();

        // show circle and annotation
        for (const Node &node : nodeUI) target.draw(node, states);
        for (auto [a, b] : edges)
            target.draw(Edges(
                std::make_unique<Node>(nodeUI[a]),
                std::make_unique<Node>(nodeUI[b]), 3
            ), states);
    }

    int createNode (int value) {
        nodeData.emplace_back();
        nodeUI.emplace_back(std::to_string(value));
        nodeUI.back().setPosition({randFloat(0, 1800), randFloat(0, 800)});
        return counter++;
    }

    void addEdge (int parentNode, int childNode) {
        edges.emplace(parentNode, childNode);
        nodeData[parentNode].addChild(childNode);
    }

    void eraseEdge (int parentNode, int childNode) {
        edges.erase({parentNode, childNode});
        nodeData[parentNode].removeChild(childNode);
    }

    void setRoot (int u) { rootNode = u; }

    void applyOffset (int node, float amount) {
        nodeData[node].offsetX(amount);
        for (int v : nodeData[node].getChildrenList()) applyOffset(v, amount);
    }

    void calculateX (int node) {
        nodeData[node].resetPosition();
        for (int v : nodeData[node].getChildrenList()) {
            calculateX(v);
            float offset = nodeData[node].mergeNode(nodeData[v]);
            applyOffset(v, offset);
        }
        nodeData[node].prependRoot();
        // std::cerr << "calculate " << node << " " << nodeData[node].getX() << "\n";
    }

    void calculateY (int node, float curHeight, float hd) {
        nodeData[node].setY(curHeight);
        for (int v : nodeData[node].getChildrenList())
            calculateY(v, curHeight + hd, hd);
    }

    void calculatePosition (float maxWidth, float maxHeight) {
        calculateX(rootNode);
        float hd = std::min(deltaHeight, maxHeight / nodeData[rootNode].subtreeHeight());
        calculateY(rootNode, 0, hd);

        for (int i = 0; i < nodeUI.size(); i++)
            nodeUI[i].setPosition({nodeData[i].getX(), nodeData[i].getY()});
    }

    void printPosition() {
        for (const TreeNode &node : nodeData)
            std::cerr << "(" << node.getX() << ", " << node.getY() << ", " << node.getWidth() << ") ";
        std::cerr << std::endl;
    }

    void handleMousePress (const sf::Vector2f &mousePos) override {}
    void handleMouseMovement (const sf::Vector2f &mousePos) override {}
    void handleMouseRelease (const sf::Vector2f &mousePos) override {}
    void handleTextEntered (const char &unicode) override {}

    sf::FloatRect getBoundary() const override {
        return sf::FloatRect();
    }

    sf::FloatRect getLocalBounds() const {
        if (nodeUI.empty()) return sf::FloatRect();
        sf::FloatRect totalBounds = nodeUI[0].getGlobalBounds();
        for (const auto& circle : nodeUI) {
            sf::FloatRect itemBounds = circle.getGlobalBounds();
            float left   = std::min(totalBounds.position.x, itemBounds.position.x);
            float top    = std::min(totalBounds.position.y, itemBounds.position.y);
            float right  = std::max(totalBounds.position.x + totalBounds.size.x, 
                                    itemBounds.position.x + itemBounds.size.x);
            float bottom = std::max(totalBounds.position.y + totalBounds.size.y, 
                                    itemBounds.position.y + itemBounds.size.y);
            totalBounds.position = {left, top};
            totalBounds.size     = {right - left, bottom - top};
        }
        return totalBounds;
    }

    void centerOrigin() {
        auto localRectangle = getLocalBounds();
        setOrigin({
            localRectangle.position.x + localRectangle.size.x / 2.f,
            localRectangle.position.y + localRectangle.size.y / 2.f
        });
    }
};