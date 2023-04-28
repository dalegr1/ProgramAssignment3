#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <limits>

using namespace std;

const int MAX_TIME = 2400;
const int INF = numeric_limits<int>::max();

// Graph representation using adjacency list
unordered_map<string, unordered_map<string, int>> graph;

// Custom comparator for priority queue
class CustomCompare {
public:
    bool operator()(const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    }
};

// Dijkstra's algorithm to find shortest path
int dijkstra(string source, string destination, int startTime) {
    // Initialize distance map
    unordered_map<string, int> dist;
    for (const auto& [city, neighbors] : graph) {
        dist[city] = INF;
    }
    dist[source] = startTime;

    // Initialize priority queue
    priority_queue<pair<string, int>, vector<pair<string, int>>, CustomCompare> pq;
    pq.push({ source, startTime });

    // Main loop
    while (!pq.empty()) {
        auto [u, uDist] = pq.top();
        pq.pop();

        // Found shortest path to destination
        if (u == destination) {
            return uDist;
        }

        // Check all neighbors of u
        for (const auto& [v, weight] : graph[u]) {
            int vDist = uDist + weight;

            // Check if path through u is shorter than current shortest path to v
            if (vDist < dist[v] && vDist <= MAX_TIME) {
                dist[v] = vDist;
                pq.push({ v, vDist });
            }
        }
    }

    // Destination is unreachable
    return -1;
}

int main() {
    // Read input
    string source, destination;
    int startTime;
    cin >> source >> destination >> startTime;

    // Read graph from file
    ifstream inputFile("flights.txt");
    string line;
    while (getline(inputFile, line)) {
        string city1, city2;
        int departureTime, travelTime;
        sscanf(line.c_str(), "%s %s %d %d", &city1[0], &city2[0], &departureTime, &travelTime);
        if (graph.find(city1) == graph.end()) {
            graph[city1] = {};
        }
        graph[city1][city2] = departureTime + travelTime;
    }
    inputFile.close();

    // Find earliest arrival time
    int earliestArrivalTime = dijkstra(source, destination, startTime);

    // Output result
    cout << earliestArrivalTime << endl;

    return 0;
}
