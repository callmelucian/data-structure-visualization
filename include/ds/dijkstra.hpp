#pragma once
#include <functional>
#include <vector>
#include <climits>
#include <queue>
#include <tuple>
#include <string>
#include <iostream>

#include "../core/utility.hpp"
#include "../core/code-snippets.hpp"
#include "../../assets/theme.hpp"

namespace DS {

class DijkstraAlgorithm {
private:
    std::vector<std::vector<std::pair<int,int>>> adjency;
    std::vector<bool> nodeDeleted, edgeDeleted;
    std::vector<int> edgeWeight;
    int graphSize;
    bool isDirected;

    // callback function
    std::function<void(int)> callbackCreateNode;
    std::function<void(int,int,int)> callbackAddEdge;
    std::function<void(int)> callbackDeleteNode;
    std::function<void(int)> callbackDeleteEdge;
    std::function<void(int,int)> callbackEditEdge;
    std::function<void(int,sf::Color)> callbackSetEdgeColor;

    std::function<void()> callbackClearAnnotation;
    std::function<void(int,int)> callbackEditAnnotation;
    std::function<void(int)> callbackMarkAnnotation;

    std::function<void(int)> callbackHighlightNode;
    std::function<void(int)> callbackHighlightEdge;
    std::function<void()> callbackApplyAnimation;
    std::function<void()> callbackCompleteAnimation;

    std::function<void(const std::vector<std::string>&)> callbackLoadCode;
    std::function<void(int)> callbackHighlightCode;

public:
    DijkstraAlgorithm();

    void setCallbackCreateNode (auto func) {
        callbackCreateNode = func;
    }

    void setCallbackAddEdge(auto func) {
        callbackAddEdge = func;
    }

    void setCallbackDeleteNode (auto func) {
        callbackDeleteNode = func;
    }

    void setCallbackDeleteEdge (auto func) {
        callbackDeleteEdge = func;
    }

    void setCallbackEditEdge (auto func) {
        callbackEditEdge = func;
    }

    void setCallbackSetEdgeColor (auto func) {
        callbackSetEdgeColor = func;
    }

    void setCallbackClearAnnotation(auto func) {
        callbackClearAnnotation = func;
    }

    void setCallbackEditAnnotation(auto func) {
        callbackEditAnnotation = func;
    }

    void setCallbackMarkAnnotation(auto func) {
        callbackMarkAnnotation = func;
    }

    void setCallbackHighlightNode (auto func) {
        callbackHighlightNode = func;
    }

    void setCallbackHighlightEdge (auto func) {
        callbackHighlightEdge = func;
    }

    void setCallbackApplyAnimation (auto func) {
        callbackApplyAnimation = func;
    }

    void setCallbackCompleteAnimation (auto func) {
        callbackCompleteAnimation = func;
    }

    void setCallbackLoadCode(auto func) {
        callbackLoadCode = func;
    }

    void setCallbackHighlightCode(auto func) {
        callbackHighlightCode = func;
    }

    void clearUIState();
    
    void makeDirected();

    void createNode();

    void createEdge (int fromNode, int toNode);

    void deleteNode (int targetNode);

    void deleteEdge (int targetEdge);

    bool editEdge (int edgeID, int weight);

    void run (int source);

    void runPrim (int source);
};

}; // namespace DS