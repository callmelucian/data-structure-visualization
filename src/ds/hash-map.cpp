#include "../../include/ds/hash-map.hpp"

namespace DS {

HashMap::HashMap() : rootNode(17, nullptr), nodeCounter(0) {}

HashMap::HashMap (const HashMap &other) {
    copyFrom(other);
}

HashMap::~HashMap() {
    for (Node* ptr : rootNode) destroyLinkedList(ptr);
}

HashMap& HashMap::operator= (const HashMap &other) {
    if (this == &other) return *this;
    for (Node* ptr : rootNode) destroyLinkedList(ptr);
    rootNode.clear();
    return copyFrom(other), *this;
}

void HashMap::copyFrom (const HashMap &other) {
    this->nodeCounter = other.nodeCounter;
    this->callbackCreateNode = other.callbackCreateNode;
    this->callbackDeleteNode = other.callbackDeleteNode;
    this->callbackAddEdge = other.callbackAddEdge;
    this->callbackAttachRoot = other.callbackAttachRoot;
    this->callbackHighlightNode = other.callbackHighlightNode;
    this->callbackApplyAnimation = other.callbackApplyAnimation;
    this->callbackCompleteAnimation = other.callbackCompleteAnimation;
    this->callbackLoadCode = other.callbackLoadCode;
    this->callbackHighlightCode = other.callbackHighlightCode;

    rootNode.resize(other.rootNode.size());
    for (Node* &ptr : rootNode) ptr = copyLinkedList(ptr); 
}

HashMap::Node* HashMap::copyLinkedList (Node* ptr) {
    if (ptr == nullptr) return nullptr;
    Node* head = new Node(*ptr);
    head->pNext = copyLinkedList(ptr->pNext);
    return head;
}

void HashMap::destroyLinkedList (Node* ptr) {
    if (ptr == nullptr) return;
    destroyLinkedList(ptr->pNext);
    delete ptr;
}

int HashMap::getVisualID (Node* ptr) {
    return ptr ? ptr->visualID : -1;
}

void HashMap::insert (int value) {
    callbackLoadCode(CodeRepo::HASH_MAP_INSERT);
    callbackHighlightCode(0);
    callbackApplyAnimation();

    if (rootNode[value % 17] == nullptr) {
        callbackHighlightCode(1);
        callbackCreateNode(value);
        callbackHighlightCode(nodeCounter);
        callbackApplyAnimation();
        rootNode[value % 17] = new Node(value, nodeCounter++);
    }
    else {
        callbackHighlightCode(2);
        callbackApplyAnimation();

        Node* ptr = rootNode[value % 17];
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
        callbackCreateNode(value);
        callbackAddEdge(getVisualID(ptr), nodeCounter);
        callbackApplyAnimation();
        ptr->pNext = new Node(value, nodeCounter++);
    }

    callbackHighlightCode(-1);
    callbackLoadCode({});
    callbackHighlightNode(-1);
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

void HashMap::erase (int eraseKey) {
    callbackLoadCode(CodeRepo::HASH_MAP_ERASE);
    callbackHighlightCode(0);
    callbackApplyAnimation();

    if (rootNode[eraseKey % 17] == nullptr) {
        callbackHighlightCode(1);
        callbackApplyAnimation();
        return;
    }

    if (rootNode[eraseKey % 17]->value == eraseKey) {
        callbackHighlightCode(2);
        Node* tmp = rootNode[eraseKey % 17];
        callbackHighlightNode(getVisualID(tmp));
        callbackApplyAnimation();

        rootNode[eraseKey % 17] = tmp->pNext;
        callbackHighlightCode(3);
        callbackAttachRoot(eraseKey % 17, getVisualID(tmp->pNext));
        callbackHighlightNode(-1);
        callbackDeleteNode(getVisualID(tmp));
        callbackApplyAnimation();
        delete tmp;
    }
    
    else {
        callbackHighlightCode(4);
        callbackApplyAnimation();

        Node* ptr = rootNode[eraseKey % 17];
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