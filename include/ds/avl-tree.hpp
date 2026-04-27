#pragma once
#include <functional>
#include <algorithm>
#include <iostream>
#include <cassert>

#include "../core/code-snippets.hpp"

namespace DS {
    
class AVLTree {
private:
    struct Node {
        int value, visualID, height, count;
        Node *lpt, *rpt;
    
        Node();
        Node(int value, int visualID);
    };
    Node *root;
    int nodeCounter;
    bool eraseSuccess;

    std::function<void(int, bool)> callbackCreateNode;
    std::function<void(int)> callbackDeleteNode;
    std::function<void(int, int)> callbackSwapValue;
    std::function<void(int, int, bool)> callbackAddEdge;
    std::function<void(int)> callbackChangeRoot;
    std::function<void()> callbackApplyAnimation;
    std::function<void(int)> callbackHighlightNode;
    std::function<void(int,bool)> callbackColorNode;
    std::function<void()> callbackCompleteAnimation;
    std::function<void(const std::vector<std::string>&)> callbackLoadCode;
    std::function<void(int)> callbackHighlightCode;

    /**
     * @brief Get the height of the node given by ptr in O(1)
     */
    int getHeight(Node *ptr);

    /**
     * @brief Get the balance factor of the node given by ptr in O(1)
     */
    int getBalance(Node *ptr);

    /**
     * @brief Get the visual ID of a node, if nullptr, return -1
     */
    int getVisualID(Node *ptr);

    /**
     * @brief Update the height of the node given by ptr in O(1)
     */
    void updateHeight(Node *ptr);

    /**
     * @brief Perform left rotation on the subtree given by ptr
     * and return the new root in O(1)
     */
    Node* leftRotation(Node* ptr, Node* parent, bool isLeftChild);

    /**
     * @brief Perform right rotation on the subtree given by ptr
     * and return the new root in O(1)
     */
    Node* rightRotation(Node* ptr, Node* parent, bool isLeftChild);

    /**
     * @brief Perform self balancing in O(1)
     */
    Node* selfBalancing(Node *ptr, Node* parent, bool leftChild);

    /**
     * @brief Return the pointer pointing the smallest key
     * within the subtree given by ptr in O(height)
     */
    Node* getSmallestKey(Node *ptr);

    void fixEdges (Node* ptr, Node* parent, bool leftChild);

    /**
     * @brief Perform insertion on the subtree given by ptr
     * and return the new root in O(height)
     */
    Node* insertValue(Node* ptr, Node* parent, bool leftChild, int insertKey);

    /**
     * @brief Perform deletion on the subtree given by ptr
     * and return the new root in O(height)
     */
    Node* eraseValue(Node* ptr, Node* parent, bool leftChild, int deleteKey);

    /**
     * @brief Perform search on the subtree given by ptr
     * and return the result as a boolen in O(height)
     */
    bool searchValue (Node* ptr, int searchKey);

    /**
     * @brief Helper function to perform deep-copy on this AVL Tree
     */
    Node* copyNodes (Node* otherNode);

    /**
     * @brief Helper function to perform deletion on this AVL Tree
     */
    void destroyTree(Node *node);

    /**
     * @brief Helper function to copy member variables and callback functions
     */
    void copyFrom(const AVLTree &other);

public:
    AVLTree();
    AVLTree(const AVLTree &other);
    ~AVLTree();

    /**
     * @brief Assigment operator to perform deep-copy
     */
    AVLTree& operator=(const AVLTree &other);

    /**
     * @brief Assign a callback function that will be called
     * whenever AVL Tree need to create a node
     */
    void setCallbackCreateNode (auto func) {
        callbackCreateNode = func;
    }

    /**
     * @brief Assign a callback function that will be called
     * whenever AVL Tree need to create a node
     */
    void setCallbackDeleteNode (auto func) {
        callbackDeleteNode = func;
    }

    /**
     * @brief Assign a callback function that will be called
     * whenever AVL Tree need to swap the values of 2 nodes
     */
    void setCallbackSwapValue (auto func) {
        callbackSwapValue = func;
    }
    
    /**
     * @brief Assign a callback function that will be called
     * whenever AVL Tree need to create/change an edge
     */
    void setCallbackAddEdge (auto func) {
        callbackAddEdge = func;
    }

    /**
     * @brief Assign a callback function that will be called
     * whenever AVL Tree need to initiate/change the root
     */
    void setCallbackChangeRoot (auto func) {
        callbackChangeRoot = func;
    }

    /**
     * @brief Assign a callback function that will be called
     * whenever AVL Tree wants to apply animation
     */
    void setCallbackApplyAnimation (auto func) {
        callbackApplyAnimation = func;
    }

    /**
     * @brief Assign a callback function that will be called
     * whenever AVL Tree wants to highlight a node
     */
    void setCallbackHighlightNode (auto func) {
        callbackHighlightNode = func;
    }

    void setCallbackColorNode (auto func) {
        callbackColorNode = func;
    }

    /**
     * @brief Assign a callback function that will be called
     * whenever AVL Tree finishes a chain of animations
     */
    void setCallbackCompleteAnimation (auto func) {
        callbackCompleteAnimation = func;
    }

    /**
     * @brief Assign a callback function that will be called
     * whenever AVL Tree wants to load a new piece of code
     */
    void setCallbackLoadCode (auto func) {
        callbackLoadCode = func;
    }
    
    /**
     * @brief Assign a callback function that will be called
     * whenever AVL Tree wants to highlight another line of code
     */
    void setCallbackHighlightCode (auto func) {
        callbackHighlightCode = func;
    }

    /**
     * @brief Insert a value and rebalance itself accordingly
     */
    void insert (int value, bool complete = true);

    /**
     * @brief Delete a value and rebalance itself accordingly
     * and return a boolean flag to tell whether the node is actually deleted
     */
    bool erase (int value, bool complete = true);

    bool search (int value);

    bool update (int oldKey, int newKey);
};

}; // namespace DS