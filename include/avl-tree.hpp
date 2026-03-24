#pragma once
#include <functional>

namespace DS {
struct Node {
    int value, visualID;
    Node *lpt, *rpt;

    Node() : value(0), lpt(nullptr), rpt(nullptr) {}
    Node (int value, int visualID) : value(value), visualID(visualID), lpt(nullptr), rpt(nullptr) {}
};

class AVLTree {
private:
    Node *root;
    std::function<int(int, bool)> callbackCreateNode;
    std::function<void(int, int, bool)> callbackAddEdge;

public:
    AVLTree() : root(nullptr) {}

    void setCallbackCreateNode (auto func) {
        callbackCreateNode = func;
    }
    
    void setCallbackAddEdge (auto func) {
        callbackAddEdge = func;
    }

    void insert (int value) {
        if (root == nullptr) {
            int visualID = callbackCreateNode(value, true);
            root = new Node(value, visualID);
            return;
        }
        Node *ptr = root;
        while (ptr) {
            if (value == ptr->value) return;
            else if (value < ptr->value) {
                if (ptr->lpt == nullptr) {
                    ptr->lpt = new Node(value, callbackCreateNode(value, false));
                    callbackAddEdge(ptr->visualID, ptr->lpt->visualID, true);
                    return;
                }
                ptr = ptr->lpt;
            }
            else if (value > ptr->value) {
                if (ptr->rpt == nullptr) {
                    ptr->rpt = new Node(value, callbackCreateNode(value, false));
                    callbackAddEdge(ptr->visualID, ptr->rpt->visualID, false);
                    return;
                }
                ptr = ptr->rpt;
            }
        }
    }
};
};