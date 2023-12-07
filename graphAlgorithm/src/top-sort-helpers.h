#pragma once

#include <unordered_map>

#include "weighted-graph.hpp"
#include "graph-types.h"

template <typename T>
void computeIndegrees(const WeightedGraph<T>& graph, std::unordered_map<value_type<T>, int>& indegrees) {
    // TODO store the indegree for each vertex in the graph in the indegrees map
    for(const auto& vertex_pair : graph){
        const auto& vertex = vertex_pair.first;
        indegrees[vertex] = 0;
    }

    for(const auto& vertex_pair : graph){
        const auto& edges = vertex_pair.second;
        for(const auto& edge : edges){
            const auto& neighbor = edge.first;
            indegrees[neighbor]++;
        }
    }
}
