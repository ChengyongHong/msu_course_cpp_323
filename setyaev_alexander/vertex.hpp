#ifndef VERTEX
#define VERTEX

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  explicit Vertex(VertexId id, int level) : id_(id), level_(level) {}
  VertexId get_id() const;
  const std::vector<EdgeId>& get_connected_edge_ids() const;
  void add_connected_edge_id(EdgeId id);
  std::string to_string() const;
  int get_level() const;

 private:
  VertexId id_ = 0;
  int level_ = 0;
  std::vector<EdgeId> connected_edge_ids_;
};

#endif
