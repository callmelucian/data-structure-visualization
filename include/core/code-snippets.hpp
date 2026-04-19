#pragma once
#include <string>
#include <vector>

namespace CodeRepo {

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

inline const std::vector<std::string> AVL_TREE_SELF_BALANCE = {
    "Node* selfBalance (Node* ptr):",
    "    if subtree is left-leaning:",
    "        if leftST is right-leaning: leftRotate",
    "        rightRotate",
    "    if subtree is right-leaning:",
    "        if rightST is left-leaning: rightRotate",
    "        leftRotate",
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