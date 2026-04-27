#include "../../include/ds/avl-tree.hpp"

namespace DS {

// === Struct Node ===
AVLTree::Node::Node() : value(0), visualID(0), height(0), count(0), lpt(nullptr), rpt(nullptr) {}
AVLTree::Node::Node (int value, int visualID) : value(value), visualID(visualID), height(1), count(1), lpt(nullptr), rpt(nullptr) {}

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

void AVLTree::fixEdges (Node* ptr, Node* parent, bool leftChild) {
    if (!parent) callbackChangeRoot(getVisualID(ptr));
    else callbackAddEdge(getVisualID(parent), getVisualID(ptr), leftChild);
}

AVLTree::Node* AVLTree::leftRotation(Node* ptr, Node* parent, bool isLeftChild) {
    Node* rightChild = ptr->rpt; // is surely a valid pointer
    Node* tmp = rightChild->lpt; // this one could be a nullptr

    rightChild->lpt = ptr;
    callbackAddEdge(getVisualID(rightChild), getVisualID(ptr), true);
    ptr->rpt = tmp;
    callbackAddEdge(getVisualID(ptr), getVisualID(tmp), false); // tmp could be nullptr
    updateHeight(ptr), updateHeight(rightChild);
    fixEdges(rightChild, parent, isLeftChild);
    return rightChild;
}

AVLTree::Node* AVLTree::rightRotation (Node* ptr, Node* parent, bool isLeftChild) {
    Node* leftChild = ptr->lpt; // is surely a valid pointer
    Node* tmp = leftChild->rpt; // this one could be a nullptr

    leftChild->rpt = ptr;
    callbackAddEdge(getVisualID(leftChild), getVisualID(ptr), false);
    ptr->lpt = tmp;
    callbackAddEdge(getVisualID(ptr), getVisualID(tmp), true); // tmp could be nullptr
    updateHeight(ptr), updateHeight(leftChild);
    fixEdges(leftChild, parent, isLeftChild);
    return leftChild;
}

AVLTree::Node* AVLTree::getSmallestKey (Node *ptr) {
    if (ptr == nullptr or ptr->lpt == nullptr) return ptr;
    return getSmallestKey(ptr->lpt);
}

AVLTree::Node* AVLTree::selfBalancing (Node* ptr, Node* parent, bool leftChild) {
    updateHeight(ptr);
    int balanceFactor = getBalance(ptr);

    callbackHighlightCode(0);
    callbackApplyAnimation();

    // rebalance tree: unbalanceness caused by left subtree
    if (balanceFactor > 1) {
        callbackHighlightCode(1);
        callbackApplyAnimation();
        if (getBalance(ptr->lpt) < 0) { // turn the Left-Right case into the Left-Left case
            ptr->lpt = leftRotation(ptr->lpt, ptr, true);
            callbackHighlightCode(2);
            callbackHighlightNode(getVisualID(ptr->lpt));
            callbackApplyAnimation();
        }
        // resolve the Left-Left unbalanceness
        callbackHighlightCode(3);
        Node* newRoot = rightRotation(ptr, parent, leftChild);
        callbackHighlightNode(getVisualID(newRoot));
        callbackApplyAnimation();
        return newRoot;
    }

    // rebalance tree: unbalanceness caused by right subtree
    if (balanceFactor < -1) {
        callbackHighlightCode(4);
        callbackApplyAnimation();
        if (getBalance(ptr->rpt) > 0) { // turn the Right-Left case into the Right-Right case
            ptr->rpt = rightRotation(ptr->rpt, ptr, false);
            callbackHighlightCode(5);
            callbackHighlightNode(getVisualID(ptr->lpt));
            callbackApplyAnimation();
        }
        // resolve the Right-Right unbalanceness
        callbackHighlightCode(6);
        Node* newRoot = leftRotation(ptr, parent, leftChild);
        callbackHighlightNode(getVisualID(newRoot));
        callbackApplyAnimation();
        return newRoot;
    }

    callbackHighlightCode(7);
    callbackApplyAnimation();
    return ptr;
}

AVLTree::Node* AVLTree::insertValue (Node* ptr, Node* parent, bool leftChild, int insertKey) {
    // insertion happen at this node
    callbackHighlightCode(0);
    callbackHighlightNode(getVisualID(ptr));
    callbackApplyAnimation();

    // empty node, insertion
    if (ptr == nullptr) {
        callbackHighlightCode(1);
        callbackCreateNode(insertKey, false);
        callbackHighlightNode(nodeCounter);
        Node* newNode = new Node(insertKey, nodeCounter++);
        fixEdges(newNode, parent, leftChild);
        callbackApplyAnimation();
        return newNode;
    }

    // node already inserted, no insertion happen
    if (insertKey == ptr->value) {
        callbackHighlightCode(2);
        callbackApplyAnimation();
        return ptr->count++, ptr;
    }
    
    // go to left subtree
    if (insertKey < ptr->value) {
        callbackHighlightCode(3);
        callbackApplyAnimation();
        callbackHighlightCode(4);
        callbackApplyAnimation();
        ptr->lpt = insertValue(ptr->lpt, ptr, true, insertKey);

        callbackHighlightCode(4);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
    }

    // go to right subtree
    if (insertKey > ptr->value) {
        callbackHighlightCode(5);
        callbackApplyAnimation();
        callbackHighlightCode(6);
        callbackApplyAnimation();
        ptr->rpt = insertValue(ptr->rpt, ptr, false, insertKey);
        
        callbackHighlightCode(6);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
    }

    callbackHighlightCode(7);
    callbackApplyAnimation();

    callbackLoadCode(CodeRepo::AVL_TREE_SELF_BALANCE);
    Node* newRoot = selfBalancing(ptr, parent, leftChild);
    callbackLoadCode(CodeRepo::AVL_TREE_INSERT);
    return newRoot;
}

AVLTree::Node* AVLTree::eraseValue (Node* ptr, Node* parent, bool leftChild, int deleteKey) {
    callbackHighlightCode(0);
    callbackHighlightNode(getVisualID(ptr));
    callbackApplyAnimation();

    if (ptr == nullptr) {
        callbackHighlightCode(1);
        fixEdges(nullptr, parent, leftChild);
        callbackApplyAnimation();
        return nullptr;
    }
    
    if (ptr->value == deleteKey) {
        callbackHighlightCode(2);    
        callbackApplyAnimation();
        // if either subtree is empty, simply bring the other subtree up
        if (ptr->lpt == nullptr || ptr->rpt == nullptr) {
            callbackHighlightCode(3);
            callbackApplyAnimation();
            Node* tmp = (ptr->lpt == nullptr ? ptr->rpt : ptr->lpt);
            callbackHighlightNode(-1);
            callbackDeleteNode(getVisualID(ptr));
            callbackHighlightCode(4);
            delete ptr;
            fixEdges(tmp, parent, leftChild);
            callbackApplyAnimation();
            eraseSuccess = true;
            return tmp;
        }
        callbackHighlightCode(5);
        callbackApplyAnimation();

        // if both subtrees aren't empty, get the inorder successor,
        // bring that node up and delete it
        Node* tmp = getSmallestKey(ptr->rpt);
        callbackHighlightNode(-1);
        callbackHighlightCode(6);
        callbackSwapValue(getVisualID(ptr), getVisualID(tmp));
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
        std::swap(ptr->value, tmp->value);

        callbackHighlightCode(7);
        callbackApplyAnimation();
        ptr->rpt = eraseValue(ptr->rpt, ptr, false, tmp->value);

        callbackHighlightCode(7);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
    }
    else if (deleteKey < ptr->value) {
        callbackHighlightCode(8);
        callbackApplyAnimation();
        callbackHighlightCode(9);
        callbackApplyAnimation();
        ptr->lpt = eraseValue(ptr->lpt, ptr, true, deleteKey);
        callbackHighlightCode(9);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
    }
    else if (deleteKey > ptr->value) {
        callbackHighlightCode(10);
        callbackApplyAnimation();
        callbackHighlightCode(11);
        callbackApplyAnimation();
        ptr->rpt = eraseValue(ptr->rpt, ptr, false, deleteKey);
        callbackHighlightCode(11);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
    }

    callbackHighlightCode(12);
    callbackApplyAnimation();

    callbackLoadCode(CodeRepo::AVL_TREE_SELF_BALANCE);
    Node* newRoot = ptr ? selfBalancing(ptr, parent, leftChild) : nullptr;
    callbackLoadCode(CodeRepo::AVL_TREE_ERASE);
    return newRoot;
}

bool AVLTree::searchValue (Node* ptr, int searchKey) {
    callbackHighlightCode(0);
    callbackHighlightNode(getVisualID(ptr));
    callbackApplyAnimation();

    if (ptr == nullptr) {
        callbackHighlightCode(1);
        callbackApplyAnimation();
        return false;
    }

    if (searchKey == ptr->value) {
        callbackHighlightCode(2);
        callbackColorNode(getVisualID(ptr), true);
        callbackApplyAnimation();

        callbackColorNode(getVisualID(ptr), false);
        return true;
    }

    if (searchKey < ptr->value) {
        callbackHighlightCode(3);
        callbackApplyAnimation();
        
        callbackHighlightCode(4);
        callbackApplyAnimation();
        return searchValue(ptr->lpt, searchKey);
    }

    if (searchKey > ptr->value) {
        callbackHighlightCode(5);
        callbackApplyAnimation();
        
        callbackHighlightCode(6);
        callbackApplyAnimation();
        return searchValue(ptr->rpt, searchKey);
    }

    return false;
}

AVLTree::Node* AVLTree::copyNodes(Node* otherNode) {
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
    this->callbackColorNode = other.callbackColorNode;
    this->callbackCompleteAnimation = other.callbackCompleteAnimation;
    this->callbackLoadCode = other.callbackLoadCode;
    this->callbackHighlightCode = other.callbackHighlightCode;
}

// ========== AVL TREE PUBLIC FUNCTIONS ==========

AVLTree::AVLTree() : root(nullptr), nodeCounter(0), eraseSuccess(false) {}

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

void AVLTree::insert (int value, bool complete) {
    callbackLoadCode(CodeRepo::AVL_TREE_INSERT);
    root = insertValue(root, nullptr, false, value);
    if (complete) {
        callbackHighlightNode(-1);
        callbackLoadCode({});
        callbackCompleteAnimation();
        callbackApplyAnimation();
    }
}

bool AVLTree::erase (int value, bool complete) {
    eraseSuccess = false;
    if (root == nullptr) return false;
    callbackLoadCode(CodeRepo::AVL_TREE_ERASE);
    root = eraseValue(root, nullptr, false, value);
    if (complete) {
        callbackHighlightNode(-1);
        callbackLoadCode({});
        callbackCompleteAnimation();
        callbackApplyAnimation();
    }
    return true;
}

bool AVLTree::search (int value) {
    if (root == nullptr) return false;
    callbackLoadCode(CodeRepo::AVL_TREE_SEARCH);
    searchValue(root, value);
    
    callbackHighlightNode(-1);
    callbackLoadCode({});
    callbackCompleteAnimation();
    callbackApplyAnimation();
    return true;
}

bool AVLTree::update (int oldKey, int newKey) {
    if (root == nullptr) return false;
    erase(oldKey, false);
    if (eraseSuccess) insert(newKey, true);
    else {
        callbackHighlightNode(-1);
        callbackLoadCode({});
        callbackCompleteAnimation();
        callbackApplyAnimation();
    }
    return true;
}

}; // namespace DS