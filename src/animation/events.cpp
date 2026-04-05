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

// ========== BINARY TREE COMPLETE ANIMATION ========== //

BinaryTreeCompleteAnimation::BinaryTreeCompleteAnimation() {}

int BinaryTreeCompleteAnimation::apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) {
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