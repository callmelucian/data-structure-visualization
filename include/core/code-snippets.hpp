#pragma once
#include <string>
#include <vector>

namespace CodeRepo {

inline const std::vector<std::string> AVL_TREE_INSERT = {
    "Node* insertValue (Node *ptr, int insertKey) {",
    "   if (ptr == nullptr)",
    "      return new Node(insertKey);",
    "   if (insertKey == ptr->value)",
    "      return ptr;",
    "   if (insertKey < ptr->value)",
    "      ptr->lpt = insertValue(ptr->lpt, insertKey);",
    "   if (insertKey > ptr->value)",
    "      ptr->rpt = insertValue(ptr->rpt, insertKey);",
    "   return selfBalancing(ptr);",
    "}"
};

inline const std::vector<std::string> AVL_TREE_SELF_BALANCING = {
    "Node* selfBalancing (Node* ptr) {",
    "   updateHeight(ptr);",
    "   int balanceFactor = getBalance(ptr);",
    "   if (balanceFactor > 1) {",
    "      if (getBalance(ptr->lpt) < 0)",
    "         ptr->lpt = leftRotation(ptr->lpt);",
    "      return rightRotation(ptr);",
    "   }",
    "   if (balanceFactor < -1) {",
    "      if (getBalance(ptr->rpt) > 0)",
    "         ptr->rpt = rightRotation(ptr->rpt);",
    "      return leftRotation(ptr);",
    "   }",
    "   return ptr;",
    "}"
};

inline const std::vector<std::string> AVL_TREE_LEFT_ROTATION = {
    "Node* leftRotation (Node* ptr) {",
    "    Node* x = ptr->rpt; // is surely a valid pointer",
    "    Node* tmp = x->lpt; // this one could be nullptr",
    "    x->lpt = ptr, ptr->rpt = tmp;",
    "    updateHeight(ptr), updateHeight(x);",
    "    return x;",
    "}"
};

inline const std::vector<std::string> AVL_TREE_RIGHT_ROTATION = {
    "Node* rightRotation (Node* ptr) {",
    "    Node* x = ptr->lpt; // is surely a valid pointer",
    "    Node* tmp = x->rpt; // this one could be nullptr",
    "    x->rpt = ptr, ptr->lpt = tmp;",
    "    updateHeight(ptr), updateHeight(x);",
    "    return x;",
    "}"
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

}; // namespace CodeRepo