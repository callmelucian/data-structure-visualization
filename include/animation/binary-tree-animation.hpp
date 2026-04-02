#pragma once
#include "../components/binary-tree.hpp"
#include "animation-event.hpp"

class BinaryTreeCreateNode : public AnimationEvent<UI::BinaryTree> {
private:
    std::string msgString;
    bool isRoot;
public:
    BinaryTreeCreateNode (const std::string &s);
    BinaryTreeCreateNode (const std::string &s, bool isRoot);

    bool apply (UI::BinaryTree &ui) override;
};

class BinaryTreeDeleteNode : public AnimationEvent<UI::BinaryTree> {
private:
    int nodeID;
public:
    BinaryTreeDeleteNode (int nodeID);

    bool apply (UI::BinaryTree &ui) override;
};

class BinaryTreeAddEdge : public AnimationEvent<UI::BinaryTree> {
private:
    int parent, childNode;
    bool isLeft;
public:
    BinaryTreeAddEdge (int parent, int childNode, bool isLeft);
    
    bool apply (UI::BinaryTree &ui) override;
};

class BinaryTreeChangeRoot : public AnimationEvent<UI::BinaryTree> {
private:
    int newRoot;
public:
    BinaryTreeChangeRoot (int newRoot);

    bool apply (UI::BinaryTree &ui) override;
};

class BinaryTreeSwapValue : public AnimationEvent<UI::BinaryTree> {
private:
    int a, b;
public:
    BinaryTreeSwapValue (int a, int b);

    bool apply (UI::BinaryTree &ui) override;
};

class BinaryTreeHighlightNode : public AnimationEvent<UI::BinaryTree> {
private:
    int targetNode;
public:
    BinaryTreeHighlightNode (int targetNode);

    bool apply (UI::BinaryTree &ui) override;
};

class BinaryTreeLockHighlight : public AnimationEvent<UI::BinaryTree> {
public:
    BinaryTreeLockHighlight();

    bool apply (UI::BinaryTree &ui) override;
};