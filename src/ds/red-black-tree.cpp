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

RedBlackTree::Node* RedBlackTree::leftRotation (Node* ptr, Node* parent, bool isLeftChild) {
    Node* rightChild = ptr->rpt; // is surely a valid pointer
    Node* tmp = rightChild->lpt; // this one could be a nullptr

    rightChild->lpt = ptr;
    callbackAddEdge(getVisualID(rightChild), getVisualID(ptr), true);
    ptr->rpt = tmp;
    callbackAddEdge(getVisualID(ptr), getVisualID(tmp), false); // tmp could be nullptr
    rightChild->color = ptr->color;
    callbackColorNode(getVisualID(rightChild), ptr->color == BLACK ? 2 : 1);
    ptr->color = RED;
    callbackColorNode(getVisualID(ptr), 1);
    fixEdges(parent, rightChild, isLeftChild);
    return rightChild;
}

RedBlackTree::Node* RedBlackTree::rightRotation (Node* ptr, Node* parent, bool isLeftChild) {
    Node* leftChild = ptr->lpt; // is surely a valid pointer
    Node* tmp = leftChild->rpt; // this one could be a nullptr

    leftChild->rpt = ptr;
    callbackAddEdge(getVisualID(leftChild), getVisualID(ptr), false);
    ptr->lpt = tmp;
    callbackAddEdge(getVisualID(ptr), getVisualID(tmp), true); // tmp could be nullptr
    leftChild->color = ptr->color;
    callbackColorNode(getVisualID(leftChild), ptr->color == BLACK ? 2 : 1);
    ptr->color = RED;
    callbackColorNode(getVisualID(ptr), 1);
    fixEdges(parent, leftChild, isLeftChild);
    return leftChild;
}

RedBlackTree::Node* RedBlackTree::moveRedLeft (Node* ptr, Node* parent, bool leftChild) {
    flipColors(ptr);
    if (isRed(ptr->rpt->lpt)) {
        ptr->rpt = rightRotation(ptr->rpt, ptr, false);
        ptr = leftRotation(ptr, parent, leftChild);
        flipColors(ptr);
    }
    return ptr;
}

RedBlackTree::Node* RedBlackTree::moveRedRight (Node* ptr, Node* parent, bool leftChild) {
    flipColors(ptr);
    if (isRed(ptr->lpt->lpt)) {
        ptr = rightRotation(ptr, parent, leftChild);
        flipColors(ptr);
    }
    return ptr;
}

RedBlackTree::Node* RedBlackTree::selfBalancing (Node* ptr, Node* parent, bool leftChild) {
    // case 1: right child is red, left child is black
    callbackHighlightCode(0);
    callbackApplyAnimation();
    if (isRed(ptr->rpt) && !isRed(ptr->lpt)) {
        callbackHighlightCode(1);
        ptr = leftRotation(ptr, parent, leftChild);
        callbackApplyAnimation();
    }
    // case 2: left child and left-left granchild are red
    if (isRed(ptr->lpt) && isRed(ptr->lpt->lpt)) {
        callbackHighlightCode(2);
        ptr = rightRotation(ptr, parent, leftChild);
        callbackApplyAnimation();
    }
    // case 3: both children are red
    if (isRed(ptr->lpt) && isRed(ptr->rpt)) {
        callbackHighlightCode(3);
        flipColors(ptr);
        callbackApplyAnimation();
    }
    callbackHighlightCode(4);
    callbackApplyAnimation();
    return ptr;
}

RedBlackTree::Node* RedBlackTree::getSmallestKey (Node* ptr) {
    while (ptr->lpt != nullptr) ptr = ptr->lpt;
    return ptr;
}

void RedBlackTree::flipColors (Node* ptr) {
    ptr->color = RED;
    callbackColorNode(getVisualID(ptr), 1);
    ptr->lpt->color = BLACK;
    callbackColorNode(getVisualID(ptr->lpt), 2);
    ptr->rpt->color = BLACK;
    callbackColorNode(getVisualID(ptr->rpt), 2);
}

RedBlackTree::Node* RedBlackTree::insertValue (Node* ptr, Node* parent, bool leftChild, int insertKey) {
    // insertion happen at this node
    callbackHighlightCode(0);
    callbackHighlightNode(getVisualID(ptr));
    callbackApplyAnimation();

    if (ptr == nullptr) {
        callbackHighlightCode(1);
        callbackCreateNode(insertKey, false);
        callbackColorNode(nodeCounter, 1);
        callbackHighlightNode(nodeCounter);
        ptr = new Node(insertKey, nodeCounter++);
        fixEdges(parent, ptr, leftChild);
        callbackApplyAnimation();
        return ptr;
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
        ptr->lpt = insertValue(ptr->lpt, ptr, true, insertKey);
        // callbackAddEdge(getVisualID(ptr), getVisualID(ptr->lpt), true);
        // callbackApplyAnimation();

        callbackHighlightCode(4);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
    }

    if (insertKey > ptr->value) {
        callbackHighlightCode(5);
        callbackApplyAnimation();
        callbackHighlightCode(6);
        callbackApplyAnimation();
        ptr->rpt = insertValue(ptr->rpt, ptr, false, insertKey);
        // callbackAddEdge(getVisualID(ptr), getVisualID(ptr->rpt), false);
        // callbackApplyAnimation();

        callbackHighlightCode(6);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
    }

    callbackHighlightCode(8);
    callbackApplyAnimation();

    callbackLoadCode(CodeRepo::RB_TREE_SELF_BALANCE);
    Node* newRoot = selfBalancing(ptr, parent, leftChild);
    callbackLoadCode(CodeRepo::RB_TREE_INSERT);
    return newRoot;
}

