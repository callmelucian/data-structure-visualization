#include "../../include/animation/binary-tree-animation.hpp"

// ========== BINARY TREE CREATE NODE ========== //

BinaryTreeCreateNode::BinaryTreeCreateNode (const std::string &s) : msgString(s), isRoot(false) {}
BinaryTreeCreateNode::BinaryTreeCreateNode (const std::string &s, bool isRoot) : msgString(s), isRoot(isRoot) {}

void BinaryTreeCreateNode::apply (UI::BinaryTree &ui) {
    ui.createNode(msgString, isRoot);
}

// ========== BINARY TREE DELETE NODE ========== //

BinaryTreeDeleteNode::BinaryTreeDeleteNode (int nodeID) : nodeID(nodeID) {}

void BinaryTreeDeleteNode::apply (UI::BinaryTree &ui) {
    ui.deleteNode(nodeID);
}

// ========== BINARY TREE ADD EDGE ========== //

BinaryTreeAddEdge::BinaryTreeAddEdge (int parent, int childNode, bool isLeft) :
    parent(parent), childNode(childNode), isLeft(isLeft) {}

void BinaryTreeAddEdge::apply (UI::BinaryTree &ui) {
    ui.addEdge(parent, childNode, isLeft);
}

// ========== BINARY TREE CHANGE ROOT ========== //

BinaryTreeChangeRoot::BinaryTreeChangeRoot (int newRoot) : newRoot(newRoot) {}

void BinaryTreeChangeRoot::apply (UI::BinaryTree &ui) {
    ui.setRootNode(newRoot);
}