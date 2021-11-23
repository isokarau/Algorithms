#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <set>
 
class Dijkstra {
public:
    Dijkstra(int vertices, int edges)
            : number_of_vertices(vertices)
            , number_of_edges(edges)
            , graph(vertices)
    {}
 
    void AddEdge(int first_vertex, int second_vertex, int edge_length) {
        --first_vertex;
        --second_vertex;
        graph[first_vertex].push_back({second_vertex, edge_length});
        graph[second_vertex].push_back({first_vertex, edge_length});
    }
 
    void Solve(int starting_point) {
        distance.assign(number_of_vertices, std::numeric_limits<long long>::max());
        parent.resize(number_of_vertices);
        conditions.clear();
 
        --starting_point;
        distance[starting_point] = 0;
        conditions.insert({distance[starting_point], starting_point});
 
        while (!conditions.empty()) {
            auto [minimal_distance, current_vertex] = *conditions.begin();
            conditions.erase(conditions.begin());
            for (const auto [to, edge_length] : graph[current_vertex]) {
                if (minimal_distance + edge_length < distance[to]) {
                    conditions.erase({distance[to], to});
                    distance[to] = minimal_distance + edge_length;
                    conditions.insert({distance[to], to});
                    parent[to] = current_vertex;
                }
            }
        }
    }
 
    long long GetMinDistance(int finishing_point) const {
        --finishing_point;
        return (distance[finishing_point] == std::numeric_limits<long long>::max()
            ? -1 // if there is no way to reach this vertex, then the answer is -1 for example
            : distance[finishing_point]); // else the answer is counted
    }
 
    // restore the path with minimal length from starting point to finishing point
    std::vector<int> RestorePath(int finishing_point) const {
        --finishing_point;
        if (distance[finishing_point] == std::numeric_limits<long long>::max()) {
            return {-1};
        }
 
        std::vector<int> path;
        path.push_back(finishing_point + 1);
        while (finishing_point != 0) {
            finishing_point = parent[finishing_point];
            path.push_back(finishing_point + 1);
        }
 
        reverse(path.begin(), path.end());
        return path;
    }
 
private:
    int number_of_vertices;
    int number_of_edges;
    struct Edge {
        int to;
        int length;
    };
 
    using Edges = std::vector<Edge>;
 
    std::vector<Edges> graph;
    std::vector<long long> distance;
    std::vector<int> parent;
    std::set<std::pair<long long, int>> conditions;
};
 
int main() {
    int number_of_vertices;
    int number_of_edges;
 
    std::cin >> number_of_vertices >> number_of_edges;
 
    Dijkstra d(number_of_vertices, number_of_edges);
    for (int i = 0; i < number_of_edges; ++i) {
        int first_vertex;
        int second_vertex;
        int edge_length;
 
        std::cin >> first_vertex >> second_vertex >> edge_length;
 
        d.AddEdge(first_vertex, second_vertex, edge_length);
    }
 
    int starting_point;
    int finishing_point;
 
    std::cin >> starting_point >> finishing_point;
 
    d.Solve(starting_point);
 
    std::cout << d.GetMinDistance(finishing_point) << "\n"; // minimal distance from starting point to finishing point
    for (const auto vertex : d.RestorePath(finishing_point)) { // full path from starting point to finishing point
        std::cout << vertex << ' ';
    }
 
    return 0;
}
