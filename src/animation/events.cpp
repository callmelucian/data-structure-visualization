#include "../../include/animation/events.hpp"

// ========== BINARY TREE CREATE NODE ========== //

BinaryTreeCreateNode::BinaryTreeCreateNode (const std::string &s) : msgString(s), isRoot(false) {}
BinaryTreeCreateNode::BinaryTreeCreateNode (const std::string &s, bool isRoot) : msgString(s), isRoot(isRoot) {}

int BinaryTreeCreateNode::apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) {
    ui.createNode(msgString, isRoot);
    return true;
}

// ========== BINARY TREE DELETE NODE ========== //

BinaryTreeDeleteNode::BinaryTreeDeleteNode (int nodeID) : nodeID(nodeID) {}

int BinaryTreeDeleteNode::apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) {
    ui.deleteNode(nodeID);
    return true;
}

// ========== BINARY TREE ADD EDGE ========== //

BinaryTreeAddEdge::BinaryTreeAddEdge (int parent, int childNode, bool isLeft) :
    parent(parent), childNode(childNode), isLeft(isLeft) {}

int BinaryTreeAddEdge::apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) {
    return ui.addEdge(parent, childNode, isLeft);
}

// ========== BINARY TREE CHANGE ROOT ========== //

BinaryTreeChangeRoot::BinaryTreeChangeRoot (int newRoot) : newRoot(newRoot) {}

int BinaryTreeChangeRoot::apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) {
    return ui.setRootNode(newRoot);
}

// ========== BINARY TREE CHANGE ROOT ========== //

BinaryTreeSwapValue::BinaryTreeSwapValue (int a, int b) : a(a), b(b) {}

int BinaryTreeSwapValue::apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) {
    ui.swapNode(a, b);
    return true;
}

// ========== BINARY TREE HIGHLIGHT NODE ========== //

BinaryTreeHighlightNode::BinaryTreeHighlightNode (int targetNode) : targetNode(targetNode) {}

int BinaryTreeHighlightNode::apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) {
    return ui.setHighlight(targetNode);
}

// ========== BINARY TREE LOCK HIGHLIGHT ========== //

BinaryTreeLockHighlight::BinaryTreeLockHighlight() {}

int BinaryTreeLockHighlight::apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) {
    return ui.lockHighlight(), false;
}

// ========== GRAPH CREATE NODE ========== //

GraphCreateNode::GraphCreateNode (int value) : value(value) {}

int GraphCreateNode::apply (UI::Graph &ui, UI::CodeHighlighter &code) {
    return ui.insertNode(value), true;
}

// ========== GRAPH ADD EDGE ========== //

GraphAddEdge::GraphAddEdge (int fromNode, int toNode, int weight) :
    fromNode(fromNode), toNode(toNode), weight(weight) {}

int GraphAddEdge::apply (UI::Graph &ui, UI::CodeHighlighter &code) {
    return ui.insertEdge(fromNode, toNode, weight), true;
}

// ========== GRAPH DELETE NODE ========== //

GraphDeleteNode::GraphDeleteNode (int nodeID) : nodeID(nodeID) {}

int GraphDeleteNode::apply (UI::Graph &ui, UI::CodeHighlighter &code) {
    return ui.deleteNode(nodeID), true;
}

// ========== GRAPH DELETE EDGE ========== //

GraphDeleteEdge::GraphDeleteEdge (int edgeID) : edgeID(edgeID) {}

int GraphDeleteEdge::apply (UI::Graph &ui, UI::CodeHighlighter &code) {
    return ui.deleteEdge(edgeID), true;
}

// ========== GRAPH EDIT EDGE ========== //

GraphEditEdge::GraphEditEdge (int edgeID, int newWeight) : edgeID(edgeID), newWeight(newWeight) {}

int GraphEditEdge::apply (UI::Graph &ui, UI::CodeHighlighter &code) {
    return ui.changeWeight(edgeID, newWeight), true;
}

// ========== GRAPH SET EDGE COLOR ========== //

GraphSetEdgeColor::GraphSetEdgeColor (int edgeID, const sf::Color &color) : edgeID(edgeID), color(color) {}

int GraphSetEdgeColor::apply (UI::Graph &ui, UI::CodeHighlighter &code) {
    return ui.setEdgeColor(edgeID, color), true;
}

// ========== GRAPH CLEAR ANNOTATION ========== //

GraphClearAnnotation::GraphClearAnnotation() {}

int GraphClearAnnotation::apply (UI::Graph &ui, UI::CodeHighlighter &code) {
    return ui.clearAnnotation(), true;
}

// ========== GRAPH EDIT ANNOTATION ========== //

GraphEditAnnotation::GraphEditAnnotation (int nodeID, int value) : nodeID(nodeID), value(value) {}

int GraphEditAnnotation::apply (UI::Graph &ui, UI::CodeHighlighter &code) {
    return ui.setAnnotation(nodeID, value), true;
}

// ========== GRAPH MARK ANNOTATION ========== //

GraphMarkAnnotation::GraphMarkAnnotation (int nodeID) : nodeID(nodeID) {}

int GraphMarkAnnotation::apply (UI::Graph &ui, UI::CodeHighlighter &code) {
    return ui.markAnnotation(nodeID), true;
}

// ========== GRAPH HIGHLIGHT NODE ========== //

GraphHighlightNode::GraphHighlightNode (int nodeID) : nodeID(nodeID) {}

int GraphHighlightNode::apply (UI::Graph &ui, UI::CodeHighlighter &code) {
    return ui.highlightNode(nodeID), true;
}

// ========== GRAPH HIGHLIGHT EDGE ========== //

GraphHighlightEdge::GraphHighlightEdge (int edgeID) : edgeID(edgeID) {}

int GraphHighlightEdge::apply (UI::Graph &ui, UI::CodeHighlighter &code) {
    return ui.highlightEdge(edgeID), true;
}

// ========== COMPLETE ANIMATION ========== //

template <typename TypeUI>
CompleteAnimation<TypeUI>::CompleteAnimation() {}

template <typename TypeUI>
int CompleteAnimation<TypeUI>::apply (TypeUI &ui, UI::CodeHighlighter &code) {
    return 2;
}

// ========== CODE HIGHLIGHT: SET CODE ==========

template <typename TypeUI>
CodeHighlightLoadCode<TypeUI>::CodeHighlightLoadCode (const std::vector<std::string> &vec) : vec(vec) {}

template <typename TypeUI>
int CodeHighlightLoadCode<TypeUI>::apply (TypeUI &ui, UI::CodeHighlighter &code) {
    return code.loadCode(vec), true;
}

// ========== CODE HIGHLIGHT: HIGHLIGHT CODE ==========

template <typename TypeUI>
CodeHighlighting<TypeUI>::CodeHighlighting (int row) : row(row) {}

template <typename TypeUI>
int CodeHighlighting<TypeUI>::apply (TypeUI &ui, UI::CodeHighlighter &code) {
    return code.highlightCode(row), true;
}