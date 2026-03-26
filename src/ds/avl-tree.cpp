#include "../../include/ds/avl-tree.hpp"
#include <iostream>

namespace DS {

// === Struct Node ===
Node::Node() : value(0), visualID(0), height(0), count(0), lpt(nullptr), rpt(nullptr) {}
Node::Node (int value, int visualID) : value(value), visualID(visualID), height(1), count(1), lpt(nullptr), rpt(nullptr) {}

// === Class AVLTree ===
AVLTree::AVLTree() : root(nullptr) {}

int AVLTree::getHeight (Node *ptr) {
    return ptr ? ptr->height : 0;
}

int AVLTree::getBalance (Node *ptr) {
    return ptr ? getHeight(ptr->lpt) - getHeight(ptr->rpt) : 0;
}

int AVLTree::getVisualID (Node *ptr) {
    return ptr ? ptr->visualID : -1;
}

void AVLTree::updateHeight(Node *ptr) {
    ptr->height = 1 + std::max(getHeight(ptr->lpt), getHeight(ptr->rpt));
}

Node* AVLTree::leftRotation(Node *ptr) {
    Node* x = ptr->rpt; // is surely a valid pointer
    Node* tmp = x->lpt; // this one could be nullptr

    x->lpt = ptr;
    callbackAddEdge(getVisualID(x), getVisualID(ptr), true);
    ptr->rpt = tmp;
    callbackAddEdge(getVisualID(ptr), getVisualID(tmp), false); // tmp could be nullptr
    updateHeight(ptr), updateHeight(x);
    return x;
}

Node* AVLTree::rightRotation(Node *ptr) {
    Node* x = ptr->lpt; // is surely a valid pointer
    Node* tmp = x->rpt; // this one could be nullptr

    x->rpt = ptr;
    callbackAddEdge(getVisualID(x), getVisualID(ptr), false);
    ptr->lpt = tmp;
    callbackAddEdge(getVisualID(ptr), getVisualID(tmp), true); // tmp could be nullptr
    updateHeight(ptr), updateHeight(x);
    return x;
}

Node* AVLTree::getSmallestKey (Node *ptr) {
    if (ptr == nullptr or ptr->lpt == nullptr) return ptr;
    return getSmallestKey(ptr->lpt);
}

Node* AVLTree::selfBalancing (Node *ptr) {
    updateHeight(ptr);
    int balanceFactor = getBalance(ptr);

    // rebalance tree: unbalanceness caused by left subtree
    if (balanceFactor > 1) {
        if (getBalance(ptr->lpt) < 0) { // turn the Left-Right case into the Left-Left case
            ptr->lpt = leftRotation(ptr->lpt);
            callbackAddEdge(getVisualID(ptr), getVisualID(ptr->lpt), true);
        }
        // resolve the Left-Left unbalanceness
        return rightRotation(ptr);
    }

    // rebalance tree: unbalanceness caused by right subtree
    if (balanceFactor < -1) {
        if (getBalance(ptr->rpt) > 0) { // turn the Right-Left case into the Right-Right case
            ptr->rpt = rightRotation(ptr->rpt);
            callbackAddEdge(getVisualID(ptr), getVisualID(ptr->rpt), false);
        }
        // resolve the Right-Right unbalanceness
        return leftRotation(ptr);
    }
    return ptr;
}

Node* AVLTree::insertValue (Node *ptr, int insertKey) {
    // insertion happen at this node
    if (ptr == nullptr)
        return new Node(insertKey, callbackCreateNode(insertKey, false));
    
    // std::cerr << "Trying to insert at " << ptr->value << " " << insertKey << std::endl;
    
    // recursive call for either subtree
    if (insertKey == ptr->value) return ptr->count++, ptr;
    if (insertKey < ptr->value) {
        ptr->lpt = insertValue(ptr->lpt, insertKey);
        callbackAddEdge(getVisualID(ptr), getVisualID(ptr->lpt), true);
    }
    if (insertKey > ptr->value) {
        ptr->rpt = insertValue(ptr->rpt, insertKey);
        callbackAddEdge(getVisualID(ptr), getVisualID(ptr->rpt), false);
    }
    return selfBalancing(ptr);
}

Node* AVLTree::eraseValue (Node *ptr, int deleteKey) {
    if (ptr == nullptr) return nullptr;
    if (ptr->value == deleteKey) {
        // if either subtree is empty, simply bring the other subtree up
        if (ptr->lpt == nullptr) {
            Node* tmp = ptr->rpt;
            callbackDeleteNode(getVisualID(ptr));
            delete ptr;
            return tmp;
        }
        if (ptr->rpt == nullptr) {
            Node* tmp = ptr->lpt;
            callbackDeleteNode(getVisualID(ptr));
            delete ptr;
            return tmp;
        }

        // if both subtrees aren't empty, get the inorder successor,
        // bring that node up and delete it
        Node *tmp = getSmallestKey(ptr->rpt);
        callbackSwapValue(getVisualID(ptr), getVisualID(tmp));
        std::swap(ptr->value, tmp->value);
        ptr->rpt = eraseValue(ptr->rpt, tmp->value);
        callbackAddEdge(getVisualID(ptr), getVisualID(ptr->rpt), false); // ptr->rpt could be nullptr
    }
    else if (deleteKey < ptr->value) {
        ptr->lpt = eraseValue(ptr->lpt, deleteKey);
        callbackAddEdge(getVisualID(ptr), getVisualID(ptr->lpt), true); // ptr->lpt could be nullptr
    }
    else if (deleteKey > ptr->value) {
        ptr->rpt = eraseValue(ptr->rpt, deleteKey);
        callbackAddEdge(getVisualID(ptr), getVisualID(ptr->rpt), false); // ptr->rpt could be nullptr
    }
    return ptr ? selfBalancing(ptr) : nullptr;
}

void AVLTree::insert (int value) {
    if (root == nullptr)
        root = new Node(value, callbackCreateNode(value, true));
    else {
        root = insertValue(root, value);
        callbackChangeRoot(getVisualID(root));
    }
    callbackReposition();
}

void AVLTree::erase (int value) {
    if (root == nullptr) return;
    root = eraseValue(root, value);
    callbackChangeRoot(getVisualID(root)); // root could be nullptr
    callbackReposition();
}

}; // namespace DS