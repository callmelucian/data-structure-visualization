#include "../../include/animation/binary-tree-animation.hpp"

// ========== BINARY TREE CREATE NODE ========== //

BinaryTreeCreateNode::BinaryTreeCreateNode (const std::string &s) : msgString(s), isRoot(false) {}
BinaryTreeCreateNode::BinaryTreeCreateNode (const std::string &s, bool isRoot) : msgString(s), isRoot(isRoot) {}

bool BinaryTreeCreateNode::apply (UI::BinaryTree &ui) {
    ui.createNode(msgString, isRoot);
    return true;
}

// ========== BINARY TREE DELETE NODE ========== //

BinaryTreeDeleteNode::BinaryTreeDeleteNode (int nodeID) : nodeID(nodeID) {}

bool BinaryTreeDeleteNode::apply (UI::BinaryTree &ui) {
    ui.deleteNode(nodeID);
    return true;
}

// ========== BINARY TREE ADD EDGE ========== //

BinaryTreeAddEdge::BinaryTreeAddEdge (int parent, int childNode, bool isLeft) :
    parent(parent), childNode(childNode), isLeft(isLeft) {}

bool BinaryTreeAddEdge::apply (UI::BinaryTree &ui) {
    return ui.addEdge(parent, childNode, isLeft);
}

// ========== BINARY TREE CHANGE ROOT ========== //

BinaryTreeChangeRoot::BinaryTreeChangeRoot (int newRoot) : newRoot(newRoot) {}

bool BinaryTreeChangeRoot::apply (UI::BinaryTree &ui) {
    return ui.setRootNode(newRoot);
}

// ========== BINARY TREE CHANGE ROOT ========== //

BinaryTreeSwapValue::BinaryTreeSwapValue (int a, int b) : a(a), b(b) {}

bool BinaryTreeSwapValue::apply (UI::BinaryTree &ui) {
    ui.swapNode(a, b);
    return true;
}

// ========== BINARY TREE HIGHLIGHT NODE ========== //

BinaryTreeHighlightNode::BinaryTreeHighlightNode (int targetNode) : targetNode(targetNode) {}

bool BinaryTreeHighlightNode::apply (UI::BinaryTree &ui) {
    return ui.setHighlight(targetNode);
}

// ========== BINARY TREE LOCK HIGHLIGHT ========== //

BinaryTreeLockHighlight::BinaryTreeLockHighlight() {}

bool BinaryTreeLockHighlight::apply (UI::BinaryTree &ui) {
    return ui.lockHighlight(), false;
}