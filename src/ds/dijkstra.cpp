#include "../../include/ds/dijkstra.hpp"

namespace DS {

void DijkstraAlgorithm::run (int source) {
    // Logic intialization
    std::vector<int> dist(graphSize, INT_MAX);
    std::vector<bool> vist(graphSize, false);
    using Item = std::pair<long long, int>;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> pq;

    // UI initialization
    callbackClearAnnotation();
    for (int i = 0; i < graphSize; i++)
        callbackEditAnnotation(i, (i == source ? 0 : INT_MAX));
    callbackApplyAnimation();

    // Run Dijkstra algorithm
    while (pq.size()) {
        int curNode = pq.top().second; pq.pop();
        callbackHighlightNode(curNode);
        if (vist[curNode]) continue;
        callbackMarkAnnotation(curNode);

        for (auto [nextNode, weight, edgeID] : adjency[curNode]) {
            callbackHighlightEdge(edgeID);
            if (dist[curNode] + weight < dist[nextNode]) {
                dist[nextNode] = dist[curNode] + weight;
                pq.emplace(dist[nextNode], nextNode);
                callbackEditAnnotation(nextNode, dist[nextNode]);
            }
            callbackHighlightEdge(-1);
        }
    }

    for (int i = 0; i < graphSize; i++)
        if (dist[i] == INT_MAX) callbackMarkAnnotation(i);
}

}; // namespace DS