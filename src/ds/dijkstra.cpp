#include "../../include/ds/dijkstra.hpp"

namespace DS {

DijkstraAlgorithm::DijkstraAlgorithm() : graphSize(0) {}

void DijkstraAlgorithm::createNode() {
    adjency.push_back(std::vector<std::pair<int,int>>(0));
    nodeDeleted.push_back(false);
    callbackCreateNode(graphSize++);
    callbackApplyAnimation();
}

void DijkstraAlgorithm::createEdge (int fromNode, int toNode) {
    adjency[fromNode].emplace_back(toNode, edgeWeight.size());
    edgeWeight.push_back(randInt(0, 99));
    edgeDeleted.push_back(false);
    callbackAddEdge(fromNode, toNode, edgeWeight.back());
    callbackApplyAnimation();
}

void DijkstraAlgorithm::deleteNode (int targetNode) {
    nodeDeleted[targetNode] = true;
    for (int fromNode = 0; fromNode < graphSize; fromNode++) {
        for (auto [toNode, edgeID] : adjency[fromNode])
            edgeDeleted[edgeID] = true;
    }
    callbackDeleteNode(targetNode);
    callbackApplyAnimation();
}

void DijkstraAlgorithm::deleteEdge (int targetEdge) {
    edgeDeleted[targetEdge] = true;
    callbackDeleteEdge(targetEdge);
    callbackApplyAnimation();
}

bool DijkstraAlgorithm::editEdge (int targetEdge, int weight) {
    if (edgeWeight[targetEdge] == weight) return false;
    edgeWeight[targetEdge] = weight;
    callbackEditEdge(targetEdge, weight);
    callbackApplyAnimation();
    return true;
}

void DijkstraAlgorithm::run (int source) {
    // Logic intialization
    std::vector<int> dist(graphSize, INT_MAX);
    std::vector<bool> vist(graphSize, false);
    using Item = std::pair<long long, int>;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> pq;

    // UI initialization
    callbackClearAnnotation();
    for (int i = 0; i < graphSize; i++)
        if (!nodeDeleted[i])
            callbackEditAnnotation(i, (i == source ? 0 : INT_MAX));
    callbackApplyAnimation();

    // Run Dijkstra algorithm
    while (pq.size()) {
        int curNode = pq.top().second; pq.pop();
        callbackHighlightNode(curNode);
        if (vist[curNode]) continue;
        callbackMarkAnnotation(curNode);

        for (auto [nextNode, edgeID] : adjency[curNode]) {
            if (edgeDeleted[edgeID]) continue;
            callbackHighlightEdge(edgeID);
            int weight = edgeWeight[edgeID];
            if (dist[curNode] + weight < dist[nextNode]) {
                dist[nextNode] = dist[curNode] + weight;
                pq.emplace(dist[nextNode], nextNode);
                callbackEditAnnotation(nextNode, dist[nextNode]);
            }
            callbackHighlightEdge(-1);
        }
    }

    for (int i = 0; i < graphSize; i++)
        if (dist[i] == INT_MAX && !nodeDeleted[i])
            callbackMarkAnnotation(i);
}

}; // namespace DS