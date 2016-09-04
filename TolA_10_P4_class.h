//****************************************************************************
//       CLASS:  CSCE-2110.001
//  ASSIGNMENT:  Program 4
//     STUDENT:  Andrew Tolle
// DESCRIPTION:  This program creates a basic graph 
//				
//
//				 This file contains class definitions for node, edge, and
//				 collector classes
//
//
// REQUIRED SOURCE FILES:   TolA_10_P4_main.cpp   
//							TolA_10_P4_class.cpp							
//
//
//****************************************************************************
#include <iostream>		// header for cin cout
#include <iomanip>		// header for format output
#include <fstream>		// header for file i/o
#include <list>			// header for STL linked lists
#include <string>		// header for STL strings
#include <sstream>		// header for string streaming
#include <vector>		// header for STL vectors
#include <algorithm>	// header for converting to lowercase
#include <stdlib.h>

class Edge;			// forward declaration
class Collector;	// forward declaration

class Node 			
{
	private:
			std::string name;		// name of node
			int num_edges;			// number of edges
			std::list<Edge*> edges;	// list of edge pointers
			std::string root;			// used to determine cycles in Kruskal's Algorithm
			bool visited;
			int dist;

	public:
			Node(){}

			Node(std::string n)		// constructor, sets name and number of edges to 0
			{
				name = n;
				num_edges = 0;
				root = "";
				visited = false;
			}

			std::string get_name()						{return name;}			// node name
			std::list<Edge*> get_edges()				{return edges;}			// node's edges
			std::string get_root()						{return root;}			// node's root, used for Kruskal's		
			void set_root(std::string r)				{root = r;}				// root name
			void addEdge(Edge* edge)					{edges.push_back(edge);}// function prototypes
			void sortEdges();
			bool findEdge(Node* nodeA, Node* nodeB);
			std::list<Edge*>::iterator findEdgeIter(Node* nodeA, Node* nodeB);
			void set_isVisited()						{visited = true;}
			void set_isNotVisited()						{visited = false;}
			bool isVisited()							{return visited;}
			int get_dist()								{return dist;}
			void set_dist(int d)						{dist = d;}
};

class Edge
{
	private:
			Node* nodeA;		// pointer to node A
			Node* nodeB;		// pointer to node B
			int dist;		// length of node
			std::string name;		// name of node
			int num_directions;

	public:
			Edge(Node* A, Node* B, std::string n, int d)	// constructor for new edge
			{
				nodeA = A;
				nodeB = B;
				dist = d;
				name = n;
				num_directions = 2;
			}
			
			int get_dist()							{return dist;}		// setters and getters for private data
			void set_dist(int d)					{dist = d;}					
			std::string get_name()					{return name;}
			void set_name(std::string n)			{name = n;}
			Node* get_nodeA()						{return nodeA;}
			Node* get_nodeB()						{return nodeB;}
			std::string get_nodeAroot()				{return nodeA->get_root();}		// gets node names and node roots
			std::string get_nodeBroot()				{return nodeB->get_root();}
			std::string get_nodeAname()				{return nodeA->get_name();}
			std::string get_nodeBname()				{return nodeB->get_name();}
			int get_numdir()						{return num_directions;}
			void decrease_dir()						{num_directions--;}
};

class Collector 		// collection class that stores node and edges
{
	private:
			std::list<Node> node_list;	// list of nodes
			std::list<Edge> edge_list;	// list of edges

	public:		
			void addNode(std::string name);						// function prototypes
			void deleteNode(std::string name);
			void printNodes();
			Node* findNode(std::string name);
			void addEdge(Node *nodeA, Node *nodeB, std::string name, int dist);
			void deleteEdge(Node *nodeA, Node *nodeB);
			void printEdges();
			Edge* findEdge(Node* nodeA, Node* nodeB);
			void printMatrix();
			void printList();									// prints adjacency list
			void printList(std::list<Edge*> MStree);			// prints adjacency list for Kruskal's
			void Kruskals();
			void ShortestPath(Node* startNode, Node* targetNode);
			std::list<Edge>::iterator findActualEdgeIter(Node* nodeA, Node* nodeB);
			Node* findMinDistance();
};