void RedBlackTree::fixEdges (Node* parent, Node *ptr, bool leftChild) {
    if (!parent) callbackChangeRoot(getVisualID(ptr));
    else callbackAddEdge(getVisualID(parent), getVisualID(ptr), leftChild);
}

RedBlackTree::Node* RedBlackTree::eraseValue (Node* ptr, Node* parent, bool leftChild, int eraseKey) {
    callbackHighlightCode(0);
    callbackHighlightNode(getVisualID(ptr));
    callbackApplyAnimation();

    if (ptr == nullptr) {
        callbackHighlightCode(1);
        callbackApplyAnimation();
        return nullptr;
    }

    if (eraseKey < ptr->value) { // erase on left subtree
        callbackHighlightCode(2);
        callbackApplyAnimation();

        if (!isRed(ptr->lpt) && !isRed(ptr->lpt->lpt)) {
            callbackHighlightCode(3);
            callbackApplyAnimation();

            callbackHighlightCode(4);
            ptr = moveRedLeft(ptr, parent, leftChild);
            callbackApplyAnimation();
        }

        callbackHighlightCode(5);
        callbackApplyAnimation();
        ptr->lpt = eraseValue(ptr->lpt, ptr, true, eraseKey);
        callbackHighlightCode(5);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
    }

    else { // erase on current node or right subtree
        callbackHighlightCode(6);
        callbackApplyAnimation();

        if (isRed(ptr->lpt)) {
            callbackHighlightCode(7);
            ptr = rightRotation(ptr, parent, leftChild);
            callbackApplyAnimation();
        }

        if (eraseKey == ptr->value && ptr->rpt == nullptr) {
            callbackHighlightCode(8);
            callbackApplyAnimation();

            callbackDeleteNode(getVisualID(ptr));
            callbackHighlightCode(9);
            delete ptr;
            fixEdges(parent, nullptr, leftChild);
            callbackApplyAnimation();
            return nullptr;
        }

        if (!isRed(ptr->rpt) && !isRed(ptr->rpt->lpt)) {
            callbackHighlightCode(10);
            callbackApplyAnimation();

            callbackHighlightCode(11);
            ptr = moveRedRight(ptr, parent, leftChild);
            callbackApplyAnimation();
        }

        if (eraseKey == ptr->value) {
            callbackHighlightCode(12);
            callbackApplyAnimation();
            Node* tmp = getSmallestKey(ptr->rpt);

            callbackHighlightCode(13);
            callbackSwapValue(getVisualID(ptr), getVisualID(tmp));
            callbackColorNode(getVisualID(ptr), ptr->color == BLACK ? 2 : 1);
            callbackColorNode(getVisualID(tmp), tmp->color == BLACK ? 2 : 1);
            std::swap(ptr->value, tmp->value);
            callbackApplyAnimation();
        }
        callbackHighlightCode(14);
        callbackApplyAnimation();
        ptr->rpt = eraseValue(ptr->rpt, ptr, false, eraseKey);

        callbackHighlightCode(14);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
    }

    callbackHighlightCode(16);
    callbackApplyAnimation();

    callbackLoadCode(CodeRepo::RB_TREE_SELF_BALANCE);
    Node* newRoot = selfBalancing(ptr, parent, leftChild);
    callbackLoadCode(CodeRepo::RB_TREE_ERASE_VALUE);
    return newRoot;
}

bool RedBlackTree::searchValue (Node* ptr, int searchKey) {
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
        callbackColorNode(getVisualID(ptr), 1);
        callbackApplyAnimation();

        callbackColorNode(getVisualID(ptr), 0);
        callbackApplyAnimation();
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

void RedBlackTree::colorNodes (Node* ptr, int targetColor) {
    if (ptr == nullptr) return;
    int curColor = (targetColor == -1 ? (ptr->color == BLACK ? 2 : 1) : targetColor);
    callbackColorNode(getVisualID(ptr), curColor);
    if (ptr->lpt) colorNodes(ptr->lpt, targetColor);
    if (ptr->rpt) colorNodes(ptr->rpt, targetColor);
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
        callbackColorNode(nodeCounter, 1);
        callbackHighlightNode(nodeCounter);
        root = new Node(value, nodeCounter++);
        callbackApplyAnimation();
    }
    else {
        root = insertValue(root, nullptr, false, value);
        callbackChangeRoot(getVisualID(root));
        callbackApplyAnimation();
    }

    root->color = BLACK;
    callbackHighlightCode(7);
    callbackColorNode(getVisualID(root), 2);
    callbackApplyAnimation();

    callbackHighlightNode(-1);
    callbackLoadCode({});
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

bool RedBlackTree::erase (int value) {
    if (root == nullptr) return false;
    callbackLoadCode(CodeRepo::RB_TREE_ERASE_VALUE);
    root = eraseValue(root, nullptr, false, value);
    callbackChangeRoot(getVisualID(root));
    callbackApplyAnimation();
    if (root != nullptr) {
        root->color = BLACK;
        callbackLoadCode(CodeRepo::RB_TREE_ERASE_VALUE);
        callbackHighlightCode(15);
        callbackColorNode(getVisualID(root), 2);
        callbackApplyAnimation();
    }
    callbackHighlightNode(-1);
    callbackLoadCode({});
    callbackCompleteAnimation();
    callbackApplyAnimation();
    return true;
}

bool RedBlackTree::search (int value) {
    if (root == nullptr) return false;
    colorNodes(root, 0);
    callbackLoadCode(CodeRepo::AVL_TREE_SEARCH);
    searchValue(root, value);
    
    colorNodes(root, -1);
    callbackHighlightNode(-1);
    callbackLoadCode({});
    callbackCompleteAnimation();
    callbackApplyAnimation();
    return true;
}

}; // namespace DS