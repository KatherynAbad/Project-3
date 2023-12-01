#pragma once
#include "graph.cpp"
#include <queue>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

// Reconstruct path from BFS
vector<int> reconstructPathBFS(const Graph& graph, int startActorId, int targetActorId, const unordered_map<int, int>& predecessor) {
    vector<int> path;
    auto it = predecessor.find(targetActorId);
    if (it == predecessor.end()) {
        return {};  // Path not found
    }
    for (int at = targetActorId; at != startActorId; at = predecessor.at(at)) {
        path.push_back(at);
        path.push_back(graph.findMovieId(at, predecessor.at(at)));
    }
    path.push_back(startActorId);
    reverse(path.begin(), path.end());
    return path;
}

vector<int> bfs(const Graph& graph, int startActorId, int targetActorId) {
    cout << "Running Breadth-First Search Algorithm..." << endl;

    unordered_map<int, int> predecessor;
    unordered_map<int, bool> visited;
    queue<int> q;

    q.push(startActorId);
    visited[startActorId] = true;

    while (!q.empty()) {
        int currentId = q.front();
        q.pop();

        
        if (currentId == targetActorId) { // If the target actor is found
            break;
        }

        for (const auto& edge : graph.getAdjList().at(currentId)) {
            if (!visited[edge.actor_id]) {
                visited[edge.actor_id] = true;
                predecessor[edge.actor_id] = currentId;
                q.push(edge.actor_id);
            }
        }
    }

    return reconstructPathBFS(graph, startActorId, targetActorId, predecessor);
}
