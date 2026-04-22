#include "../../include/ds/linked-list.hpp"

namespace DS {

// ===== STRUCT NODE =====
LinkedList::Node::Node() : value(0), visualID(0), pNext(nullptr) {}
LinkedList::Node::Node (int value, int visualID) : value(value), visualID(visualID), pNext(nullptr) {}

// ===== CONSTRUCTORS & DESTRUCTORS =====
LinkedList::LinkedList() : rootNode(nullptr), nodeCounter(0) {}
LinkedList::LinkedList (const LinkedList &other) {
    copyFrom(other);
}

LinkedList::~LinkedList() {
    destroyLinkedList(rootNode);
}

LinkedList& LinkedList::operator= (const LinkedList &other) {
    if (this == &other) return *this;
    destroyLinkedList(rootNode);
    return copyFrom(other), *this;
}

// ===== PRIVATE FUNCTIONS =====
void LinkedList::copyFrom (const LinkedList &other) {
    this->nodeCounter = other.nodeCounter;
    this->callbackCreateNode = other.callbackCreateNode;
    this->callbackDeleteNode = other.callbackDeleteNode;
    this->callbackAddEdge = other.callbackAddEdge;
    this->callbackHighlightNode = other.callbackHighlightNode;
    this->callbackSetHead = other.callbackSetHead;
    this->callbackApplyAnimation = other.callbackApplyAnimation;
    this->callbackCompleteAnimation = other.callbackCompleteAnimation;
    this->callbackLoadCode = other.callbackLoadCode;
    this->callbackHighlightCode = other.callbackHighlightCode;
    rootNode = copyLinkedList(other.rootNode);
}

LinkedList::Node* LinkedList::copyLinkedList (Node* ptr) {
    if (ptr == nullptr) return nullptr;
    Node* curr = new Node(*ptr);
    return curr->pNext = copyLinkedList(ptr->pNext), curr;
}

void LinkedList::destroyLinkedList (Node* ptr) {
    if (ptr == nullptr) return;
    destroyLinkedList(ptr->pNext), delete ptr;
}

int LinkedList::getVisualID (Node* ptr) {
    return ptr ? ptr->visualID : -1;
}

// ===== PUBLIC FUNCTIONS =====
void LinkedList::appendElement (int insertKey) {
    callbackLoadCode(CodeRepo::LINKED_LIST_INSERT);
    callbackHighlightCode(0);
    callbackApplyAnimation();

    if (rootNode == nullptr) {
        callbackHighlightCode(1);
        callbackCreateNode(insertKey, true);
        callbackHighlightNode(nodeCounter);
        callbackApplyAnimation();
        rootNode = new Node(insertKey, nodeCounter++);
    }
    else {
        callbackHighlightCode(2);
        callbackApplyAnimation();

        Node* ptr = rootNode;
        callbackHighlightCode(3);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();

        while (ptr->pNext != nullptr) {
            callbackHighlightCode(4);
            callbackHighlightNode(getVisualID(ptr));
            callbackApplyAnimation();

            ptr = ptr->pNext;
            callbackHighlightCode(5);
            callbackHighlightNode(getVisualID(ptr));
            callbackApplyAnimation();
        }

        callbackHighlightCode(6);
        callbackCreateNode(insertKey, false);
        callbackHighlightNode(nodeCounter);
        callbackAddEdge(getVisualID(ptr), nodeCounter);
        callbackApplyAnimation();
        ptr->pNext = new Node(insertKey, nodeCounter++);
    }

    callbackHighlightCode(-1);
    callbackLoadCode({});
    callbackHighlightNode(-1);
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

void LinkedList::erase (int eraseKey) {
    callbackLoadCode(CodeRepo::LINKED_LIST_ERASE);
    callbackHighlightCode(0);
    callbackApplyAnimation();

    if (rootNode == nullptr) {
        callbackHighlightCode(1);
        callbackApplyAnimation();
    }

    else if (rootNode->value == eraseKey) {
        callbackHighlightCode(2);
        Node* tmp = rootNode;
        callbackHighlightNode(getVisualID(tmp));
        callbackApplyAnimation();

        rootNode = tmp->pNext;
        callbackSetHead(getVisualID(tmp->pNext));
        callbackHighlightNode(-1);
        callbackDeleteNode(getVisualID(tmp));
        callbackApplyAnimation();
        delete tmp;
    }
    
    else {
        callbackHighlightCode(4);
        callbackApplyAnimation();

        Node* ptr = rootNode;
        callbackHighlightCode(5);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();

        while (ptr->pNext != nullptr && ptr->pNext->value != eraseKey) {
            callbackHighlightCode(6);
            callbackHighlightNode(getVisualID(ptr));
            callbackApplyAnimation();

            ptr = ptr->pNext;
            callbackHighlightCode(7);
            callbackHighlightNode(getVisualID(ptr));
            callbackApplyAnimation();
        }

        if (ptr->pNext != nullptr) {
            Node* toDelete = ptr->pNext;
            callbackHighlightCode(8);
            callbackHighlightNode(getVisualID(toDelete));
            callbackApplyAnimation();

            ptr->pNext = ptr->pNext->pNext;
            callbackHighlightCode(9);
            callbackAddEdge(getVisualID(ptr), getVisualID(ptr->pNext));
            callbackHighlightNode(-1);
            callbackDeleteNode(getVisualID(toDelete));
            callbackApplyAnimation();
            delete toDelete;
        }
    }

    callbackHighlightCode(-1);
    callbackLoadCode({});
    callbackHighlightNode(-1);
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

}; // namespace DS