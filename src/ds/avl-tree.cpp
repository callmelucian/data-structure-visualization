#include "../../include/ds/avl-tree.hpp"
#include <iostream>

namespace DS {

// === Struct Node ===
Node::Node() : value(0), visualID(0), height(0), count(0), lpt(nullptr), rpt(nullptr) {}
Node::Node (int value, int visualID) : value(value), visualID(visualID), height(1), count(1), lpt(nullptr), rpt(nullptr) {}

// ========== AVL TREE PRIVATE FUNCTIONS ==========

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
            callbackHighlightNode(getVisualID(ptr->lpt));
            callbackApplyAnimation();
        }
        // resolve the Left-Left unbalanceness
        Node* newRoot = rightRotation(ptr);
        callbackHighlightNode(getVisualID(newRoot));
        return newRoot;
    }

    // rebalance tree: unbalanceness caused by right subtree
    if (balanceFactor < -1) {
        if (getBalance(ptr->rpt) > 0) { // turn the Right-Left case into the Right-Right case
            ptr->rpt = rightRotation(ptr->rpt);
            callbackAddEdge(getVisualID(ptr), getVisualID(ptr->rpt), false);
        }
        // resolve the Right-Right unbalanceness
        Node* newRoot = leftRotation(ptr);
        callbackHighlightNode(getVisualID(newRoot));
        return newRoot;
    }
    return ptr;
}

Node* AVLTree::insertValue (Node *ptr, int insertKey) {
    // insertion happen at this node
    if (ptr == nullptr) {
        callbackCreateNode(insertKey, false);
        Node* newNode = new Node(insertKey, nodeCounter++);
        return newNode;
    }
    
    callbackHighlightNode(getVisualID(ptr));
    callbackApplyAnimation();
    
    // recursive call for either subtree
    if (insertKey == ptr->value) return ptr->count++, ptr;
    if (insertKey < ptr->value) {
        // callbackAddEdge(getVisualID(ptr), -1, true);
        ptr->lpt = insertValue(ptr->lpt, insertKey);
        callbackAddEdge(getVisualID(ptr), getVisualID(ptr->lpt), true);
        callbackApplyAnimation();
    }
    if (insertKey > ptr->value) {
        ptr->rpt = insertValue(ptr->rpt, insertKey);
        callbackAddEdge(getVisualID(ptr), getVisualID(ptr->rpt), false);
        callbackApplyAnimation();
    }

    callbackHighlightNode(getVisualID(ptr));
    callbackApplyAnimation();

    return selfBalancing(ptr);
}

Node* AVLTree::eraseValue (Node *ptr, int deleteKey) {
    if (ptr == nullptr) return nullptr;

    callbackHighlightNode(getVisualID(ptr));
    callbackApplyAnimation();

    if (ptr->value == deleteKey) {
        // if either subtree is empty, simply bring the other subtree up
        if (ptr->lpt == nullptr) {
            Node* tmp = ptr->rpt;
            callbackDeleteNode(getVisualID(ptr));
            callbackHighlightNode(-1);
            delete ptr;
            return tmp;
        }
        if (ptr->rpt == nullptr) {
            Node* tmp = ptr->lpt;
            callbackDeleteNode(getVisualID(ptr));
            callbackHighlightNode(-1);
            delete ptr;
            return tmp;
        }

        // if both subtrees aren't empty, get the inorder successor,
        // bring that node up and delete it
        Node *tmp = getSmallestKey(ptr->rpt);
        callbackHighlightNode(-1);
        callbackSwapValue(getVisualID(ptr), getVisualID(tmp));
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
        std::swap(ptr->value, tmp->value);

        ptr->rpt = eraseValue(ptr->rpt, tmp->value);
        callbackAddEdge(getVisualID(ptr), getVisualID(ptr->rpt), false); // ptr->rpt could be nullptr
    }
    else if (deleteKey < ptr->value) {
        ptr->lpt = eraseValue(ptr->lpt, deleteKey);
        callbackAddEdge(getVisualID(ptr), getVisualID(ptr->lpt), true); // ptr->lpt could be nullptr
        callbackApplyAnimation();
    }
    else if (deleteKey > ptr->value) {
        ptr->rpt = eraseValue(ptr->rpt, deleteKey);
        callbackAddEdge(getVisualID(ptr), getVisualID(ptr->rpt), false); // ptr->rpt could be nullptr
        callbackApplyAnimation();
    }

    callbackHighlightNode(getVisualID(ptr)); // ptr could be nullptr
    callbackApplyAnimation();

    return ptr ? selfBalancing(ptr) : nullptr;
}

Node* AVLTree::copyNodes(Node* otherNode) {
    if (otherNode == nullptr) return nullptr;
    Node* newNode = new Node(otherNode->value, otherNode->visualID);
    newNode->height = otherNode->height;
    newNode->count = otherNode->count;
    newNode->lpt = copyNodes(otherNode->lpt);
    newNode->rpt = copyNodes(otherNode->rpt);
    return newNode;
}

void AVLTree::destroyTree(Node* node) {
    if (node == nullptr) return;
    destroyTree(node->lpt);
    destroyTree(node->rpt);
    delete node;
}

void AVLTree::copyFrom (const AVLTree &other) {
    this->nodeCounter = other.nodeCounter;
    this->root = copyNodes(other.root);

    this->callbackCreateNode = other.callbackCreateNode;
    this->callbackDeleteNode = other.callbackDeleteNode;
    this->callbackSwapValue = other.callbackSwapValue;
    this->callbackAddEdge = other.callbackAddEdge;
    this->callbackChangeRoot = other.callbackChangeRoot;
    this->callbackApplyAnimation = other.callbackApplyAnimation;
    this->callbackHighlightNode = other.callbackHighlightNode;
    this->callbackCompleteAnimation = other.callbackCompleteAnimation;
}

// ========== AVL TREE PUBLIC FUNCTIONS ==========

AVLTree::AVLTree() : root(nullptr), nodeCounter(0) {}

AVLTree::AVLTree(const AVLTree& other) {
    copyFrom(other);
}

AVLTree::~AVLTree() {
    destroyTree(root);
    root = nullptr;
}

AVLTree& AVLTree::operator=(const AVLTree& other) {
    if (this != &other) {
        destroyTree(root);
        copyFrom(other);
    }
    return *this;
}

void AVLTree::insert (int value) {
    if (root == nullptr) {
        callbackCreateNode(value, true);
        root = new Node(value, nodeCounter++);
    }
    else {
        root = insertValue(root, value);
        callbackChangeRoot(getVisualID(root));
    }
    callbackApplyAnimation();
    callbackHighlightNode(-1);
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

bool AVLTree::erase (int value) {
    if (root == nullptr) return false;
    root = eraseValue(root, value);
    callbackChangeRoot(getVisualID(root)); // root could be nullptr
    callbackApplyAnimation();
    callbackHighlightNode(-1);
    callbackCompleteAnimation();
    callbackApplyAnimation();
    return true;
}

}; // namespace DS