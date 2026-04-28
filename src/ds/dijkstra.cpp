#include "../../include/ds/dijkstra.hpp"

namespace DS {

DijkstraAlgorithm::DijkstraAlgorithm() : graphSize(0), isDirected(false) {}

void DijkstraAlgorithm::makeDirected() {
    isDirected = true;
}

void DijkstraAlgorithm::clearUIState() {
    for (int i = 0; i < edgeWeight.size(); i++)
        if (!edgeDeleted[i]) callbackSetEdgeColor(i, Theme::textPrimary);
    callbackClearAnnotation();
}

void DijkstraAlgorithm::createNode() {
    adjency.push_back(std::vector<std::pair<int,int>>(0));
    nodeDeleted.push_back(false);
    callbackCreateNode(graphSize++);
    clearUIState();
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

void DijkstraAlgorithm::createEdge (int fromNode, int toNode) {
    adjency[fromNode].emplace_back(toNode, edgeWeight.size());
    if (!isDirected)
        adjency[toNode].emplace_back(fromNode, edgeWeight.size());
    edgeWeight.push_back(randInt(0, 99));
    edgeDeleted.push_back(false);
    callbackAddEdge(fromNode, toNode, edgeWeight.back());
    clearUIState();
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

void DijkstraAlgorithm::createEdge (int fromNode, int toNode, int weight) {
    adjency[fromNode].emplace_back(toNode, edgeWeight.size());
    if (!isDirected)
        adjency[toNode].emplace_back(fromNode, edgeWeight.size());
    edgeWeight.push_back(weight);
    edgeDeleted.push_back(false);
    callbackAddEdge(fromNode, toNode, edgeWeight.back());
    clearUIState();
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

void DijkstraAlgorithm::deleteNode (int targetNode) {
    nodeDeleted[targetNode] = true;
    for (int fromNode = 0; fromNode < graphSize; fromNode++) {
        for (auto [toNode, edgeID] : adjency[fromNode])
            if (fromNode == targetNode || toNode == targetNode)
                edgeDeleted[edgeID] = true;
    }
    callbackDeleteNode(targetNode);
    clearUIState();
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

void DijkstraAlgorithm::deleteEdge (int targetEdge) {
    edgeDeleted[targetEdge] = true;
    callbackDeleteEdge(targetEdge);
    clearUIState();
    callbackCompleteAnimation();
    callbackApplyAnimation();
}

bool DijkstraAlgorithm::editEdge (int targetEdge, int weight) {
    if (edgeWeight[targetEdge] == weight) return false;
    edgeWeight[targetEdge] = weight;
    callbackEditEdge(targetEdge, weight);
    clearUIState();
    callbackCompleteAnimation();
    callbackApplyAnimation();
    return true;
}

bool DijkstraAlgorithm::run (int source) {
    if (source < 0 || source >= nodeDeleted.size() || nodeDeleted[source]) return false;

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
    return true;
}

bool DijkstraAlgorithm::runPrim (int source) {
    if (source < 0 || source >= nodeDeleted.size() || nodeDeleted[source]) return false;

    // Logic intialization
    std::vector<bool> vist(graphSize, false);
    std::vector<int> nodeWeight(graphSize, INT_MAX);
    using Item = std::tuple<int,int,int>;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> pq;

    // UI intialization
    callbackLoadCode(CodeRepo::PRIM_CODE);
    callbackClearAnnotation();
    for (int i = 0; i < edgeWeight.size(); i++)
        if (!edgeDeleted[i]) callbackSetEdgeColor(i, Theme::textSecondary);
    for (int i = 0; i < graphSize; i++)
        if (!nodeDeleted[i]) callbackEditAnnotation(i, INT_MAX);
    callbackHighlightCode(0);
    callbackApplyAnimation();
    
    // Run Dijkstra algorithm
    pq.emplace(0, source, -1), nodeWeight[source] = 0;
    callbackEditAnnotation(source, 0);
    callbackHighlightCode(1);
    callbackApplyAnimation();

    while (pq.size()) {
        int w, curNode, edgeID; std::tie(w, curNode, edgeID) = pq.top(); pq.pop();
        if (vist[curNode]) continue;
        
        callbackHighlightCode(2);
        callbackApplyAnimation();
        
        callbackHighlightNode(curNode);
        callbackHighlightCode(3);
        callbackApplyAnimation();

        if (curNode != source) {
            callbackSetEdgeColor(edgeID, Theme::textPrimary);
            callbackHighlightCode(4);
            callbackApplyAnimation();
        }
        
        vist[curNode] = true;
        callbackMarkAnnotation(curNode);
        callbackHighlightCode(5);
        callbackApplyAnimation();

        for (auto [nextNode, edgeID] : adjency[curNode]) {
            if (edgeDeleted[edgeID]) continue;
            callbackHighlightEdge(edgeID);
            callbackHighlightCode(6);
            callbackApplyAnimation();

            int weight = edgeWeight[edgeID];
            callbackHighlightCode(7);
            if (weight < nodeWeight[nextNode]) {
                nodeWeight[nextNode] = weight;
                pq.emplace(nodeWeight[nextNode], nextNode, edgeID);
                callbackEditAnnotation(nextNode, nodeWeight[nextNode]);
            }
            callbackApplyAnimation();

            callbackHighlightEdge(-1);
            callbackApplyAnimation();
        }
    }
    
    callbackHighlightNode(-1);
    callbackClearAnnotation();
    callbackLoadCode({});
    callbackCompleteAnimation();
    callbackApplyAnimation();
    return true;
}
}; // namespace DS