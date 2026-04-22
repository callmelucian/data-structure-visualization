#pragma once

#include "../components/binary-tree.hpp"
#include "../components/code-highlighter.hpp"
#include "../components/graph.hpp"
#include "../components/hash-map.hpp"

#include "../../assets/theme.hpp"

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

class BinaryTreeColorNode : public AnimationEvent<UI::BinaryTree> {
private:
    int nodeID;
    bool color;
public:
    BinaryTreeColorNode (int nodeID, bool color);

    int apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) override;
};

class BinaryTreeLockHighlight : public AnimationEvent<UI::BinaryTree> {
public:
    BinaryTreeLockHighlight();

    int apply (UI::BinaryTree &ui, UI::CodeHighlighter &code) override;
};

// ========== GRAPH ==========

class GraphCreateNode : public AnimationEvent<UI::Graph> {
private:
    int value;

public:
    GraphCreateNode (int value);

    int apply (UI::Graph &ui, UI::CodeHighlighter &code) override;
};

class GraphAddEdge : public AnimationEvent<UI::Graph> {
private:
    int fromNode, toNode, weight;

public:
    GraphAddEdge (int fromNode, int toNode, int weight);

    int apply (UI::Graph &ui, UI::CodeHighlighter &code) override;
};

class GraphDeleteNode : public AnimationEvent<UI::Graph> {
private:
    int nodeID;

public:
    GraphDeleteNode (int nodeID);

    int apply (UI::Graph &ui, UI::CodeHighlighter &code) override;
};

class GraphDeleteEdge : public AnimationEvent<UI::Graph> {
private:
    int edgeID;

public:
    GraphDeleteEdge (int edge);

    int apply (UI::Graph &ui, UI::CodeHighlighter &code) override;
};

class GraphEditEdge : public AnimationEvent<UI::Graph> {
private:
    int edgeID, newWeight;

public:
    GraphEditEdge (int edgeID, int newWeight);

    int apply (UI::Graph &ui, UI::CodeHighlighter &code) override;
};

class GraphSetEdgeColor : public AnimationEvent<UI::Graph> {
private:
    int edgeID;
    sf::Color color;
public:
    GraphSetEdgeColor (int edgeID, const sf::Color &color);
    
    int apply (UI::Graph &ui, UI::CodeHighlighter &code) override;
};

class GraphClearAnnotation : public AnimationEvent<UI::Graph> {
public:
    GraphClearAnnotation();

    int apply (UI::Graph &ui, UI::CodeHighlighter &code) override;
};

class GraphEditAnnotation : public AnimationEvent<UI::Graph> {
private:
    int nodeID, value;
public:
    GraphEditAnnotation (int nodeID, int value);

    int apply (UI::Graph &ui, UI::CodeHighlighter &code) override;
};

class GraphMarkAnnotation : public AnimationEvent<UI::Graph> {
private:
    int nodeID;
public:
    GraphMarkAnnotation (int nodeID);

    int apply (UI::Graph &ui, UI::CodeHighlighter &code) override;
};

class GraphHighlightNode : public AnimationEvent<UI::Graph> {
private:
    int nodeID;
public:
    GraphHighlightNode (int nodeID);

    int apply (UI::Graph &ui, UI::CodeHighlighter &code) override;
};

class GraphHighlightEdge : public AnimationEvent<UI::Graph> {
private:
    int edgeID;
public:
    GraphHighlightEdge (int edgeID);

    int apply (UI::Graph &ui, UI::CodeHighlighter &code) override;
};

// ========== HASH MAP ==========
class HashMapCreateNode : public AnimationEvent<UI::HashMap> {
private:
    int value;
public:
    HashMapCreateNode (int value);
    int apply (UI::HashMap &ui, UI::CodeHighlighter &code) override;
};

class HashMapDeleteNode : public AnimationEvent<UI::HashMap> {
private:
    int nodeID;
public:
    HashMapDeleteNode (int nodeID);
    int apply (UI::HashMap &ui, UI::CodeHighlighter &code) override;
};

class HashMapAddEdge : public AnimationEvent<UI::HashMap> {
private:
    int fromID, toID;
public:
    HashMapAddEdge (int fromID, int nodeID);
    int apply (UI::HashMap &ui, UI::CodeHighlighter &code) override;
};

class HashMapAttachRoot : public AnimationEvent<UI::HashMap> {
private:
    int slot, nodeID;
public:
    HashMapAttachRoot (int slot, int nodeID);
    int apply (UI::HashMap &ui, UI::CodeHighlighter &code) override;
};

class HashMapHighlightNode : public AnimationEvent<UI::HashMap> {
private:
    int nodeID;
public:
    HashMapHighlightNode (int nodeID);
    int apply (UI::HashMap &ui, UI::CodeHighlighter &code) override;
};

// ========== COMMON ==========

template <typename TypeUI>
class CodeHighlightLoadCode : public AnimationEvent<TypeUI> {
private:
    std::vector<std::string> vec;
    
public:
    CodeHighlightLoadCode(const std::vector<std::string> &vec);
    int apply (TypeUI &ui, UI::CodeHighlighter &code) override;
};


template <typename TypeUI>
class CodeHighlighting : public AnimationEvent<TypeUI> {
private:
    int row;

public:
    CodeHighlighting(int row);
    int apply (TypeUI &ui, UI::CodeHighlighter &code) override;
};

template <typename TypeUI>
class CompleteAnimation: public AnimationEvent<TypeUI> {
public:
CompleteAnimation();

int apply (TypeUI &ui, UI::CodeHighlighter &code) override;
};

// ========== TEMPLATE CLASSES ==========

template class CodeHighlightLoadCode<UI::BinaryTree>;
template class CodeHighlightLoadCode<UI::Graph>;
template class CodeHighlightLoadCode<UI::HashMap>;

template class CodeHighlighting<UI::BinaryTree>;
template class CodeHighlighting<UI::Graph>;
template class CodeHighlighting<UI::HashMap>;

template class CompleteAnimation<UI::BinaryTree>;
template class CompleteAnimation<UI::Graph>;
template class CompleteAnimation<UI::HashMap>;