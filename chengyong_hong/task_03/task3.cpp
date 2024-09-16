#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

constexpr int VERTEX_COUNT = 14;
constexpr int EDGE_COUNT = 18;
constexpr int INVALID_ID = -1;

using namespace std;
using EdgeId = int;
using VertexId = int;

class Graph {
    public:
	 class Vertex
 	{
  	 public:
   	 	explicit Vertex(const VertexId& new_id) : id(new_id) {}	
	
   		 void add_edge_id(const EdgeId& Next_id)
	 	{
			for (const auto& edge_id_i : edge_ids_)
       			if (edge_id_i != Next_id)assert("Edge id already exist");
     	 		edge_ids_.push_back(Next_id);
   	 	}
    		const vector<EdgeId>& get_edge_ids() const { return edge_ids_; }
    		const VertexId id = -1;
    		vector<EdgeId> edge_ids_ = {};
	  };

 	class Edge
 	{
   	public:
    		Edge(const pair<VertexId, VertexId>& new_vertex, const EdgeId& edge_id) : vertex_ids(new_vertex), id(edge_id) {}
      	    	const pair<VertexId, VertexId> vertex_ids = {};
    		const EdgeId id = -1;
	  };

	 void add_edge(const VertexId& first, const VertexId& second)
	 {
   		 assert(has_vertex(first) && "Source vertex id doesn't exist");
   		 assert(has_vertex(second) && "Destination vertex id doesn't exist");
   		 assert(!has_edge(first, second) && "Such edge already exist");
   		 const auto& new_edge = edges_.emplace_back(pair<VertexId, VertexId>{first, second}, get_nextedge());
   		 vertices_[first].add_edge_id(new_edge.id);
   		 vertices_[second].add_edge_id(new_edge.id);
 	 }

	 void add_vertex() { vertices_.emplace_back(get_nextvertex()); }

  	bool has_vertex(const VertexId& id) const
 	{
    	 	for (const auto& vertex : vertices_)if (vertex.id == id)return true;
   		 return false;
 	 }

 	 bool has_edge(const VertexId& first, const VertexId& second) const
	 {
   		 for (const auto& edge_id_from_first_vertex : vertices_[first].get_edge_ids())
     		 for (const auto& edge_id_from_second_vertex : vertices_[second].get_edge_ids())
      	 	 if (edge_id_from_first_vertex == edge_id_from_second_vertex)return true;
   		 return false;
 	 }


/*	 string makeJSON() const
	 {
   		 stringstream ss;
   		 ss << "{\"vertices\":[";
   		 for (int i = 0; i < vertices_.size(); i++)
		 {
			 ss<< "{\"id\":" <<vertices_[i].id << ",\"edge_ids\":[";
      			 for (int j = 0; j < vertices_[i].edge_ids_.size() - 1; j++)ss <<vertices_[i].edge_ids_[j] << ",";
      			 ss <<vertices_[i].edge_ids_[vertices_[i].edge_ids_.size() - 1] << "]}";
     			 if (i != vertices_.size() - 1) ss << ",\n";
   		 }
   		 ss << "\n],\"edges\":[";
   		 for (int i = 0; i < edges_.size(); i++)
		 {
     			ss<< "{\"id\":" <<edges_[i].id << ",\"vertex_ids\":[" << edges_[i].vertex_ids.first<< "," << edges_[i].vertex_ids.second << "]}";
     			 if (i != edges_.size() - 1) ss << ",\n";
   		 }
   		 ss << "]}\n";
   		 return ss.str();
 	 }
*/	
 
 	vector<Edge> edges_ = {};
  	vector<Vertex> vertices_ = {};
private:
    VertexId nextvertex_id = 0;
  	EdgeId nextedge_id = 0;
  	EdgeId get_nextedge() { return nextedge_id++; }
  	VertexId get_nextvertex() { return nextvertex_id++; }
};

//_____________________________________________________________________________
class GraphPrinter
{
    public:
        explicit GraphPrinter(Graph& graph):graph_(graph){}
        stringstream ss;
        string print( )
        {
            print_vertex(graph_.vertices_);
            print_edge(graph_.edges_);
            return ss.str();
        }

         void print_vertex(const vector<Graph::Vertex>& vertices_)
         {
             int i=0;
             ss << "{\"vertices\":[";
             for (; i < vertices_.size(); i++){
                 ss<< "{\"id\":" <<vertices_[i].id << ",\"edge_ids\":[";
                 for (int j = 0; j < vertices_[i].edge_ids_.size() - 1; j++)ss <<vertices_[i].edge_ids_[j] << ",";
                 ss <<vertices_[i].edge_ids_[vertices_[i].edge_ids_.size() - 1] << "]}";
                 if (i != vertices_.size() - 1) ss << ",\n";
             }
        }
         void print_edge(const vector<Graph::Edge>& edges_)
         {
             int i=0;
            ss << "\n],\"edges\":[";
            for (; i < edges_.size(); i++)
            {
                ss<< "{\"id\":" <<edges_[i].id << ",\"vertex_ids\":[" << edges_[i].vertex_ids.first<< "," << edges_[i].vertex_ids.second << "]}";
                    if (i != edges_.size() - 1) ss << ",\n";
            }
           ss << "]}\n";

         }
    private:
        Graph& graph_;
        
};


int main()
{
 	 Graph graph{};
	  pair<VertexId, VertexId> G [EDGE_COUNT] = { {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6}, {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10}, {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13} };
	  for (int i = 0; i < VERTEX_COUNT; i++)
	  {
  		  graph.add_vertex(); 
 	 }
	  for (const auto& iterator : G)
	  {
   		 graph.add_edge(iterator.first, iterator.second);
	  }
	  ofstream file;

      auto graph_printer=GraphPrinter(graph);
      file.open("graph.json", std::fstream::out | std::fstream::trunc);
	  file << graph_printer.print();  
	  file.close();
	  return 0;
}
