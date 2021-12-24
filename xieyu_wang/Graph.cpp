#include "Graph.hpp"
#include <cassert>
#include <stdexcept>
namespace uni_course_cpp {
const Vertex& Graph::addVertex() {
  return vertices_.emplace_back(getNextVertexId());
  ;
}
void Graph::addEdge(int fromVertexId, int toVertexId, const Edge::Color color) {
  assert(hasVertex(fromVertexId) && "Vertex doesn't exist");
  assert(hasVertex(toVertexId) && "Vertex doesn't exist");
  assert(!isConnected(fromVertexId, toVertexId) &&
         "Vertices already connected");
  const auto& newEdge =
      edges_.emplace_back(getNextEdgeId(), fromVertexId, toVertexId, color);
  coloredEdges_[color].push_back(newEdge.id);
  auto& fromVertex = getVertex(fromVertexId);
  auto& toVertex = getVertex(toVertexId);
  if (color != Edge::Color::Green) {
    fromVertex.addEdgeId(newEdge.id);
  }
  toVertex.addEdgeId(newEdge.id);
  if (color == Edge::Color::Gray) {
    toVertex.depth = fromVertex.depth + 1;
  }
}
bool Graph::hasVertex(int idFind) const {
  for (const auto& vertex : vertices_) {
    if (vertex.id == idFind) {
      return true;
    }
  }
  return false;
}
bool Graph::isConnected(int fromVertexId, int toVertexId) const {
  assert(hasVertex(fromVertexId) && "Vertex doesn't exist");
  assert(hasVertex(toVertexId) && "Vertex doesn't exist");
  if (fromVertexId ==
      toVertexId) {  // if from vertex id== to vertex id ===>green
    for (const auto& edgeId : getVertex(fromVertexId).getEdgeIds()) {
      const auto& edge = getEdge(edgeId);
      if (edge.fromVertexId == edge.toVertexId) {
        return true;
      }
    }
  } else {  // from vertex id!= to vertex id ==> not green check here
    for (const auto& fromVertexEdgeId : getVertex(fromVertexId).getEdgeIds()) {
      for (const auto& toVertexEdgeId : getVertex(toVertexId).getEdgeIds()) {
        if (fromVertexEdgeId == toVertexEdgeId) {
          return true;
        }
      }
    }
  }
  return false;
}

const Vertex& Graph::getVertex(int id) const {
  assert(hasVertex(id) && "Vertex doesn't exist");
  for (auto& vertex : vertices_) {
    if (vertex.id == id) {
      return vertex;
    }
  }
  throw std::runtime_error("Unreachable code");
}

std::vector<int> Graph::getVertexIdsAtDepth(int depth) {
  std::vector<int> vertices;
  for (const auto& vertex : vertices_) {
    if (vertex.depth == depth) {
      vertices.push_back(vertex.id);
    }
  }
  return vertices;
}

const std::vector<Vertex>& Graph::getVertices() const {
  return vertices_;
}

const Edge& Graph::getEdge(int id) const {
  for (const auto& edge : edges_) {
    if (edge.id == id) {
      return edge;
    }
  }
  throw std::runtime_error("No such vertex");
}

const std::vector<int>& Graph::getEdgesByColor(const Edge::Color& color) const {
  if (coloredEdges_.find(color) == coloredEdges_.end()) {
    static std::vector<int> empty_result = {};
    return empty_result;
  }
  return coloredEdges_.at(color);
}

const std::vector<Edge>& Graph::getEdges() const {
  return edges_;
}

Vertex& Graph::getVertex(int id) {
  const auto& constThis = *this;
  return const_cast<Vertex&>(constThis.getVertex(id));
}

int Graph::getMaxDepth() {
  int depth = 0;
  while (true) {
    std::vector<int> vertexIds = getVertexIdsAtDepth(depth);
    if (vertexIds.size() == 0) {
      break;
    } else {
      depth++;
    }
  }
  return depth - 1;
}

}  // namespace uni_course_cpp
