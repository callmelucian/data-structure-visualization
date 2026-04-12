#include "../../include/ds/red-black-tree.hpp"

namespace DS {

// ========== STRUCT NODE ==========
RedBlackTree::Node::Node() :
    value(0), visualID(0), count(0), color(RED), lpt(nullptr), rpt(nullptr) {}
RedBlackTree::Node::Node (int value, int visualID) :
    value(value), visualID(visualID), count(0), color(RED), lpt(nullptr), rpt(nullptr) {}

// ========= RED-BLACK TREE PRIVATE FUNCTIONS ==========
bool RedBlackTree::isRed (Node* ptr) {
    return ptr && ptr->color == RED;
}

RedBlackTree::Node* RedBlackTree::leftRotation (Node* ptr) {
    Node* rightChild = ptr->rpt; // is surely a valid pointer
    Node* tmp = rightChild->lpt; // this one could be a nullptr
    rightChild->lpt = ptr;
    ptr->rpt = tmp;
    rightChild->color = ptr->color;
    ptr->color = RED;
    return rightChild;
}

RedBlackTree::Node* RedBlackTree::rightRotation (Node* ptr) {
    Node* leftChild = ptr->lpt; // is surely a valid pointer
    Node* tmp = leftChild->rpt; // this one could be a nullptr

    leftChild->rpt = ptr;
    ptr->lpt = tmp;
    leftChild->color = ptr->color;
    ptr->color = RED;
    return leftChild;
}

RedBlackTree::Node* RedBlackTree::moveRedLeft (Node* ptr) {
    flipColors(ptr);
    if (isRed(ptr->rpt->lpt)) {
        ptr->rpt = rightRotation(ptr->rpt);
        ptr = leftRotation(ptr);
        flipColors(ptr);
    }
    return ptr;
}

RedBlackTree::Node* RedBlackTree::moveRedRight (Node* ptr) {
    flipColors(ptr);
    if (isRed(ptr->lpt->lpt)) {
        ptr = rightRotation(ptr);
        flipColors(ptr);
    }
    return ptr;
}

RedBlackTree::Node* RedBlackTree::selfBalancing (Node* ptr) {
    // case 1: right child is red, left child is black
    if (isRed(ptr->rpt) && !isRed(ptr->lpt)) ptr = leftRotation(ptr);
    // case 2: left child and left-left granchild are red
    if (isRed(ptr->lpt) && isRed(ptr->lpt->lpt)) ptr = rightRotation(ptr);
    // case 3: both children are red
    if (isRed(ptr->lpt) && isRed(ptr->rpt)) flipColors(ptr);
    return ptr;
}

RedBlackTree::Node* RedBlackTree::deleteMin (Node* ptr) {
    // this subtree consists of a single node
    if (ptr->lpt == nullptr) {
        delete ptr;
        return nullptr; // remember the tree is left-leaning
    }

    if (!isRed(ptr->lpt) && !isRed(ptr->lpt->lpt)) ptr = moveRedLeft(ptr);
    ptr->lpt = deleteMin(ptr->lpt);
    return selfBalancing(ptr);
}

RedBlackTree::Node* RedBlackTree::getSmallestKey (Node* ptr) {
    while (ptr->lpt != nullptr) ptr = ptr->lpt;
    return ptr;
}

void RedBlackTree::flipColors (Node* ptr) {
    ptr->color = RED;
    ptr->lpt->color = BLACK;
    ptr->rpt->color = BLACK;
}

RedBlackTree::Node* RedBlackTree::insertValue (Node* ptr, int insertKey) {
    if (ptr == nullptr)
        return new Node(insertKey, nodeCounter++);
    
    if (insertKey == ptr->value) return ptr->count++, ptr;
    if (insertKey < ptr->value) ptr->lpt = insertValue(ptr->lpt, insertKey);
    if (insertKey > ptr->value) ptr->rpt = insertValue(ptr->rpt, insertKey);

    return selfBalancing(ptr);
}

RedBlackTree::Node* RedBlackTree::eraseValue (Node* ptr, int eraseKey) {
    if (eraseKey < ptr->value) { // erase on left subtree
        if (!isRed(ptr->lpt) && !isRed(ptr->lpt->lpt)) ptr = moveRedLeft(ptr);
        ptr->lpt = eraseValue(ptr->lpt, eraseKey);
    }
    if (eraseKey > ptr->value) { // erase on right subtree
        if (isRed(ptr->lpt)) ptr = rightRotation(ptr);
        if (!isRed(ptr->rpt) && !isRed(ptr->rpt->lpt)) ptr = moveRedRight(ptr);
        ptr->rpt = eraseValue(ptr->rpt, eraseKey);
    }
    if (eraseKey == ptr->value) { // erase this node
        if (isRed(ptr->lpt)) ptr = rightRotation(ptr); // this node is temporary right-leaning
        if (ptr->rpt == nullptr) {
            delete ptr;
            return nullptr;
        }
        if (!isRed(ptr->rpt) && !isRed(ptr->rpt->lpt))
            ptr = moveRedRight(ptr);
        
        Node* tmp = getSmallestKey(ptr->rpt);
        std::swap(ptr->value, tmp->value);
        ptr->rpt = deleteMin(ptr->rpt);
    }
    return selfBalancing(ptr);
}

RedBlackTree::Node* RedBlackTree::copyNodes (Node* otherNode) {
    if (otherNode == nullptr) return nullptr;
    Node* newNode = new Node(*otherNode);
    newNode->lpt = copyNodes(newNode->lpt);
    newNode->rpt = copyNodes(newNode->rpt); 
    return newNode;
}

void RedBlackTree::destroyTree (Node* ptr) {
    if (ptr == nullptr) return;
    destroyTree(ptr->lpt);
    destroyTree(ptr->rpt);
    delete ptr;
}

void RedBlackTree::copyFrom (const RedBlackTree &other) {
    this->nodeCounter = other.nodeCounter;
    this->root = copyNodes(other.root);

    this->callbackCreateNode = other.callbackCreateNode;
    this->callbackDeleteNode = other.callbackDeleteNode;
    this->callbackSwapValue = other.callbackSwapValue;
    this->callbackColorNode = other.callbackColorNode;
    this->callbackAddEdge = other.callbackAddEdge;
    this->callbackChangeRoot = other.callbackChangeRoot;
    this->callbackApplyAnimation = other.callbackApplyAnimation;
    this->callbackHighlightNode = other.callbackHighlightNode;
    this->callbackCompleteAnimation = other.callbackCompleteAnimation;
    this->callbackLoadCode = other.callbackLoadCode;
    this->callbackHighlightCode = other.callbackHighlightCode;
}

// ========== RED-BLACK TREE PUBLIC FUNCTIONS ==========
RedBlackTree::RedBlackTree() : root(nullptr), nodeCounter(0) {}

RedBlackTree::RedBlackTree(const RedBlackTree &other) {
    copyFrom(other);
}

RedBlackTree::~RedBlackTree() {
    destroyTree(root);
    root = nullptr;
}

RedBlackTree& RedBlackTree::operator=(const RedBlackTree &other) {
    if (this != &other) {
        destroyTree(this->root);
        copyFrom(other);
    }
    return *this;
}

void RedBlackTree::insert (int value) {
    root = insertValue(root, value);
    root->color = BLACK;
}

bool RedBlackTree::erase (int value) {
    if (root == nullptr) return false;
    root = eraseValue(root, value);
    if (root != nullptr) root->color = BLACK;
    return true;
}

}; // namespace DS