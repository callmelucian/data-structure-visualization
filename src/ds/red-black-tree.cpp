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

int RedBlackTree::getVisualID (Node* ptr) {
    return ptr ? ptr->visualID : -1;
}

RedBlackTree::Node* RedBlackTree::leftRotation (Node* ptr) {
    Node* rightChild = ptr->rpt; // is surely a valid pointer
    Node* tmp = rightChild->lpt; // this one could be a nullptr

    rightChild->lpt = ptr;
    callbackAddEdge(getVisualID(rightChild), getVisualID(ptr), true);
    ptr->rpt = tmp;
    callbackAddEdge(getVisualID(ptr), getVisualID(tmp), false); // tmp could be nullptr
    rightChild->color = ptr->color;
    callbackColorNode(getVisualID(rightChild), ptr->color == BLACK);
    ptr->color = RED;
    callbackColorNode(getVisualID(ptr), false);
    return rightChild;
}

RedBlackTree::Node* RedBlackTree::rightRotation (Node* ptr) {
    Node* leftChild = ptr->lpt; // is surely a valid pointer
    Node* tmp = leftChild->rpt; // this one could be a nullptr

    leftChild->rpt = ptr;
    callbackAddEdge(getVisualID(leftChild), getVisualID(ptr), false);
    ptr->lpt = tmp;
    callbackAddEdge(getVisualID(ptr), getVisualID(tmp), true); // tmp could be nullptr
    leftChild->color = ptr->color;
    callbackColorNode(getVisualID(leftChild), ptr->color == BLACK);
    ptr->color = RED;
    callbackColorNode(getVisualID(ptr), false);
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
    callbackHighlightCode(0);
    callbackApplyAnimation();

    // case 1: right child is red, left child is black
    if (isRed(ptr->rpt) && !isRed(ptr->lpt)) {
        callbackHighlightCode(1);
        callbackApplyAnimation();
        ptr = leftRotation(ptr);
    }
    // case 2: left child and left-left granchild are red
    if (isRed(ptr->lpt) && isRed(ptr->lpt->lpt)) {
        callbackHighlightCode(2);
        callbackApplyAnimation();
        ptr = rightRotation(ptr);
    }
    // case 3: both children are red
    if (isRed(ptr->lpt) && isRed(ptr->rpt)) {
        callbackHighlightCode(3);
        flipColors(ptr);
        callbackApplyAnimation();
    }
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
    callbackColorNode(getVisualID(ptr), false);
    ptr->lpt->color = BLACK;
    callbackColorNode(getVisualID(ptr->lpt), true);
    ptr->rpt->color = BLACK;
    callbackColorNode(getVisualID(ptr->rpt), true);
    callbackApplyAnimation();
}

RedBlackTree::Node* RedBlackTree::insertValue (Node* ptr, int insertKey) {
    // insertion happen at this node
    callbackHighlightCode(0);
    callbackHighlightNode(getVisualID(ptr));
    callbackApplyAnimation();

    if (ptr == nullptr) {
        callbackHighlightCode(1);
        callbackCreateNode(insertKey, false);
        callbackColorNode(nodeCounter, false);
        callbackHighlightNode(nodeCounter);
        return new Node(insertKey, nodeCounter++);
    }
    
    if (insertKey == ptr->value) {
        callbackHighlightCode(2);
        callbackApplyAnimation();
        return ptr->count++, ptr;
    }

    if (insertKey < ptr->value) {
        callbackHighlightCode(3);
        callbackApplyAnimation();
        callbackHighlightCode(4);
        callbackApplyAnimation();
        ptr->lpt = insertValue(ptr->lpt, insertKey);
        callbackAddEdge(getVisualID(ptr), getVisualID(ptr->lpt), true);
        callbackApplyAnimation();

        callbackHighlightCode(4);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
    }

    if (insertKey > ptr->value) {
        callbackHighlightCode(5);
        callbackApplyAnimation();
        callbackHighlightCode(6);
        callbackApplyAnimation();
        ptr->rpt = insertValue(ptr->rpt, insertKey);
        callbackAddEdge(getVisualID(ptr), getVisualID(ptr->rpt), false);
        callbackApplyAnimation();

        callbackHighlightCode(6);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
    }

    callbackHighlightCode(8);
    callbackApplyAnimation();

    callbackLoadCode(CodeRepo::RB_TREE_SELF_BALANCE);
    Node* newRoot = selfBalancing(ptr);
    callbackLoadCode(CodeRepo::RB_TREE_INSERT);
    return newRoot;
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
    newNode->lpt = copyNodes(otherNode->lpt);
    newNode->rpt = copyNodes(otherNode->rpt); 
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
    callbackLoadCode(CodeRepo::RB_TREE_INSERT);
    if (root == nullptr) {
        callbackHighlightCode(0);
        callbackApplyAnimation();

        callbackHighlightCode(1);
        callbackCreateNode(value, true);
        callbackHighlightNode(nodeCounter);
        root = new Node(value, nodeCounter++);
        callbackApplyAnimation();
    }
    else {
        root = insertValue(root, value);
        callbackChangeRoot(getVisualID(root));
        callbackApplyAnimation();
    }

    root->color = BLACK;
    callbackHighlightCode(7);
    callbackColorNode(getVisualID(root), true);
    callbackApplyAnimation();

    callbackHighlightNode(-1);
    callbackLoadCode({});
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

bool RedBlackTree::erase (int value) {
    if (root == nullptr) return false;
    root = eraseValue(root, value);
    if (root != nullptr) root->color = BLACK;
    return true;
}

}; // namespace DS