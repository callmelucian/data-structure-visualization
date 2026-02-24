/**
 * This file includes the wrapper structure for singly linked-list
 */

#include "events.hpp"
#include <functional>

struct Node {
    // member variables
    int value, visualID;
    Node *ptr;

    // constructor
    Node() : value(0), visualID(0), ptr(nullptr) {}
    Node (int value, int visualID) : value(value), visualID(visualID), ptr(nullptr) {}
};

struct LinkedList {
    // member variables
    Node *head, *tail;
    std::function<int(UIEvent)> onChange;

    // constructor
    LinkedList() : head(nullptr), tail(nullptr) {}

    // set call-back function to communicate with UI handler
    void setCallback (auto cb) { onChange = cb; }

    // create new node with visual ID
    Node* createNode (int value) {
        int visualID = onChange(UIEvent(EventType::CREATE, value, true, "circle"));
        return new Node(value, visualID);
    }

    // append a node with given value to the end of the linked-list
    void append (int value) {
        if (head == nullptr)
            return head = tail = createNode(value), void();
        tail->ptr = createNode(value);
        onChange(UIEvent(EventType::ADDLINK, tail->visualID, tail->ptr->visualID, true));
        tail = tail->ptr;
    }
};

