#pragma once
#include <functional>
#include <algorithm>

namespace DS {
struct Node {
    int value, visualID, height, count;
    Node *lpt, *rpt;

    Node();
    Node(int value, int visualID);
};

class AVLTree {
private:
    Node *root;
    int nodeCounter;

    std::function<void(int, bool)> callbackCreateNode;
    std::function<void(int)> callbackDeleteNode;
    std::function<void(int, int)> callbackSwapValue;
    std::function<void(int, int, bool)> callbackAddEdge;
    std::function<void(int)> callbackChangeRoot;
    std::function<void()> callbackReposition;
    std::function<void()> callbackApplyAnimation;
    std::function<void(int)> callbackHighlightNode;

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
    Node* leftRotation(Node *ptr);

    /**
     * @brief Perform right rotation on the subtree given by ptr
     * and return the new root in O(1)
     */
    Node* rightRotation(Node *ptr);

    /**
     * @brief Perform self balancing in O(1)
     */
    Node* selfBalancing(Node *ptr);

    /**
     * @brief Return the pointer pointing the smallest key
     * within the subtree given by ptr in O(height)
     */
    Node* getSmallestKey(Node *ptr);

    /**
     * @brief Perform insertion on the subtree given by ptr
     * and return the new root in O(height)
     */
    Node* insertValue(Node *ptr, int insertKey);

    /**
     * @brief Perform deletion on the subtree given by ptr
     * and return the new root in O(height)
     */
    Node* eraseValue(Node *ptr, int deleteKey);

public:
    AVLTree();

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
     * whenever AVL Tree need to reposition itself
     */
    void setCallbackReposition (auto func) {
        callbackReposition = func;
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

    /**
     * @brief Insert a value and rebalance itself accordingly
     */
    void insert(int value);

    /**
     * @brief Deleta a value and rebalance itself accordingly
     */
    void erase(int value);
};
};