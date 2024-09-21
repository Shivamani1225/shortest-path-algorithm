#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <string>
#include <algorithm>

using namespace std;

class WeightedGraph {
private:
    int numVertices;
    vector<vector<pair<int, int>>> adjacencyList;

public:
    WeightedGraph(int vertices) : numVertices(vertices), adjacencyList(vertices) {}

    void connectNodes(int from, int to, int weight) {
        if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
            adjacencyList[from].emplace_back(to, weight);
            adjacencyList[to].emplace_back(from, weight);
        }
    }

    pair<int, vector<int>> findOptimalRoute(int start, int end) {
        if (start < 0 || start >= numVertices || end < 0 || end >= numVertices) {
            return {-1, {}};
        }

        vector<int> distances(numVertices, numeric_limits<int>::max());
        vector<int> previousNode(numVertices, -1);
        distances[start] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> nodeQueue;
        nodeQueue.push({0, start});

        while (!nodeQueue.empty()) {
            int currentNode = nodeQueue.top().second;
            int currentDist = nodeQueue.top().first;
            nodeQueue.pop();

            if (currentNode == end) break;

            if (currentDist > distances[currentNode]) continue;

            for (const auto& neighbor : adjacencyList[currentNode]) {
                int nextNode = neighbor.first;
                int edgeWeight = neighbor.second;
                int totalDist = currentDist + edgeWeight;

                if (totalDist < distances[nextNode]) {
                    distances[nextNode] = totalDist;
                    previousNode[nextNode] = currentNode;
                    nodeQueue.push({totalDist, nextNode});
                }
            }
        }

        if (distances[end] == numeric_limits<int>::max()) {
            return {-1, {}};
        }

        vector<int> path;
        for (int at = end; at != -1; at = previousNode[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        return {distances[end], path};
    }
};

int main() {
    int numNodes = 8;
    WeightedGraph cityNetwork(numNodes);

    cityNetwork.connectNodes(0, 1, 4);
    cityNetwork.connectNodes(0, 7, 8);
    cityNetwork.connectNodes(1, 2, 8);
    cityNetwork.connectNodes(1, 7, 11);
    cityNetwork.connectNodes(2, 3, 7);
    cityNetwork.connectNodes(2, 5, 4);
    cityNetwork.connectNodes(3, 4, 9);
    cityNetwork.connectNodes(3, 5, 14);
    cityNetwork.connectNodes(4, 5, 10);
    cityNetwork.connectNodes(5, 6, 2);
    cityNetwork.connectNodes(6, 7, 1);
    cityNetwork.connectNodes(7, 0, 8);

    int startCity, endCity;
    cout << "Enter the starting city (0-7): ";
    cin >> startCity;
    cout << "Enter the destination city (0-7): ";
    cin >> endCity;

    if (startCity < 0 || startCity >= numNodes || endCity < 0 || endCity >= numNodes) {
        cout << "Invalid city numbers. Please enter numbers between 0 and 7." << endl;
        return 1;
    }

    auto [shortestDistance, optimalPath] = cityNetwork.findOptimalRoute(startCity, endCity);

    if (shortestDistance == -1) {
        cout << "No route found between the specified cities." << endl;
    } else {
        cout << "Shortest distance: " << shortestDistance << endl;
        cout << "Optimal route: ";
        for (int city : optimalPath) {
            cout << city << " ";
        }
        cout << endl;
    }

    return 0;
}