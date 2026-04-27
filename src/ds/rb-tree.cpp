#include "../../include/ds/rb-tree.hpp"

namespace DS {

// ========== STRUCT NODE ==========
RedBlackTree::Node::Node() :
    value(0), visualID(-1), count(0), color(RED), parent(nullptr), lpt(nullptr), rpt(nullptr) {}
RedBlackTree::Node::Node (int value, int visualID) :
    value(value), visualID(visualID), count(0), color(RED), parent(nullptr), lpt(nullptr), rpt(nullptr) {}

// ========= RED-BLACK TREE PRIVATE FUNCTIONS ==========
int RedBlackTree::getVisualID (Node* ptr) {
    return ptr ? ptr->visualID : -1;
}

void RedBlackTree::initNullNode (Node* ptr, Node* parent) {
    ptr->value = 0, ptr->color = BLACK;
    ptr->lpt = ptr->rpt = nullptr;
    ptr->parent = parent;
}

void RedBlackTree::leftRotation (Node* ptr) {
    Node* rightChild = ptr->rpt, *tmp = rightChild->lpt;

    // change link
    ptr->rpt = tmp;
    callbackAddEdge(getVisualID(ptr), getVisualID(tmp), false);

    // fix parents
    if (rightChild->lpt != TNULL) rightChild->lpt->parent = ptr;
    rightChild->parent = ptr->parent;

    // link new root with parent
    if (ptr->parent == nullptr) {
        root = rightChild;
        callbackChangeRoot(getVisualID(root));
    }
    else if (ptr == ptr->parent->lpt) {
        ptr->parent->lpt = rightChild;
        callbackAddEdge(getVisualID(ptr->parent), getVisualID(rightChild), true);
    }
    else {
        ptr->parent->rpt = rightChild;
        callbackAddEdge(getVisualID(ptr->parent), getVisualID(rightChild), false);
    }

    // change link
    rightChild->lpt = ptr, ptr->parent = rightChild;
    callbackAddEdge(getVisualID(rightChild), getVisualID(ptr), true);
}

void RedBlackTree::rightRotation (Node* ptr) {
    Node *leftChild = ptr->lpt, *tmp = leftChild->rpt;

    // change link
    ptr->lpt = tmp;
    callbackAddEdge(getVisualID(ptr), getVisualID(tmp), true);

    // fix parents
    if (leftChild->rpt != TNULL) leftChild->rpt->parent = ptr;
    leftChild->parent = ptr->parent;

    // link new root with parent
    if (ptr->parent == nullptr) {
        root = leftChild;
        callbackChangeRoot(getVisualID(root));
    }
    else if (ptr == ptr->parent->lpt) {
        ptr->parent->lpt = leftChild;
        callbackAddEdge(getVisualID(ptr->parent), getVisualID(leftChild), true);
    }
    else {
        ptr->parent->rpt = leftChild;
        callbackAddEdge(getVisualID(ptr->parent), getVisualID(leftChild), false);
    }

    // change link
    leftChild->rpt = ptr, ptr->parent = leftChild;
    callbackAddEdge(getVisualID(leftChild), getVisualID(ptr), false);
}

void RedBlackTree::recolor (Node* ptr) {
    ptr->parent->color = BLACK;
    callbackColorNode(getVisualID(ptr->parent), 2);
    ptr->parent->parent->color = RED;
    callbackColorNode(getVisualID(ptr->parent->parent), 1);
}

void RedBlackTree::insertFixup (Node* ptr) {
    Node* u;
    callbackHighlightNode(getVisualID(ptr));
    while (ptr->parent->color == RED) {
        callbackHighlightCode(7);
        callbackApplyAnimation();

        if (ptr->parent == ptr->parent->parent->rpt) {
            u = ptr->parent->parent->lpt;
            if (u->color == RED) { // case 1
                callbackHighlightCode(8);
                callbackApplyAnimation();

                callbackHighlightCode(9);
                u->color = BLACK;
                callbackColorNode(getVisualID(u), 2);
                recolor(ptr);
                callbackApplyAnimation();

                callbackHighlightCode(10);
                ptr = ptr->parent->parent;
                callbackHighlightNode(getVisualID(ptr));
                callbackApplyAnimation();
            }
            else {
                if (ptr == ptr->parent->lpt) { // case 2
                    callbackHighlightCode(11);
                    callbackApplyAnimation();

                    ptr = ptr->parent;
                    rightRotation(ptr);
                    callbackHighlightCode(12);
                    callbackHighlightNode(getVisualID(ptr));
                    callbackApplyAnimation();
                }
                // case 3
                callbackHighlightCode(13);
                callbackApplyAnimation();

                callbackHighlightCode(14);
                recolor(ptr);
                callbackApplyAnimation();
                
                callbackHighlightCode(15);
                leftRotation(ptr->parent->parent);
                callbackApplyAnimation();
            }
        }
        else {
            u = ptr->parent->parent->rpt;
            if (u->color == RED) { // case 1
                callbackHighlightCode(8);
                callbackApplyAnimation();

                callbackHighlightCode(9);
                u->color = BLACK;
                callbackColorNode(getVisualID(u), 2);
                recolor(ptr);
                callbackApplyAnimation();

                callbackHighlightCode(10);
                ptr = ptr->parent->parent;
                callbackHighlightNode(getVisualID(ptr));
                callbackApplyAnimation();
            }
            else {
                if (ptr == ptr->parent->rpt) { // case 2
                    callbackHighlightCode(11);
                    callbackApplyAnimation();

                    ptr = ptr->parent;
                    leftRotation(ptr);
                    callbackHighlightCode(12);
                    callbackHighlightNode(getVisualID(ptr));
                    callbackApplyAnimation();
                }
                // case  3
                callbackHighlightCode(13);
                callbackApplyAnimation();

                callbackHighlightCode(14);
                recolor(ptr);
                callbackApplyAnimation();
                
                callbackHighlightCode(15);
                rightRotation(ptr->parent->parent);
                callbackApplyAnimation();
            }
        }
        if (ptr == root) break;
    }
    callbackHighlightCode(16);
    callbackHighlightNode(getVisualID(root));
    root->color = BLACK;
    callbackColorNode(getVisualID(root), 2);
    callbackApplyAnimation();
}

void RedBlackTree::eraseFixup (Node* ptr) {
    callbackLoadCode(CodeRepo::RB_TREE_ERASE_FIXUP);
    callbackHighlightCode(0);
    callbackApplyAnimation();

    Node* s;
    while (ptr != root && ptr->color == BLACK) {
        callbackHighlightCode(1);
        callbackApplyAnimation();

        if (ptr == ptr->parent->lpt) { // ptr is left subtree
            s = ptr->parent->rpt;
            if (s->color == RED) { // case 1
                callbackHighlightCode(3);
                callbackApplyAnimation();

                s->color = BLACK;
                callbackColorNode(getVisualID(s), 2);
                ptr->parent->color = RED;
                callbackColorNode(getVisualID(ptr->parent), 1);
                callbackHighlightCode(4);
                callbackApplyAnimation();

                leftRotation(ptr->parent);
                s = ptr->parent->rpt;
                callbackHighlightCode(5);
                callbackApplyAnimation();
            }
            if (s->lpt->color == BLACK && s->rpt->color == BLACK) { // case 2
                callbackHighlightCode(6);
                callbackApplyAnimation();

                s->color = RED;
                callbackColorNode(getVisualID(s), 1);
                ptr = ptr->parent;
                callbackHighlightNode(getVisualID(ptr));
                callbackHighlightCode(7);
                callbackApplyAnimation();
            }
            else {
                if (s->rpt->color == BLACK) { // case 3
                    callbackHighlightCode(8);
                    callbackApplyAnimation();

                    s->lpt->color = BLACK;
                    callbackColorNode(getVisualID(s->lpt), 2);
                    s->color = RED;
                    callbackColorNode(getVisualID(s), 1);
                    callbackHighlightCode(9);
                    callbackApplyAnimation();

                    rightRotation(s);
                    s = ptr->parent->rpt;
                    callbackHighlightCode(10);
                    callbackApplyAnimation();
                }
                // case 4
                callbackHighlightCode(11);
                callbackApplyAnimation();
                
                s->color = ptr->parent->color;
                callbackColorNode(getVisualID(s), s->color == BLACK ? 2 : 1);
                ptr->parent->color = BLACK;
                callbackColorNode(getVisualID(ptr->parent), 2);
                s->rpt->color = BLACK;
                callbackColorNode(getVisualID(s->rpt), 2);
                callbackHighlightCode(12);
                callbackApplyAnimation();

                leftRotation(ptr->parent);
                callbackHighlightCode(13);
                callbackApplyAnimation();

                ptr = root;
                callbackHighlightCode(14);
                callbackHighlightNode(getVisualID(ptr));
                callbackApplyAnimation();
            }
        } // ptr is left subtree
        else { // ptr is right subtree
            s = ptr->parent->lpt;
            if (s->color == RED) { // case 1
                callbackHighlightCode(3);
                callbackApplyAnimation();

                s->color = BLACK;
                callbackColorNode(getVisualID(s), 2);
                ptr->parent->color = RED;
                callbackColorNode(getVisualID(ptr->parent), 1);
                callbackHighlightCode(4);
                callbackApplyAnimation();

                rightRotation(ptr->parent);
                s = ptr->parent->lpt;
                callbackHighlightCode(5);
                callbackApplyAnimation();
            }
            if (s->rpt->color == BLACK && s->lpt->color == BLACK) { // case 2
                callbackHighlightCode(6);
                callbackApplyAnimation();

                s->color = RED;
                callbackColorNode(getVisualID(s), 1);
                ptr = ptr->parent;
                callbackHighlightNode(getVisualID(ptr));
                callbackHighlightCode(7);
                callbackApplyAnimation();
            }
            else {
                if (s->lpt->color == BLACK) { // case 3
                    callbackHighlightCode(8);
                    callbackApplyAnimation();

                    s->rpt->color = BLACK;
                    callbackColorNode(getVisualID(s->rpt), 2);
                    s->color = RED;
                    callbackColorNode(getVisualID(s), 1);
                    callbackHighlightCode(9);
                    callbackApplyAnimation();

                    leftRotation(s);
                    s = ptr->parent->lpt;
                    callbackHighlightCode(10);
                    callbackApplyAnimation();
                }
                // case 4
                callbackHighlightCode(11);
                callbackApplyAnimation();

                s->color = ptr->parent->color;
                callbackColorNode(getVisualID(s), s->color == BLACK ? 2 : 1);
                ptr->parent->color = BLACK;
                callbackColorNode(getVisualID(ptr->parent), 2);
                s->lpt->color = BLACK;
                callbackColorNode(getVisualID(s->lpt), 2);
                callbackHighlightCode(12);
                callbackApplyAnimation();

                rightRotation(ptr->parent);
                callbackHighlightCode(13);
                callbackApplyAnimation();

                ptr = root;
                callbackHighlightCode(14);
                callbackHighlightNode(getVisualID(ptr));
                callbackApplyAnimation();
            }
        } // case 2
    } // while loop
    ptr->color = BLACK;
    callbackHighlightCode(15);
    callbackColorNode(getVisualID(ptr), 2);
    callbackApplyAnimation();
}

void RedBlackTree::rbTransplant (Node* u, Node* v) {
    if (u->parent == nullptr) {
        root = v;
        callbackChangeRoot(getVisualID(root));
    }
    else if (u == u->parent->lpt) {
        u->parent->lpt = v;
        callbackAddEdge(getVisualID(u->parent), getVisualID(v), true);
    }
    else {
        u->parent->rpt = v;
        callbackAddEdge(getVisualID(u->parent), getVisualID(v), false);
    }
    v->parent = u->parent;
}

RedBlackTree::Node* RedBlackTree::getSmallestKey (Node* ptr) {
    while (ptr->lpt != TNULL) ptr = ptr->lpt;
    return ptr;
}

bool RedBlackTree::searchValue (Node* ptr, int searchKey) {
    callbackHighlightCode(0);
    callbackHighlightNode(getVisualID(ptr));
    callbackApplyAnimation();

    if (ptr == TNULL) {
        callbackHighlightCode(1);
        callbackApplyAnimation();
        return false;
    }

    if (searchKey == ptr->value) {
        callbackHighlightCode(2);
        callbackColorNode(getVisualID(ptr), 1);
        callbackApplyAnimation();

        callbackColorNode(getVisualID(ptr), 0);
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

RedBlackTree::Node* RedBlackTree::copyNodes (Node* otherNode, Node* otherTNULL, Node* parentPtr) {
    if (otherNode == otherTNULL) return TNULL;
    Node* newNode = new Node(*otherNode);
    newNode->parent = parentPtr;
    newNode->lpt = copyNodes(otherNode->lpt, otherTNULL, newNode);
    newNode->rpt = copyNodes(otherNode->rpt, otherTNULL, newNode);
    return newNode;
}

void RedBlackTree::destroyTree(Node* ptr) {
    if (ptr == TNULL) return;
    destroyTree(ptr->lpt);
    destroyTree(ptr->rpt);
    delete ptr;
}

void RedBlackTree::copyFrom(const RedBlackTree &other) {
    this->nodeCounter = other.nodeCounter;
    this->eraseSuccess = other.eraseSuccess;
    this->root = copyNodes(other.root, other.TNULL, nullptr);

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
RedBlackTree::RedBlackTree() : root(nullptr), TNULL(new Node()), nodeCounter(0), eraseSuccess(false) {
    initNullNode(TNULL, nullptr);
    root = TNULL;
}

RedBlackTree::RedBlackTree(const RedBlackTree &other) : TNULL(new Node()) {
    initNullNode(TNULL, nullptr);
    copyFrom(other);
}

RedBlackTree::~RedBlackTree() {
    destroyTree(root);
    root = nullptr;
    delete TNULL;
}

RedBlackTree& RedBlackTree::operator=(const RedBlackTree &other) {
    if (this != &other) {
        destroyTree(this->root);
        copyFrom(other);
    }
    return *this;
}

void RedBlackTree::insert (int value, bool complete) {
    callbackLoadCode(CodeRepo::RB_TREE_INSERT);
    callbackHighlightCode(0);
    callbackApplyAnimation();

    // iterate to position
    Node *parent = nullptr, *ptr = root;
    callbackHighlightCode(1);
    callbackHighlightNode(getVisualID(root));
    callbackApplyAnimation();

    bool valueFound = false;
    while (ptr != TNULL && !valueFound) {
        callbackHighlightCode(2);
        callbackApplyAnimation();
        parent = ptr;
        if (value == ptr->value) {
            callbackHighlightCode(3);
            valueFound = true;
        }
        else if (value < ptr->value) {
            callbackHighlightCode(4);
            ptr = ptr->lpt;
        }
        else if (value > ptr->value) {
            callbackHighlightCode(5);
            ptr = ptr->rpt;
        }
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
    }

    if (!valueFound) {
        // attach new node to tree
        Node* node = new Node(value, nodeCounter++);
        node->lpt = node->rpt = TNULL;
        callbackCreateNode(value, false);
        callbackColorNode(getVisualID(node), 1);
        callbackHighlightNode(getVisualID(node));
        node->parent = parent;
        if (parent == nullptr) {
            root = node;
            callbackChangeRoot(getVisualID(root));
        }
        else {
            if (value < parent->value) parent->lpt = node;
            else parent->rpt = node;
            callbackAddEdge(getVisualID(parent), getVisualID(node), value < parent->value);
        }
        callbackHighlightCode(6);
        callbackApplyAnimation();

        // fix tree
        if (node->parent == nullptr) {
            node->color = BLACK;
            callbackHighlightCode(15);
            callbackColorNode(getVisualID(node), 2);
            callbackApplyAnimation();
        }
        else if (node->parent->parent != nullptr) insertFixup(node);
    }

    if (complete) {
        callbackHighlightNode(-1);
        callbackLoadCode({});
        callbackCompleteAnimation();
        callbackApplyAnimation();
    }
}

bool RedBlackTree::erase (int value, bool complete) {
    eraseSuccess = false;
    if (root == TNULL) return false;

    callbackLoadCode(CodeRepo::RB_TREE_ERASE_VALUE);
    callbackHighlightCode(0);
    callbackApplyAnimation();

    // find node to erase
    Node *z = TNULL, *ptr = root;
    callbackHighlightCode(1);
    callbackHighlightNode(getVisualID(root));
    callbackApplyAnimation();

    while (ptr != TNULL) {
        if (value == ptr->value) { z = ptr; break; }
        callbackHighlightCode(2);
        callbackApplyAnimation();
        if (value < ptr->value) {
            callbackHighlightCode(3);
            ptr = ptr->lpt;
        }
        else {
            callbackHighlightCode(4);
            ptr = ptr->rpt;
        }
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();
    }

    // start erase
    if (z != TNULL) {
        eraseSuccess = true;
        Node *y = z, *x;
        RBColor yOriginalColor = y->color;

        // simple erase
        if (z->lpt == TNULL) {
            callbackHighlightCode(6);
            callbackApplyAnimation();

            x = z->rpt;
            rbTransplant(z, z->rpt);
            callbackHighlightCode(7);
        }
        else if (z->rpt == TNULL) {
            callbackHighlightCode(6);
            callbackApplyAnimation();

            x = z->lpt;
            rbTransplant(z, z->lpt);
            callbackHighlightCode(7);
        }

        // hard core erase
        else {
            callbackHighlightCode(8);
            callbackApplyAnimation();
            y = getSmallestKey(z->rpt);
            yOriginalColor = y->color;
            x = y->rpt;
            if (y->parent == z) x->parent = y;
            else {
                rbTransplant(y, y->rpt);
                y->rpt = z->rpt;
                callbackAddEdge(getVisualID(y), getVisualID(z->rpt), false);
                y->rpt->parent = y;
            }
            rbTransplant(z, y);
            y->lpt = z->lpt;
            callbackAddEdge(getVisualID(y), getVisualID(z->lpt), true);
            y->lpt->parent = y;
            y->color = z->color;
            callbackColorNode(getVisualID(y), y->color == BLACK ? 2 : 1);
            callbackHighlightCode(10);
        }
        callbackDeleteNode(getVisualID(z));
        callbackApplyAnimation();
        delete z;
        if (yOriginalColor == BLACK) {
            callbackHighlightCode(11);
            callbackApplyAnimation();

            callbackHighlightCode(12);
            callbackApplyAnimation();
            eraseFixup(x);
        }
    }

    // no node to erase
    else {
        callbackHighlightCode(5);
        callbackApplyAnimation();
    }

    if (complete) {
        callbackHighlightNode(-1);
        callbackLoadCode({});
        callbackCompleteAnimation();
        callbackApplyAnimation();
    }
    return true;
}

bool RedBlackTree::search (int value) {
    if (root == TNULL) return false;
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

bool RedBlackTree::update (int oldKey, int newKey) {
    if (root == TNULL) return false;
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