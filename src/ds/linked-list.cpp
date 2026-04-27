#include "../../include/ds/linked-list.hpp"

namespace DS {

// ===== STRUCT NODE =====
LinkedList::Node::Node() : value(0), visualID(0), pNext(nullptr) {}
LinkedList::Node::Node (int value, int visualID) : value(value), visualID(visualID), pNext(nullptr) {}

// ===== CONSTRUCTORS & DESTRUCTORS =====
LinkedList::LinkedList() : rootNode(nullptr), nodeCounter(0), listSize(0) {}
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
    this->callbackColorNode = other.callbackColorNode;
    this->callbackChangeValue = other.callbackChangeValue;
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
void LinkedList::append (int insertKey) {
    callbackLoadCode(CodeRepo::LINKED_LIST_APPEND);
    callbackHighlightCode(0);
    callbackApplyAnimation();

    if (rootNode == nullptr) {
        callbackHighlightCode(1);
        callbackCreateNode(insertKey, true);
        callbackHighlightNode(nodeCounter);
        callbackApplyAnimation();
        rootNode = new Node(insertKey, nodeCounter++);
        listSize++;
    }
    else {
        Node* ptr = rootNode;
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
        callbackCreateNode(insertKey, false);
        callbackHighlightNode(nodeCounter);
        callbackAddEdge(getVisualID(ptr), nodeCounter);
        callbackApplyAnimation();
        ptr->pNext = new Node(insertKey, nodeCounter++);
        listSize++;
    }

    callbackHighlightCode(-1);
    callbackLoadCode({});
    callbackHighlightNode(-1);
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

bool LinkedList::insert (int position, int insertKey) {
    if (position > listSize) return false;
    callbackLoadCode(CodeRepo::LINKED_LIST_INSERT);
    callbackHighlightCode(0);
    callbackApplyAnimation();

    if (position == 0) {
        callbackHighlightCode(1);
        callbackCreateNode(insertKey, true);
        callbackHighlightNode(nodeCounter);
        Node* newHead = new Node(insertKey, nodeCounter++);
        callbackAddEdge(getVisualID(newHead), getVisualID(rootNode));
        newHead->pNext = rootNode, rootNode = newHead;
        callbackApplyAnimation();
    }

    else {
        Node* ptr = rootNode;
        callbackHighlightCode(2);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();

        for (int i = 1; i < position; i++) {
            ptr = ptr->pNext;
            callbackHighlightCode(3);
            callbackHighlightNode(getVisualID(ptr));
            callbackApplyAnimation();
        }

        callbackHighlightCode(4);
        callbackCreateNode(insertKey, false);
        callbackHighlightNode(nodeCounter);
        Node* newNode = new Node(insertKey, nodeCounter++);
        if (ptr->pNext) callbackAddEdge(getVisualID(newNode), getVisualID(ptr->pNext));
        callbackAddEdge(getVisualID(ptr), getVisualID(newNode));
        callbackApplyAnimation();

        newNode->pNext = ptr->pNext;
        ptr->pNext = newNode;

        callbackHighlightCode(5);
        callbackApplyAnimation();
    }

    callbackHighlightCode(-1);
    callbackLoadCode({});
    callbackHighlightNode(-1);
    callbackCompleteAnimation();
    callbackApplyAnimation();
    return true;
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
        listSize--;
    }
    
    else {
        Node* ptr = rootNode;
        callbackHighlightCode(3);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();

        while (ptr->pNext != nullptr && ptr->pNext->value != eraseKey) {
            callbackHighlightCode(4);
            callbackHighlightNode(getVisualID(ptr));
            callbackApplyAnimation();

            ptr = ptr->pNext;
            callbackHighlightCode(5);
            callbackHighlightNode(getVisualID(ptr));
            callbackApplyAnimation();
        }

        if (ptr->pNext != nullptr) {
            Node* toDelete = ptr->pNext;
            callbackHighlightCode(6);
            callbackHighlightNode(getVisualID(toDelete));
            callbackApplyAnimation();

            ptr->pNext = ptr->pNext->pNext;
            callbackHighlightCode(7);
            callbackAddEdge(getVisualID(ptr), getVisualID(ptr->pNext));
            callbackHighlightNode(-1);
            callbackDeleteNode(getVisualID(toDelete));
            callbackApplyAnimation();
            delete toDelete;
            listSize--;
        }
    }

    callbackHighlightCode(-1);
    callbackLoadCode({});
    callbackHighlightNode(-1);
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

bool LinkedList::update (int oldKey, int newKey) {
    if (rootNode == nullptr) return false;
    callbackLoadCode(CodeRepo::LINKED_LIST_UPDATE);
    callbackHighlightCode(0);
    callbackApplyAnimation();

    for (Node* ptr = rootNode; ptr != nullptr; ptr = ptr->pNext) {
        callbackHighlightCode(1);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();

        if (ptr->value == oldKey) {
            callbackHighlightCode(2);
            callbackChangeValue(getVisualID(ptr), newKey);
            callbackApplyAnimation();
            ptr->value = newKey;
        }
    }

    callbackHighlightCode(-1);
    callbackLoadCode({});
    callbackHighlightNode(-1);
    callbackCompleteAnimation();
    callbackApplyAnimation();
    return true;
}

bool LinkedList::search (int searchKey) {
    if (rootNode == nullptr) return false;
    callbackLoadCode(CodeRepo::LINKED_LIST_SEARCH);
    callbackHighlightCode(0);
    callbackApplyAnimation();

    bool found = false;
    for (Node* ptr = rootNode; ptr != nullptr; ptr = ptr->pNext) {
        callbackHighlightCode(1);
        callbackHighlightNode(getVisualID(ptr));
        callbackApplyAnimation();

        if (ptr->value == searchKey) {
            callbackHighlightCode(2);
            callbackColorNode(getVisualID(ptr), true);
            callbackApplyAnimation();

            found = true;
            callbackColorNode(getVisualID(ptr), false);
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
    return true;
}

}; // namespace DS