#pragma once
#include <queue>
#include <vector>
#include <limits>
#include <unordered_map>
#include <algorithm>
#include "graph.cpp"
using namespace std;

struct QueueNode 
{
    int actor_id;
    double distance;

    QueueNode(int id, double dist) : actor_id(id), distance(dist) {}

    // Operator overload for convenience
    bool operator>(const QueueNode& other) const 
    {
        return distance > other.distance;
    }
};

vector<int> reconstructPath(const Graph& graph, int startActorId, int targetActorId, const unordered_map<int, int>&predecessor) 
{
    vector<int> path;
    auto it = predecessor.find(targetActorId);
    if (it == predecessor.end()) 
    {
        return {};
    }
    for (int at = targetActorId; at != startActorId; at = predecessor.at(at)) 
    {
        path.push_back(at);
        path.push_back(graph.findMovieId(at, predecessor.at(at)));
    }
    path.push_back(startActorId);
    reverse(path.begin(), path.end());
    return path;
}

vector<int> dijkstra(const Graph& graph, int startActorId, int targetActorId) {
    cout << "Running Dijkstra's Algorithm..." << endl;

    // Use double for infinite distances
    const double INF = numeric_limits<double>::infinity();

    // distance map to store the min distance to each node
    unordered_map<int, double> distances;
    unordered_map<int, int> predecessor;

    // priority queue to hold the nodes to be processed.
    priority_queue<QueueNode, vector<QueueNode>, greater<QueueNode>> pq;

    // initialize all dist's as infinity.
    for (const auto &pair : graph.getActorNames()) 
    {
        distances[pair.first] = INF;
    }

    // start with source node.
    distances[startActorId] = 0;
    pq.emplace(startActorId, 0);

    while (!pq.empty()) 
    {
        int currentId = pq.top().actor_id;
        double currentDist = pq.top().distance;
        pq.pop();

        // check if we have found the path.
        if (currentId == targetActorId) break;
        // skip if we already found a better path.
        if (currentDist > distances[currentId]) continue;

        // update distances to neighboring nodes
        for (const auto &edge : graph.getAdjList().at(currentId)) 
        {
            double newDist = currentDist + edge.movie->weight;
            if (newDist < distances[edge.actor_id]) 
            {
                distances[edge.actor_id] = newDist;
                predecessor[edge.actor_id] = currentId;
                pq.emplace(edge.actor_id, newDist);
            }
        }
    }

    return reconstructPath(graph, startActorId, targetActorId, predecessor);
}
