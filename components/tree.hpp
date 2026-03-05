#pragma once
#include <SFML/Graphics.hpp>
#include "../assets/theme.hpp"
#include "nodes.hpp"
#include "edges.hpp"
#include <vector>
#include <utility>
#include <set>
#include <memory>
#include <string>
#include <random>

const float delta = 10.f;
std::mt19937 rng(21);

float randFloat (float L, float R) {
    return std::uniform_real_distribution<float>(L, R)(rng);
}

class TreeNode : public FloatingNode {
private:
    std::vector<float> leftBound, rightBound;
    std::set<int> childList;
    float subtreeWidth;
    int depth;
    
public:
    TreeNode (int value) : subtreeWidth(0.0f), depth(0), FloatingNode(25.0, std::to_string(value), Theme::ibmRegular, 30, 2.5) {}
    
    int subtreeHeight() const { return leftBound.size(); }
    float getLeftBound (int d) const { return leftBound[d]; }
    float getRightBound (int d) const { return rightBound[d]; }
    void setNodeDepth (int d) { depth = d; }

    void addChild (int u) { childList.insert(u); }
    void removeChild (int u) { childList.erase(u); }
    
    friend float calculateOffset (const TreeNode &a, const TreeNode &b) {
        float offset = 0.f;
        for (int i = 0; i < std::min(a.subtreeHeight(), b.subtreeHeight()); i++)
            offset = std::max(offset, a.getRightBound(i) + delta - b.getLeftBound(i));
        return offset;
    }

    void mergeNode (const TreeNode &o) {
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
            subtreeWidth = std::max(subtreeWidth, rightBound[i]);
        }
    }

    void prependRoot() {
        float position = (float)subtreeWidth / (float)childList.size();
        leftBound.insert(leftBound.begin(), position);
        rightBound.insert(rightBound.begin(), position);
    }

    void resetPosition() { leftBound.clear(), rightBound.clear(), subtreeWidth = 0.f; }
    std::set<int>& getChildrenList() { return childList; }
};

class GraphicTree : public std::vector<TreeNode>, public sf::Drawable, public sf::Transformable {
private:
    std::set<std::pair<int,int>> edges;
    int rootNode;

public:
    void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
        // apply state transform
        states.transform *= getTransform();

        // show circle and annotation
        for (const FloatingNode &node : (*this)) target.draw(node, states);
        for (auto [a, b] : edges)
            target.draw(Edges(
                std::make_unique<TreeNode>(at(a)),
                std::make_unique<TreeNode>(at(b)), 3
            ));
    }

    int createNode (int value) {
        emplace_back(value);
        back().setPosition({randFloat(0, 1800), randFloat(0, 800)});
        return (int)size() - 1;
    }

    void addEdge (int parentNode, int childNode) {
        edges.emplace(parentNode, childNode);
        at(parentNode).addChild(childNode);
    }

    void eraseEdge (int parentNode, int childNode) {
        edges.erase({parentNode, childNode});
        at(parentNode).removeChild(childNode);
    }

    void setRoot (int u) { rootNode = u; }

    void calculateDepth (int node) {
        at(node).resetPosition();
        for (int v : at(node).getChildrenList()) at(node).mergeNode(at(v));
        at(node).prependRoot();
    }
};