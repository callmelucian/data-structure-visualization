#pragma once
#include <functional>

namespace DS {
struct Node {
    int value, visualID, height, count;
    Node *lpt, *rpt;

    Node() :
        value(0), visualID(0), height(0), count(0), lpt(nullptr), rpt(nullptr) {}
    Node (int value, int visualID) :
        value(value), visualID(visualID), height(1), count(1), lpt(nullptr), rpt(nullptr) {}
};

class AVLTree {
private:
    Node *root;
    std::function<int(int, bool)> callbackCreateNode;
    std::function<void(int, int, bool)> callbackAddEdge;
    std::function<void(int)> callbackChangeRoot;
    std::function<void()> callbackReposition;

public:
    AVLTree() : root(nullptr) {}

    void setCallbackCreateNode (auto func) {
        callbackCreateNode = func;
    }
    
    void setCallbackAddEdge (auto func) {
        callbackAddEdge = func;
    }

    void setCallbackChangeRoot (auto func) {
        callbackChangeRoot = func;
    }

    void setCallbackReposition (auto func) {
        callbackReposition = func;
    }

    // get the height of node ptr, O(1)
    int getHeight (Node *ptr) {
        return ptr ? ptr->height : 0;
    }

    // get the balance factor of node ptr, O(1)
    int getBalance (Node *ptr) {
        return ptr ? getHeight(ptr->lpt) - getHeight(ptr->rpt) : 0;
    }

    // recursively update the height of every nodes wihin the subtree given by ptr
    void updateHeight (Node *ptr) {
        ptr->height = 1 + std::max(getHeight(ptr->lpt), getHeight(ptr->rpt));
    }

    // perform left rotation on the subtree given by ptr and return the new root
    Node* leftRotation (Node *ptr) {
        Node* x = ptr->rpt; // is surely a valid pointer
        Node* tmp = x->lpt; // this one could be nullptr

        x->lpt = ptr;
        callbackAddEdge(x->visualID, ptr->visualID, true);
        ptr->rpt = tmp;
        callbackAddEdge(ptr->visualID, (tmp ? tmp->visualID : -1), false);
        updateHeight(ptr), updateHeight(x);
        return x;
    }

    // perform right rotation on the subtree given by ptr and return the new root
    Node* rightRotation (Node *ptr) {
        Node* x = ptr->lpt; // is surely a valid pointer
        Node* tmp = x->rpt; // this one could be nullptr

        x->rpt = ptr;
        callbackAddEdge(x->visualID, ptr->visualID, false);
        ptr->lpt = tmp;
        callbackAddEdge(ptr->visualID, (tmp ? tmp->visualID : -1), true);
        updateHeight(ptr), updateHeight(x);
        return x;
    }

    // perform insertion on the subtree given by ptr and return the new root
    Node* insertValue (Node *ptr, int insertKey) {
        // insertion happen at this node
        if (ptr == nullptr)
            return new Node(insertKey, callbackCreateNode(insertKey, false));
        
        // recursive call for either subtree
        if (insertKey == ptr->value) return ptr->count++, ptr;
        if (insertKey < ptr->value) {
            ptr->lpt = insertValue(ptr->lpt, insertKey);
            callbackAddEdge(ptr->visualID, ptr->lpt->visualID, true);
        }
        if (insertKey > ptr->value) {
            ptr->rpt = insertValue(ptr->rpt, insertKey);
            callbackAddEdge(ptr->visualID, ptr->rpt->visualID, false);
        }

        updateHeight(ptr);
        int balanceFactor = getBalance(ptr);

        // rebalance tree: unbalanceness caused by left subtree
        if (balanceFactor > 1) {
            if (getBalance(ptr->lpt) < 0) { // turn the Left-Right case into the Left-Left case
                ptr->lpt = leftRotation(ptr->lpt);
                callbackAddEdge(ptr->visualID, ptr->lpt->visualID, true);
            }
            // resolve the Left-Left unbalanceness
            return rightRotation(ptr);
        }

        // rebalance tree: unbalanceness caused by right subtree
        if (balanceFactor < -1) {
            if (getBalance(ptr->rpt) > 0) { // turn the Right-Left case into the Right-Right case
                ptr->rpt = rightRotation(ptr->rpt);
                callbackAddEdge(ptr->visualID, ptr->rpt->visualID, false);
            }
            // resolve the Right-Right unbalanceness
            return leftRotation(ptr);
        }

        return ptr;
    }

    void insert (int value) {
        if (root == nullptr)
            root = new Node(value, callbackCreateNode(value, true));
        else {
            root = insertValue(root, value);
            callbackChangeRoot(root->visualID);
        }
        callbackReposition();
    }
};
};