/**
 * Since my original implementation Red-Black Tree is NOT robust
 * This implementation is inspired by Gemini's implementation: https://gemini.google.com/share/857bc839bad8
 */

#pragma once
#include <functional>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>

#include "../core/code-snippets.hpp"

namespace DS {

enum RBColor { RED, BLACK };

class RedBlackTree {
private:
    struct Node {
        int value, visualID, count;
        RBColor color;
        Node *parent, *lpt, *rpt;

        Node();
        Node (int value, int visualID);
    };
    Node *root, *TNULL;
    int nodeCounter;
    bool eraseSuccess;

    // callback functions
    std::function<void(int, bool)> callbackCreateNode;
    std::function<void(int)> callbackDeleteNode;
    std::function<void(int, int)> callbackSwapValue;
    std::function<void(int, int)> callbackColorNode;
    std::function<void(int, int, bool)> callbackAddEdge;
    std::function<void(int)> callbackChangeRoot;
    std::function<void()> callbackApplyAnimation;
    std::function<void(int)> callbackHighlightNode;
    std::function<void()> callbackCompleteAnimation;
    std::function<void(const std::vector<std::string>&)> callbackLoadCode;
    std::function<void(int)> callbackHighlightCode;
    
    // helper functions
    int getVisualID (Node* ptr);
    void initNullNode (Node* ptr, Node* parent);
    void leftRotation (Node* ptr);
    void rightRotation (Node* ptr);
    void recolor (Node* ptr);
    void insertFixup (Node* ptr);
    void eraseFixup (Node* ptr);
    void rbTransplant (Node* u, Node* v);
    Node* getSmallestKey (Node* ptr);

    bool searchValue (Node* ptr, int searchValue);
    void colorNodes (Node* ptr, int targetColor);
    Node* copyNodes (Node* otherNode, Node* otherTNULL, Node* parentPtr);
    void destroyTree (Node* ptr);
    void copyFrom (const RedBlackTree &other);
    
public:
    RedBlackTree();
    RedBlackTree (const RedBlackTree &other);
    ~RedBlackTree();
    RedBlackTree& operator= (const RedBlackTree &other);

    void setCallbackCreateNode (auto func) {
        callbackCreateNode = func;
    }

    void setCallbackDeleteNode(auto func) {
        callbackDeleteNode = func;
    }

    void setCallbackSwapValue (auto func) {
        callbackSwapValue = func;
    }

    void setCallbackColorNode(auto func) {
        callbackColorNode = func;
    }

    void setCallbackAddEdge(auto func) {
        callbackAddEdge = func;
    }

    void setCallbackChangeRoot(auto func) {
        callbackChangeRoot = func;
    }

    void setCallbackApplyAnimation(auto func) {
        callbackApplyAnimation = func;
    }

    void setCallbackHighlightNode(auto func) {
        callbackHighlightNode = func;
    }

    void setCallbackCompleteAnimation(auto func) {
        callbackCompleteAnimation = func;
    }

    void setCallbackLoadCode(auto func) {
        callbackLoadCode = func;
    }

    void setCallbackHighlightCode(auto func) {
        callbackHighlightCode = func;
    }

    void insert (int value, bool complete = true);

    bool erase (int value, bool complete = true);

    bool search (int value);

    bool update (int oldKey, int newKey);
};

}; // namespace DS