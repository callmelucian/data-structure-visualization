#include "../../include/ds/dijkstra.hpp"

namespace DS {

DijkstraAlgorithm::DijkstraAlgorithm() : graphSize(0) {}

void DijkstraAlgorithm::createNode() {
    adjency.push_back(std::vector<std::pair<int,int>>(0));
    nodeDeleted.push_back(false);
    callbackCreateNode(graphSize++);
    callbackClearAnnotation();
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

void DijkstraAlgorithm::createEdge (int fromNode, int toNode) {
    adjency[fromNode].emplace_back(toNode, edgeWeight.size());
    edgeWeight.push_back(randInt(0, 99));
    edgeDeleted.push_back(false);
    callbackAddEdge(fromNode, toNode, edgeWeight.back());
    callbackClearAnnotation();
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

void DijkstraAlgorithm::deleteNode (int targetNode) {
    nodeDeleted[targetNode] = true;
    for (int fromNode = 0; fromNode < graphSize; fromNode++) {
        for (auto [toNode, edgeID] : adjency[fromNode])
            edgeDeleted[edgeID] = true;
    }
    callbackDeleteNode(targetNode);
    callbackClearAnnotation();
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

void DijkstraAlgorithm::deleteEdge (int targetEdge) {
    edgeDeleted[targetEdge] = true;
    callbackDeleteEdge(targetEdge);
    callbackClearAnnotation();
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

bool DijkstraAlgorithm::editEdge (int targetEdge, int weight) {
    if (edgeWeight[targetEdge] == weight) return false;
    edgeWeight[targetEdge] = weight;
    callbackEditEdge(targetEdge, weight);
    callbackClearAnnotation();
    callbackCompleteAnimation();
    callbackApplyAnimation();
    return true;
}

void DijkstraAlgorithm::run (int source) {
    if (nodeDeleted[source]) return;

    // Logic intialization
    std::vector<int> dist(graphSize, INT_MAX);
    std::vector<bool> vist(graphSize, false);
    using Item = std::pair<long long, int>;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> pq;

    // UI initialization
    callbackLoadCode(CodeRepo::DIJKSTRA_CODE);
    callbackClearAnnotation();
    for (int i = 0; i < graphSize; i++)
        if (!nodeDeleted[i]) callbackEditAnnotation(i, INT_MAX);
            // callbackEditAnnotation(i, (i == source ? 0 : INT_MAX));
    callbackHighlightCode(0);
    callbackApplyAnimation();

    // Run Dijkstra algorithm
    pq.emplace(0, source), dist[source] = 0;
    callbackEditAnnotation(source, 0);
    callbackHighlightCode(1);
    callbackApplyAnimation();

    while (pq.size()) {
        int curNode = pq.top().second; pq.pop();
        if (vist[curNode]) continue;

        callbackHighlightCode(2);
        callbackApplyAnimation();

        callbackHighlightNode(curNode);
        callbackHighlightCode(3);
        callbackApplyAnimation();

        vist[curNode] = true;
        callbackHighlightCode(4);
        callbackMarkAnnotation(curNode);
        callbackApplyAnimation();

        for (auto [nextNode, edgeID] : adjency[curNode]) {
            if (edgeDeleted[edgeID]) continue;
            callbackHighlightEdge(edgeID);
            callbackHighlightCode(5);
            callbackApplyAnimation();

            int weight = edgeWeight[edgeID];
            callbackHighlightCode(6);
            if (dist[curNode] + weight < dist[nextNode]) {
                dist[nextNode] = dist[curNode] + weight;
                pq.emplace(dist[nextNode], nextNode);
                callbackEditAnnotation(nextNode, dist[nextNode]);
            }
            callbackApplyAnimation();

            callbackHighlightEdge(-1);
            callbackApplyAnimation();
        }
    }

    for (int i = 0; i < graphSize; i++)
        if (dist[i] == INT_MAX && !nodeDeleted[i])
            callbackMarkAnnotation(i);
    
    callbackHighlightNode(-1);
    callbackLoadCode({});
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

}; // namespace DS