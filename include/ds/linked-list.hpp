#pragma once
#include <vector>
#include <functional>
#include <string>
#include <iostream>

#include "../core/code-snippets.hpp"

namespace DS {

class LinkedList {
private:
    struct Node {
        int value, visualID;
        Node* pNext;

        Node();
        Node (int value, int visualID);
    };
    Node* rootNode;
    int nodeCounter, listSize;

    std::function<void(int,bool)> callbackCreateNode;
    std::function<void(int)> callbackDeleteNode;
    std::function<void(int,int)> callbackAddEdge;
    std::function<void(int)> callbackHighlightNode;
    std::function<void(int)> callbackSetHead;
    std::function<void(int,bool)> callbackColorNode;
    std::function<void(int,int)> callbackChangeValue;
    std::function<void()> callbackApplyAnimation;
    std::function<void()> callbackCompleteAnimation;
    std::function<void(const std::vector<std::string>&)> callbackLoadCode;
    std::function<void(int)> callbackHighlightCode;

    void copyFrom (const LinkedList &other);
    Node* copyLinkedList (Node* ptr);
    void destroyLinkedList (Node* ptr);
    int getVisualID (Node* ptr);

public:
    LinkedList();
    LinkedList (const LinkedList &other);
    ~LinkedList();
    LinkedList& operator= (const LinkedList &other);

    void setCallbackCreateNode(auto func) {
        callbackCreateNode = std::move(func);
    }

    void setCallbackDeleteNode(auto func) {
        callbackDeleteNode = std::move(func);
    }

    void setCallbackAddEdge(auto func) {
        callbackAddEdge = std::move(func);
    }

    void setCallbackHighlightNode(auto func) {
        callbackHighlightNode = std::move(func);
    }

    void setCallbackSetHead (auto func) {
        callbackSetHead = std::move(func);
    }

    void setCallbackColorNode (auto func) {
        callbackColorNode = std::move(func);
    }

    void setCallbackChangeValue (auto func) {
        callbackChangeValue = std::move(func);
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

    void append (int insertKey);
    bool insert (int position, int insertKey);
    void erase (int eraseKey);
    bool update (int oldKey, int newKey);
    bool search (int targetKey);
};

}; // namespace DS