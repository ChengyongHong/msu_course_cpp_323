#pragma once

#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "graph.hpp"

namespace graph_generation {

double get_color_probability(const Edge::Color& color) {
  switch (color) {
    case Edge::Color::Green:
      return 0.1;
      break;
    case Edge::Color::Blue:
      return 0.25;
      break;
    case Edge::Color::Red:
      return 0.33;
      break;
    default:
      assert(false && "Unexpected behavior");
      return 0.0;
      break;
  }
}

bool is_lucky(const double probability) {
  const double eps = std::numeric_limits<double>::epsilon();
  assert(probability + eps > 0 && probability - eps < 1 &&
         "given probability is incorrect");
  static std::knuth_b rand_engine{};
  std::mt19937 rng{rand_engine()};
  std::bernoulli_distribution bernoullu_distribution_var(probability);
  return bernoullu_distribution_var(rng);
}

int get_random_number(const int size) {
  std::random_device
      rd;  // Will be used to obtain a seed for the random number engine
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<int> distrib(0, size - 1);
  return distrib(gen);
}

void generate_green_edges(Graph& graph) {
  const double probability = get_color_probability(Edge::Color::Green);
  for (Depth current_depth = 0; current_depth < graph.get_depth();
       ++current_depth) {
    for (const auto& current_vertex_id :
         graph.get_vertices_at_depth(current_depth)) {
      if (is_lucky(probability)) {
        graph.add_edge(current_vertex_id, current_vertex_id,
                       Edge::Color::Green);
      }
    }
  }
}

void generate_blue_edges(Graph& graph) {
  const double probability = get_color_probability(Edge::Color::Blue);
  for (Depth current_depth = 1;
       current_depth <=
       graph.get_depth();  // так как на нулевом уровне только одна вершина ==
                           // нулевая, нет смысла ее учитывать
       ++current_depth) {
    const auto& vertices_at_depth = graph.get_vertices_at_depth(current_depth);
    for (int idx = 0; idx < vertices_at_depth.size() - 1; ++idx) {
      if ((idx < vertices_at_depth.size()) && is_lucky(probability)) {
        graph.add_edge(vertices_at_depth[idx], vertices_at_depth[idx + 1],
                       Edge::Color::Blue);
      }
    }
  }
}

void generate_yellow_edges(Graph& graph) {
  double yellow_edge_probability = 1.0 / (graph.get_depth() - 1);
  for (Depth current_depth = 1;
       current_depth <
       graph.get_depth();  //так как вероятность генерации желтых ребер из
                           //нулевой вершины должна быть нулевой, то можно
                           //просто не рассматривать эту вершину
       ++current_depth) {
    const auto& vertices_at_depth = graph.get_vertices_at_depth(current_depth);
    const auto& vertices_at_next_depth =
        graph.get_vertices_at_depth(current_depth + 1);
    for (const auto& current_vertex_id : vertices_at_depth) {
      if (is_lucky(yellow_edge_probability)) {
        std::vector<VertexId> not_binded_vertices;
        for (const auto& next_vertex_id : vertices_at_next_depth) {
          if (!graph.check_binding(current_vertex_id, next_vertex_id)) {
            not_binded_vertices.push_back(next_vertex_id);
          }
        }
        if (not_binded_vertices.size()) {
          int idx = get_random_number(not_binded_vertices.size());
          graph.add_edge(current_vertex_id, not_binded_vertices[idx],
                         Edge::Color::Yellow);
        }
      }
    }
    yellow_edge_probability += 1.0 / (graph.get_depth() - 1);
  }
}

void generate_red_edges(Graph& graph) {
  const double probability = get_color_probability(Edge::Color::Red);
  for (Depth current_depth = 0; current_depth < graph.get_depth() - 1;
       ++current_depth) {
    const auto& vertices_at_depth = graph.get_vertices_at_depth(current_depth);
    const auto& vertices_at_next_depth =
        graph.get_vertices_at_depth(current_depth + 2);
    for (const auto& current_vertex_id : vertices_at_depth) {
      if (is_lucky(probability)) {
        const int index = get_random_number(vertices_at_next_depth.size());
        graph.add_edge(current_vertex_id, vertices_at_next_depth[index],
                       Edge::Color::Red);
      }
    }
  }
}

void generate_gray_edges(Graph& graph,
                         const Depth& depth,
                         const int new_vertices_num) {
  double new_vertext_probability = 1.0;

  for (Depth current_depth = 0;
       current_depth < depth && current_depth <= graph.get_depth();
       ++current_depth) {  //по всем уровням вершин
    for (VertexId parent_vertex_id : graph.get_vertices_at_depth(
             current_depth)) {  //по всем порождающим вершинам
      for (int i = 0; i < new_vertices_num; ++i) {
        if (is_lucky(new_vertext_probability)) {
          const VertexId& new_vertex_id =
              graph.add_vertex();  //добавляю новую вершину в граф
          graph.add_edge(parent_vertex_id,
                         new_vertex_id);  //соединяю вершину ребенка и вершину
                                          //родителя(чтобы малыш не потерялся в
                                          //этом страшном мире)
        }
      }
    }
    new_vertext_probability -= 1.0 / depth;
  }
}

Graph generate_graph(const Depth& depth, int new_vertices_num) {
  auto graph = Graph();
  graph.add_vertex();
  generate_gray_edges(graph, depth, new_vertices_num);
  generate_green_edges(graph);
  generate_blue_edges(graph);
  generate_yellow_edges(graph);
  generate_red_edges(graph);
  return graph;
}
}  // namespace graph_generation
