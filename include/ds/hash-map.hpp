#pragma once
#include <vector>
#include <functional>
#include <string>
#include <iostream>

#include "../core/code-snippets.hpp"

namespace DS {

class HashMap {
private:
    struct Node {
        int value, visualID;
        Node* pNext;

        Node() : value(0), visualID(0), pNext(nullptr) {}
        Node (int value, int visualID) :
            value(value), visualID(visualID), pNext(nullptr) {}
    };
    std::vector<Node*> rootNode;
    int nodeCounter;

    std::function<void(int)> callbackCreateNode;
    std::function<void(int)> callbackDeleteNode;
    std::function<void(int,int)> callbackAddEdge;
    std::function<void(int,int)> callbackAttachRoot;
    std::function<void(int)> callbackHighlightNode;
    std::function<void()> callbackApplyAnimation;
    std::function<void()> callbackCompleteAnimation;
    std::function<void(const std::vector<std::string>&)> callbackLoadCode;
    std::function<void(int)> callbackHighlightCode;

    void copyFrom (const HashMap &other);
    Node* copyLinkedList (Node* ptr);
    void destroyLinkedList (Node* ptr);
    int getVisualID (Node* ptr);

public:
    HashMap();
    HashMap (const HashMap &other);
    ~HashMap();

    HashMap& operator= (const HashMap &other);

    void setCallbackCreateNode(auto func) {
        callbackCreateNode = std::move(func);
    }

    void setCallbackDeleteNode(auto func) {
        callbackDeleteNode = std::move(func);
    }

    void setCallbackAddEdge(auto func) {
        callbackAddEdge = std::move(func);
    }

    void setCallbackAttachRoot(auto func) {
        callbackAttachRoot = std::move(func);
    }

    void setCallbackHighlightNode(auto func) {
        callbackHighlightNode = std::move(func);
    }

    void setCallbackApplyAnimation(auto func) {
        callbackApplyAnimation = std::move(func);
    }

    void setCallbackCompleteAnimation(auto func) {
        callbackCompleteAnimation = std::move(func);
    }

    void setCallbackLoadCode(auto func) {
        callbackLoadCode = std::move(func);
    }

    void setCallbackHighlightCode(auto func) {
        callbackHighlightCode = std::move(func);
    }

    void insert (int value);
    void erase (int eraseKey);
    void find (int value);
};

}; // namespace DS