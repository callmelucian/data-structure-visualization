#pragma once
#include "../components/binary-tree.hpp"
#include "../components/code-highlighter.hpp"

template <typename TypeUI>
class AnimationEvent {
private:
public:
    virtual ~AnimationEvent() = default;
    virtual int apply (TypeUI &ui, UI::CodeHighlighter &code) = 0;
};

class BinaryTreeCreateNode : public AnimationEvent<UI::BinaryTree> {
private:
    std::string msgString;
    bool isRoot;
public:
    BinaryTreeCreateNode (const std::string &s);
    BinaryTreeCreateNode (const std::string &s, bool isRoot);

    int apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) override;
};

class BinaryTreeDeleteNode : public AnimationEvent<UI::BinaryTree> {
private:
    int nodeID;
public:
    BinaryTreeDeleteNode (int nodeID);

    int apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) override;
};

class BinaryTreeAddEdge : public AnimationEvent<UI::BinaryTree> {
private:
    int parent, childNode;
    bool isLeft;
public:
    BinaryTreeAddEdge (int parent, int childNode, bool isLeft);
    
    int apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) override;
};

class BinaryTreeChangeRoot : public AnimationEvent<UI::BinaryTree> {
private:
    int newRoot;
public:
    BinaryTreeChangeRoot (int newRoot);

    int apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) override;
};

class BinaryTreeSwapValue : public AnimationEvent<UI::BinaryTree> {
private:
    int a, b;
public:
    BinaryTreeSwapValue (int a, int b);

    int apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) override;
};

class BinaryTreeHighlightNode : public AnimationEvent<UI::BinaryTree> {
private:
    int targetNode;
public:
    BinaryTreeHighlightNode (int targetNode);

    int apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) override;
};

class BinaryTreeLockHighlight : public AnimationEvent<UI::BinaryTree> {
public:
    BinaryTreeLockHighlight();

    int apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) override;
};

class BinaryTreeCompleteAnimation : public AnimationEvent<UI::BinaryTree> {
public:
    BinaryTreeCompleteAnimation();

    int apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) override;
};

template <typename TypeUI>
class CodeHighlightLoadCode : public AnimationEvent<TypeUI> {
private:
    std::vector<std::string> vec;
    
public:
    CodeHighlightLoadCode(const std::vector<std::string> &vec);
    int apply (TypeUI &ui, UI::CodeHighlighter &code) override;
};

template class CodeHighlightLoadCode<UI::BinaryTree>;

template <typename TypeUI>
class CodeHighlighting : public AnimationEvent<TypeUI> {
private:
    int row;

public:
    CodeHighlighting(int row);
    int apply (TypeUI &ui, UI::CodeHighlighter &code) override;
};

template class CodeHighlighting<UI::BinaryTree>;