#pragma once
#include <string>
#include <vector>

namespace CodeRepo {

inline const std::vector<std::string> HASH_MAP_INSERT = {
    "void insertValue (int value):",
    "    if root[value % 17] is empty: create new node and return",
    "    ptr = rootNode[value % 17]",
    "    while ptr->pNext != nullptr:",
    "        ptr = ptr->pNext",
    "    ptr->pNext = new node"
};

inline const std::vector<std::string> LINKED_LIST_APPEND = {
    "void appendValue (int value):",
    "    if empty: head = new node and return",
    "    ptr = head",
    "    while ptr->pNext != nullptr: ptr = ptr->pNext",
    "    ptr->pNext = new node"
};

inline const std::vector<std::string> LINKED_LIST_INSERT = {
    "void insertValue (int position, int insertKey):",
    "    if position == 0: insert at head and return",
    "    let ptr = head",
    "    loop position - 1 times: cur = cur->pNext",
    "    newNode = new node",
    "    newNode->pNext = cur->pNext, cur->pNext = newNode"
};

inline const std::vector<std::string> HASH_MAP_ERASE = {
    "void eraseValue (int value):",
    "    if root[value % 17] is empty: return",
    "    if root[value % 17]->value == value:",
    "        delete head, make new head and return",
    "    Node* ptr = rootNode[eraseKey % 17]",
    "    while ptr->pNext != nullptr and ptr->pNext->value != value:",
    "        ptr = ptr->pNext",
    "    if ptr->pNext != nulltr:",
    "        delete ptr->pNext and make new link"
};

inline const std::vector<std::string> LINKED_LIST_ERASE = {
    "void eraseValue (int value):",
    "    if empty: return",
    "    if head->value == value: delete head and return",
    "    ptr = head",
    "    while ptr->pNext != nullptr and ptr->pNext->value != value:",
    "        ptr = ptr->pNext",
    "    if ptr->pNext != nulltr:",
    "        delete ptr->pNext"
};

inline const std::vector<std::string> LINKED_LIST_UPDATE = {
    "void update (int oldKey, int newKey):",
    "    for (ptr = head; ptr != nullptr; ptr = ptr->pNext):",
    "        if ptr->value == oldKey: ptr->value = newKey",
};

inline const std::vector<std::string> LINKED_LIST_SEARCH = {
    "bool search (int searchKey):",
    "    for (ptr = head; ptr != nullptr; ptr = ptr->pNext):",
    "        if ptr->value == searchKey: return true",
    "    return false"
};

inline const std::vector<std::string> AVL_TREE_INSERT = {
    "Node* insertValue (Node* ptr, int insertKey):",
    "    if ptr == nullptr: return new Node(insertKey);",
    "    if insertKey == ptr->value: return ptr",
    "    if insertKey < ptr->value:",
    "        ptr->leftST = insertValue(ptr->leftST, insertKey)",
    "    if insertKey > ptr->value:",
    "        ptr->rightST = insertValue(ptr->rightST, insertKey)",
    "    return selfBalance(ptr)"
};

inline const std::vector<std::string> RB_TREE_INSERT = {
    "Node* insertValue (Node* ptr, int insertKey):",
    "    if ptr == nullptr: return new Node(insertKey, RED);",
    "    if insertKey == ptr->value: return ptr",
    "    if insertKey < ptr->value:",
    "        ptr->leftST = insertValue(ptr->leftST, insertKey)",
    "    if insertKey > ptr->value:",
    "        ptr->rightST = insertValue(ptr->rightST, insertKey)",
    "    if ptr is root: ptr->color = BLACK",
    "    return selfBalance(ptr)"
};

inline const std::vector<std::string> RB_TREE_SELF_BALANCE = {
    "Node* selfBalance (Node* ptr):",
    "    if right child is red and left child is black: leftRotate",
    "    if both left and left-left children are red: rightRotate",
    "    if both left and right children are red: flipColors",
    "    return ptr"
};

inline const std::vector<std::string> RB_TREE_ERASE_VALUE = {
    "Node* eraseValue (Node* ptr, int eraseKey):",
    "    if ptr == nullptr: return nullptr",
    "    if eraseKey < ptr->value:",
    "        if both left and left-left children are black/empty:",
    "            ptr = moveRedLeft(ptr)",
    "        ptr->leftST = eraseValue(ptr->leftST, eraseKey)",
    "    if eraseKey >= ptr->value:",
    "        if left child is red: rightRotate current subtree",
    "        if eraseKey == ptr->value and current subtree contains 1 node:",
    "            delete current node and return nullptr",
    "        if both right and right-left children are black/empty:",
    "            ptr = moveRedRight(ptr)",
    "        if eraseKey == ptr->value:",
    "            swap current node with the smallest value among rightST",
    "        ptr->rightST = eraseValue(ptr->rightST, eraseKey)",
    "    if ptr is root: ptr->color = BLACK",
    "    return selfBalance(ptr)"
};

inline const std::vector<std::string> AVL_TREE_SELF_BALANCE = {
    "Node* selfBalance (Node* ptr):",
    "    if subtree is left-leaning:",
    "        if leftST is right-leaning: leftRotate leftST",
    "        rightRotate current subtree",
    "    if subtree is right-leaning:",
    "        if rightST is left-leaning: rightRotate rightST",
    "        leftRotate current subtree",
    "    return ptr"
};

inline const std::vector<std::string> AVL_TREE_ERASE = {
    "Node* eraseValue (Node* ptr, int eraseKey):",
    "    if ptr == nullptr: return nullptr",
    "    if eraseKey == ptr->value:",
    "        if current node has one child:",
    "            delete node and bring the other subtree up",
    "        let tmp be smallest value among the right subtree",
    "        swap current node with tmp",
    "        ptr->rightST = eraseValue(ptr->rightST, tmp)",
    "    if eraseKey < ptr->value:",
    "        ptr->leftST = eraseValue(ptr->leftST, eraseKey)",
    "    if eraseKey > ptr->value:",
    "        ptr->rightST = eraseValue(ptr->rightST, eraseKey)",
    "    return selfBalance(ptr)"
};

inline const std::vector<std::string> AVL_TREE_SEARCH = {
    "bool searchValue (Node* ptr, int searchKey):",
    "    if ptr == nullptr: return false",
    "    if searchKey == ptr->value: return true",
    "    if searchKey < ptr->value:",
    "        return search(ptr->lpt, searchKey)",
    "    if searchKey > ptr->value:",
    "        return search(ptr->rpt, searchKey)",
};

inline const std::vector<std::string> DIJKSTRA_CODE = {
    "intialize all nodes with infinity distance",
    "distance[source] := 0",
    "while queue is not empty:",
    "    let u be the unvisited node with smallest distance",
    "    visited[u] := true",
    "    for v as neighbors of u:",
    "        distance[v] := min(distance[v], distance[u] + weight)"
};

inline const std::vector<std::string> PRIM_CODE = {
    "initialize all nodes with infinity weight",
    "weight[source] := 0",
    "while queue is not empty:",
    "    let u be the unvisited node with smallest weight",
    "    if u != source: select the corresponding edge",
    "    visited[u] := true",
    "    for v as neighbors of u:",
    "        weight[v] := min(weight[v], edgeWeight)"
};

}; // namespace CodeRepo