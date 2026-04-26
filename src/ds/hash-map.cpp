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
    this->callbackColorNode = other.callbackColorNode;
    this->callbackApplyAnimation = other.callbackApplyAnimation;
    this->callbackCompleteAnimation = other.callbackCompleteAnimation;
    this->callbackLoadCode = other.callbackLoadCode;
    this->callbackHighlightCode = other.callbackHighlightCode;

    rootNode.resize(other.rootNode.size());
    for (int i = 0; i < rootNode.size(); i++)
        rootNode[i] = copyLinkedList(other.rootNode[i]);
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

void HashMap::insert (int value, bool complete) {
    int modulo = value % 17;
    if (modulo < 0) modulo += 17;

    callbackLoadCode(CodeRepo::HASH_MAP_INSERT);
    callbackHighlightCode(0);
    callbackApplyAnimation();

    if (rootNode[modulo] == nullptr) {
        callbackHighlightCode(1);
        callbackCreateNode(value);
        callbackHighlightNode(nodeCounter);
        callbackAttachRoot(modulo, nodeCounter);
        callbackApplyAnimation();
        rootNode[modulo] = new Node(value, nodeCounter++);
    }
    else {
        Node* ptr = rootNode[modulo];
        callbackHighlightCode(2);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();

        if (ptr->pNext != nullptr) {
            callbackHighlightCode(3);
            callbackHighlightNode(getVisualID(ptr));
            callbackApplyAnimation();
        }
        while (ptr->pNext != nullptr) {
            ptr = ptr->pNext;
            callbackHighlightNode(getVisualID(ptr));
            callbackApplyAnimation();
        }

        callbackHighlightCode(4);
        callbackCreateNode(value);
        callbackHighlightNode(nodeCounter);
        callbackAddEdge(getVisualID(ptr), nodeCounter);
        callbackApplyAnimation();
        ptr->pNext = new Node(value, nodeCounter++);
    }

    if (complete) {
        callbackHighlightCode(-1);
        callbackLoadCode({});
        callbackHighlightNode(-1);
        callbackCompleteAnimation();
        callbackApplyAnimation();
    }
}

void HashMap::erase (int eraseKey, bool complete) {
    int modulo = eraseKey % 17;
    if (modulo < 0) modulo += 17;

    callbackLoadCode(CodeRepo::HASH_MAP_ERASE);
    callbackHighlightCode(0);
    callbackApplyAnimation();

    if (rootNode[modulo] == nullptr) {
        callbackHighlightCode(1);
        callbackApplyAnimation();
    }

    else if (rootNode[modulo]->value == eraseKey) {
        callbackHighlightCode(2);
        Node* tmp = rootNode[modulo];
        callbackHighlightNode(getVisualID(tmp));
        callbackApplyAnimation();

        rootNode[modulo] = tmp->pNext;
        callbackHighlightCode(3);
        callbackAttachRoot(modulo, getVisualID(tmp->pNext));
        callbackHighlightNode(-1);
        callbackDeleteNode(getVisualID(tmp));
        callbackApplyAnimation();
        delete tmp;
    }
    
    else {
        Node* ptr = rootNode[modulo];
        callbackHighlightCode(4);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();

        while (ptr->pNext != nullptr && ptr->pNext->value != eraseKey) {
            callbackHighlightCode(5);
            callbackHighlightNode(getVisualID(ptr));
            callbackApplyAnimation();

            ptr = ptr->pNext;
            callbackHighlightCode(6);
            callbackHighlightNode(getVisualID(ptr));
            callbackApplyAnimation();
        }

        if (ptr->pNext != nullptr) {
            Node* toDelete = ptr->pNext;
            callbackHighlightCode(7);
            callbackHighlightNode(getVisualID(toDelete));
            callbackApplyAnimation();

            ptr->pNext = ptr->pNext->pNext;
            callbackHighlightCode(8);
            callbackAddEdge(getVisualID(ptr), getVisualID(ptr->pNext));
            callbackHighlightNode(-1);
            callbackDeleteNode(getVisualID(toDelete));
            callbackApplyAnimation();
            delete toDelete;
        }
    }

    if (complete) {
        callbackHighlightCode(-1);
        callbackLoadCode({});
        callbackHighlightNode(-1);
        callbackCompleteAnimation();
        callbackApplyAnimation();
    }
}

void HashMap::find (int searchKey) {
    callbackLoadCode(CodeRepo::HASH_MAP_SEARCH);
    callbackHighlightCode(0);
    callbackApplyAnimation();

    int modulo = searchKey % 17;
    if (modulo < 0) modulo += 17;

    bool found = false;
    for (Node* ptr = rootNode[modulo]; ptr != nullptr; ptr = ptr->pNext) {
        callbackHighlightCode(1);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();

        if (ptr->value == searchKey) {
            callbackHighlightCode(2);
            callbackColorNode(getVisualID(ptr), true);
            callbackApplyAnimation();

            found = true;
            callbackColorNode(getVisualID(ptr), false);
            callbackApplyAnimation();
            break;
        }
    }
    if (!found) {
        callbackHighlightCode(3);
        callbackApplyAnimation();
    }

    callbackHighlightCode(-1);
    callbackLoadCode({});
    callbackHighlightNode(-1);
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

void HashMap::update (int oldKey, int newKey) {
    erase(oldKey, false);
    insert(newKey, true);
}

}; // namespace DS