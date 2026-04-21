/**
 * The implementation of Red-Black Tree is inspired by
 * Gemini's implementation: https://gemini.google.com/share/e2a6665370ad
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
        Node *lpt, *rpt;

        Node();
        Node (int value, int visualID);
    };
    Node* root;
    int nodeCounter;

    // callback functions
    std::function<void(int, bool)> callbackCreateNode;
    std::function<void(int)> callbackDeleteNode;
    std::function<void(int, int)> callbackSwapValue;
    std::function<void(int, bool)> callbackColorNode;
    std::function<void(int, int, bool)> callbackAddEdge;
    std::function<void(int)> callbackChangeRoot;
    std::function<void()> callbackApplyAnimation;
    std::function<void(int)> callbackHighlightNode;
    std::function<void()> callbackCompleteAnimation;
    std::function<void(const std::vector<std::string>&)> callbackLoadCode;
    std::function<void(int)> callbackHighlightCode;
    
    bool isRed (Node* ptr);

    int getVisualID (Node* ptr);

    Node* leftRotation (Node* ptr, Node* parent, bool isLeftChild);

    Node* rightRotation (Node* ptr, Node* parent, bool isLeftChild);

    /**
     * @brief Borrow from right sibling to make ptr or its child red (?)
     */
    Node* moveRedLeft (Node* ptr, Node* parent, bool leftChild);

    /**
     * @brief Borrow from left sibling to make ptr or its child red (?)
     */
    Node* moveRedRight (Node* ptr, Node* parent, bool leftChild);

    Node* selfBalancing (Node* ptr, Node* parent, bool leftChild);

    Node* getSmallestKey (Node* ptr);

    void flipColors (Node* ptr);

    void fixEdges (Node* parent, Node* ptr, bool leftChild);

    Node* insertValue (Node* ptr, Node* parent, bool leftChild, int insertKey);

    Node* eraseValue (Node* ptr, Node* parent, bool leftChild, int eraseKey);

    Node* copyNodes (Node* otherNode);

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

    void insert (int value);

    bool erase (int value);
};

}; // namespace DS