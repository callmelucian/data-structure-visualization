#pragma once
#include <functional>
#include <vector>
#include <climits>
#include <queue>
#include <tuple>
#include <string>

namespace DS {

class DijkstraAlgorithm {
private:
    std::vector<std::vector<std::tuple<int,int,int>>> adjency;
    int graphSize;

    // callback function
    std::function<void(int)> callbackCreateNode;
    std::function<void(int,int)> callbackAddEdge;
    std::function<void()> callbackClearAnnotation;
    std::function<void(int,int)> callbackEditAnnotation;
    std::function<void(int)> callbackMarkAnnotation;
    std::function<void(int)> callbackHighlightNode;
    std::function<void(int)> callbackHighlightEdge;
    std::function<void()> callbackApplyAnimation;
    std::function<void(const std::vector<std::string>&)> callbackLoadCode;
    std::function<void(int)> callbackHighlightCode;

public:
    void setCallbackCreateNode (auto func) {
        callbackCreateNode = func;
    }

    void setCallbackAddEdge(auto func) {
        callbackAddEdge = func;
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

    void setCallbackLoadCode(auto func) {
        callbackLoadCode = func;
    }

    void setCallbackHighlightCode(auto func) {
        callbackHighlightCode = func;
    }

    void run (int source);
};

}; // namespace DS