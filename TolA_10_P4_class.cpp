//****************************************************************************
//       CLASS:  CSCE-2110.001
//  ASSIGNMENT:  Program 4
//     STUDENT:  Andrew Tolle
// DESCRIPTION:  This program creates a basic graph 
//				
//
//				 This file contains class methods for node, edge, and
//				 collector classes
//
//
// REQUIRED SOURCE FILES:   TolA_10_P4_main.cpp   
//							TolA_10_P4_classes.h							
//
//
//****************************************************************************
#include "TolA_10_P4_class.h"

bool UNDIRECTED = true;	// flag for turning off kruskal's algorithm in the case of edge deletion

struct CompareNodeNames								// overload for sorting algorithm
{													// used to sort nodes by name
	bool operator()(Node &a, Node &b)
	{
		return (a.get_name() < b.get_name());
	}
};

struct CompareNodeDist
{													// used to sort nodes by name
	bool operator()(Node &a, Node &b)
	{
		return (a.get_dist() < b.get_dist());
	}
};

struct CompareEdges									// overload for sorting algorithm
{		
	std::string nodeName;	
	CompareEdges(std::string name)		{nodeName = name;}	// used to sort each node's list of edge pointers depending on their order (A-B or B-A)
	bool operator()(Edge *a, Edge *b)								// i.e. A-C D-A A-B would sort A-B A-C A-D for node A
	{
		if(nodeName == a->get_nodeAname() && nodeName == b->get_nodeAname())			// if comparison for node A is A-C A-B 
			return (a->get_nodeBname() < b->get_nodeBname());	
		else if(nodeName == a->get_nodeBname() && nodeName == b->get_nodeBname())		// if comparison for node A is C-A B-A
			return (a->get_nodeAname() < b->get_nodeAname());	
		else if(nodeName == a->get_nodeAname() && nodeName == b->get_nodeBname())		// if comparison for node A is A-C B-A
			return (a->get_nodeBname() < b->get_nodeAname());	
		else 		// if comparison for node A is C-A A-B
			return (a->get_nodeAname() < b->get_nodeBname());
	}
};

struct KruskalsSort
{
	bool operator()(Edge *a, Edge *b)						// sorts edges by distance for Kruskals
	{
		return (a->get_dist() < b->get_dist());
	}
};

void Node::sortEdges()								// sorts node's list of edge pointers
{
	edges.sort(CompareEdges(get_name()));		// passes node name to be used in sorting
}

Node* Collector::findNode(std::string name)			// find a node given the name of the node
{
	std::list<Node>::iterator it;
	for (it = node_list.begin(); it != node_list.end(); ++it)		// runs through node list, returns pointer to node if found	
	{																	// if not found, returns NULL
		if(name == it->get_name())
			return &(*it);
	}

	return NULL;	
}

Edge* Collector::findEdge(Node *nodeA, Node *nodeB)		// find edge given pointers to node A and node B
{
	std::list<Edge>::iterator it;	// iterator to run through list of edges in Collector
	for (it = edge_list.begin(); it != edge_list.end(); ++it)		// runs through list of edges
	{
		if((nodeA == it->get_nodeA() && nodeB == it->get_nodeB()) || (nodeB == it->get_nodeA() && nodeA == it->get_nodeB())) 
			return &(*it);	// returns edge if node A and node B are A-B or B-A in the list of edges
	}

	return NULL;
}

bool Node::findEdge(Node* nodeA, Node* nodeB)
{
	std::list<Edge*>::iterator it;
	for(it = edges.begin(); it != edges.end(); ++it)
	{
		if((nodeA == (*it)->get_nodeA() && nodeB == (*it)->get_nodeB()) || (nodeB == (*it)->get_nodeA() && nodeA == (*it)->get_nodeB())) 
			return true;	// returns edge if node A and node B are A-B or B-A in the list of edges
	}

	return false;
}

std::list<Edge*>::iterator Node::findEdgeIter(Node* nodeA, Node* nodeB)
{
	std::list<Edge*>::iterator it;
	for(it = edges.begin(); it != edges.end(); ++it)
	{
		if((nodeA == (*it)->get_nodeA() && nodeB == (*it)->get_nodeB()) || (nodeB == (*it)->get_nodeA() && nodeA == (*it)->get_nodeB())) 
			return it;	// returns edge if node A and node B are A-B or B-A in the list of edges
	}

	return it;	// if edge is not found
}

std::list<Edge>::iterator Collector::findActualEdgeIter(Node* nodeA, Node* nodeB)
{
	std::list<Edge>::iterator it;
	for(it = edge_list.begin(); it != edge_list.end(); ++it)
	{
		if((nodeA == it->get_nodeA() && nodeB == it->get_nodeB()) || (nodeB == it->get_nodeA() && nodeA == it->get_nodeB())) 
			return it;	// returns edge if node A and node B are A-B or B-A in the list of edges
	}

	return it;	// if edge is not found
}

void Collector::addNode(std::string name)		// add a node to the list in the Collector
{
	if(findNode(name) != NULL)					// if the node is not found, an error is displayed
	{
		std::cout << std::endl <<"*** ERROR *** DUPLICATE NODE: " << name;
	}		
	else	// node is found
	{
		Node newnode(name);				// create new node object
		node_list.push_back(newnode);	// puts node at end of node list
		std::cout << std::endl << "ADDED: NODE " << name;;

		node_list.sort(CompareNodeNames());	// resorts list of nodes
	}
}

void Collector::addEdge(Node *nodeA, Node *nodeB, std::string name, int dist)	// add edge given target nodes A and B
{
	Edge* this_edge = findEdge(nodeA,nodeB);

	if(this_edge != NULL)				// if edges already exist
	{
		this_edge->set_dist(dist);		// updates values for the existing edge
		this_edge->set_name(name);
		std::cout << std::endl << "UPDATED: EDGE " << nodeA->get_name() << "-" << nodeB->get_name() << " NAME: " << this_edge->get_name() << " DIST: " << this_edge->get_dist();

	}
	else	// if edge not already in list
	{
		Edge edge(nodeA,nodeB,name,dist);		// creates new edge A-B
		edge_list.push_back(edge);	// add edge A-B to edge list in Collector

		std::cout << std::endl << "ADDED: EDGE " << nodeA->get_name() << "-" << nodeB->get_name() << " NAME: " << edge.get_name() << " DIST: " << edge.get_dist();
	
		nodeA->addEdge(&(edge_list.back())); 	// adds pointer to edge A-B to edge list for node A
		nodeA->sortEdges();	// sorts edge pointers list for node A

		if(nodeA != nodeB)	// if it's not adding an edge A-A
		{
			std::cout << std::endl << "ADDED: EDGE " << nodeB->get_name() << "-" << nodeA->get_name() << " NAME: " << edge.get_name() << " DIST: " << edge.get_dist();
		
			nodeB->addEdge(&(edge_list.back()));	// add pointer to edge B-A to edge list for node B
			nodeB->sortEdges();	// sorts edge pointers list for node B
		}
	}
}

void Collector::deleteNode(std::string name)	// delete node from list in Collector by name
{
	std::list<Edge*> node_edges;		// list to be used to navigate each node's list of edge pointers
	std::list<Node>::iterator it;		// iterator for node list
	std::list<Edge>::iterator eit;		// iterator for node's list of edge pointers
	std::list<Edge*>::iterator targetEdge;	// edge to be deleted
	std::list<Edge>::iterator targetActualEdge;
	Node* this_node;
	Node* nodeA;
	Node* nodeB;
	Edge* this_edge;

	this_node = findNode(name);

	if(this_node == NULL)
		std::cout << std::endl <<"*** ERROR *** NODE NOT FOUND FOR DELETION: " << name;

	for(eit = edge_list.begin(); eit != edge_list.end();)
	{
		if(eit->get_nodeA()->get_name() == name || eit->get_nodeB()->get_name() == name)
		{
			nodeA = eit->get_nodeA();
			nodeB = eit->get_nodeB();
		
			this_edge = findEdge(nodeA,nodeB);

			if(nodeA == this_node)				// to delete edge in other node
			{
				if(nodeB->findEdge(this_node,nodeB))	// if the node has the edge in its list of edge pointers
				{
					UNDIRECTED = false;
					targetEdge = nodeB->findEdgeIter(this_node,nodeB);

					if(this_node->get_name() == (*targetEdge)->get_nodeA()->get_name())
						std::cout << std::endl << "EDGE AUTO-REMOVED BY NODE DELETION: " << (*targetEdge)->get_nodeB()->get_name() << "-" << this_node->get_name();
					else
						std::cout << std::endl << "EDGE AUTO-REMOVED BY NODE DELETION: " << (*targetEdge)->get_nodeA()->get_name() << "-" << this_node->get_name();
					
					nodeB->get_edges().erase(targetEdge);	// removes edge from node's list of edge pointers
					this_edge->decrease_dir();				// decreases directions for edge by 1
				}
			}

			else if(nodeB == this_node)			// to delete edge in other node
			{
				if(nodeA->findEdge(this_node,nodeA))	// if the node has the edge in its list of edge pointers
				{
					UNDIRECTED = false;
					targetEdge = nodeA->findEdgeIter(this_node,nodeA);

					if(this_node->get_name() == (*targetEdge)->get_nodeA()->get_name())
						std::cout << std::endl << "EDGE AUTO-REMOVED BY NODE DELETION: " << (*targetEdge)->get_nodeB()->get_name() << "-" << this_node->get_name();
					else
						std::cout << std::endl << "EDGE AUTO-REMOVED BY NODE DELETION: " << (*targetEdge)->get_nodeA()->get_name() << "-" << this_node->get_name();

					nodeA->get_edges().erase(targetEdge);	// removes edge from node's list of edge pointers
					this_edge->decrease_dir();				// decreases directions for edge by 1
				}
			}

			if(this_node->findEdge(nodeA,nodeB))	// if the node has the edge in its list of edge pointers
			{
				UNDIRECTED = false;
				targetEdge = this_node->findEdgeIter(nodeA,nodeB);

				if(this_node->get_name() == (*targetEdge)->get_nodeA()->get_name())
					std::cout << std::endl << "EDGE AUTO-REMOVED BY NODE DELETION: " << this_node->get_name() << "-" << (*targetEdge)->get_nodeB()->get_name();
				else
					std::cout << std::endl << "EDGE AUTO-REMOVED BY NODE DELETION: " << this_node->get_name() << "-" << (*targetEdge)->get_nodeA()->get_name();
				
				this_node->get_edges().erase(targetEdge);		// removes edge from node's list of edge pointers
				this_edge->decrease_dir();						// decreases directions for edge by 1

			}						

			if(this_edge->get_numdir() == 0)		// if number of directions for edge is 0, then the edge goes nowhere and is deleted
				edge_list.erase(eit++);				// deletes edge and iterates to the next one in the list	
		}
		else
			++eit;		// moves to next edge
	}

	for(it = node_list.begin(); it != node_list.end(); ++it)
	{
		if(name == it->get_name())
		{
			node_list.erase(it);	// deletes target node
			std::cout << std::endl << "DELETED: NODE " << name;	

			return;
		}
	}

}

void Collector::deleteEdge(Node* nodeA, Node* nodeB)	// delete edge given node A and B
{
	Edge* this_edge;
	std::list<Edge*>::iterator targetEdge;	// edge to be deleted
	std::list<Edge>::iterator targetActualEdge;

	targetEdge = nodeA->findEdgeIter(nodeA,nodeB);		// finds iterator for edge
	this_edge = findEdge(nodeA,nodeB);

	if(nodeA->findEdge(nodeA,nodeB))		// if the node has the edge
	{
		UNDIRECTED = false;
		nodeA->get_edges().erase(targetEdge);	// delete edge from node's list of edge pointers
		this_edge->decrease_dir();				// decreases edge's directions by 1

		std::cout << std::endl << "DELETED: EDGE " << nodeA->get_name() << "-" << nodeB->get_name();

		if(this_edge->get_numdir() == 0)
		{
			targetActualEdge = this->findActualEdgeIter(nodeA,nodeB);	// if the edge's directions are 0, edge goes nowhere and is deleted

			edge_list.erase(targetActualEdge);
		}
	}
	else
		std::cout << std::endl <<"*** ERROR *** EDGE DOES NOT EXIST FOR DELETION: " << nodeA->get_name() << "-" << nodeB->get_name();
}

void Collector::printNodes()	// function to print all nodes, not called for in program instruction but used for debugging
{
	int cnt; 

	if(node_list.empty())		// if no nodes are in the system
	{
		std::cout << std::endl << "No nodes in system.";
		return;
	}
	else						// if nodes are in system
	{
		cnt = 1;
		std::list<Node>::iterator it;	// iterator for node list
		for (it = node_list.begin(); it != node_list.end(); ++it)		// runs through node list and prints each node name
		{
			std::cout << std::endl << std::endl << "Node " << cnt << ": " << it->get_name();
			cnt++;
		}
	}
}

void Collector::printEdges()	// function to print all edges, not called for in program instruction but used for debugging
{	
	int cnt; 

	if(edge_list.empty())		// if no edges are in the system
	{
		std::cout << std::endl << "No edges in system.";
		return;
	}
	else						// if edges are in system
	{
		cnt = 1;
		std::list<Edge>::iterator it;	// iterator for edge list
		for (it = edge_list.begin(); it != edge_list.end(); ++it)		// runs through node list and prints each edge name
		{
			std::cout << std::endl << std::endl << "Edge " << cnt << ": " << it->get_nodeAname() << " " << it->get_nodeBname();
			cnt++;
		}
	}
}

void Collector::printMatrix()		// prints adjacency matrix
{
	std::list<Node>::iterator it;			// iterator for node list
	std::list<Edge*>::iterator eit;		// iterator for edge pointers list for each node
	std::vector<std::string> nodeColumns;	// vector to store node name for columns in matrix
	std::list<Edge*> node_edges;			// list of edge pointers to store each node's list of edges for itation
	int node_edgecnt = 0;					// count of node's edges

	std::cout << std::endl << std::endl << "   ";	// formatting space

	for(it = node_list.begin(); it != node_list.end(); ++it)
	{
		std::cout << "   " << it->get_name();		// this runs through the list of nodes and prints each one, then pushes them
		nodeColumns.push_back(it->get_name());	// into the vector
	}

	std::cout << std::endl << "   ";		// formatting line

	for(int i = 0; i < (int) node_list.size(); i++)	// formatting line beneath each column
		std::cout << "-----";

	std::cout << std::endl;				// formatting line

	for(it = node_list.begin(); it != node_list.end(); ++it)	// runs through list of nodes
	{
		node_edges = it->get_edges();			// stores node's list of edges

		eit = node_edges.begin();				// sets iterator to beginning of node's list of edges

		node_edgecnt = node_edges.size();

		std::cout << it->get_name() << "|";	// prints node name and formatting line on other side of node name i.e. A| 

		for(int j = 0; j < (int) nodeColumns.size(); j++) // runs through vector of node names 
		{
			if(node_edges.empty())		// if node has no edges, prints 0's for each column
			{
				std::cout << "   -1";
			}
			else	// if node has edges
			{
				if(node_edgecnt > 0)
				{	
					if(it->get_name() == (*eit)->get_nodeAname() && (*eit)->get_nodeBname() == nodeColumns[j])		// if node B of edge matches the column, i.e. A-B matches column B
					{
						std::cout << std::setw(5) << (*eit)->get_dist(); //"    1";		// prints 1 and increments eit to next edge
						++eit;
						node_edgecnt--;
					}
					else if(it->get_name() == (*eit)->get_nodeBname() && (*eit)->get_nodeAname() == nodeColumns[j])		// if node B of edge matches the column, i.e. A-B matches column B
					{
						std::cout << std::setw(5) << (*eit)->get_dist();// "    1";		// prints 1 and increments eit to next edge
						++eit;
						node_edgecnt--;
					}
					else		// if node B of edge does not match the column, 0 is printed
						std::cout << "   -1";
				}
				else		// if node B of edge does not match the column, 0 is printed
						std::cout << "   -1";
			}	
		}

		std::cout << std::endl << std::endl;
	}
}

void Collector::printList()		// prints adjacency list
{
	std::list<Node>::iterator it;		// iterator for node list
	std::list<Edge*>::iterator eit;	// iterator for edge pointer list for each node
	std::list<Edge*> node_edges;		// list of edge pointers to store node's edges

	std::cout << std::endl << std::endl;	

	for(it = node_list.begin(); it != node_list.end(); ++it)	// runs through list of nodes
	{
		node_edges = it->get_edges();	// copies node's edges to node_edges

		std::cout << it->get_name() << ":	";	// prints node name

		for(eit = node_edges.begin(); eit != node_edges.end(); ++eit)	// runs through node edges and prints node B of each one 
		{
			if(it->get_name() == (*eit)->get_nodeAname())
				std::cout << (*eit)->get_nodeBname() << "(" << (*eit)->get_name() << "," << (*eit)->get_dist() << ")	";
			else if(it->get_name() == (*eit)->get_nodeBname())
				std::cout << (*eit)->get_nodeAname() << "(" << (*eit)->get_name() << "," << (*eit)->get_dist() << ")	";
		}	

		std::cout << std::endl;
	}
}

void Collector::printList(std::list<Edge*> MStree)		// prints adjacency list for Kruskal's given minimal spanning tree
{
	std::list<Node>::iterator it;		// iterator for node list
	std::list<Edge*>::iterator eit;	// iterator for edge pointer list for each node
	std::list<Edge*> node_edges;		// list of edge pointers to store node's edges

	std::cout << std::endl << std::endl;	

	for(it = node_list.begin(); it != node_list.end(); ++it)	// runs through list of nodes
	{
	//	node_edges = it->get_edges();	// copies node's edges to node_edges

		MStree.sort(CompareEdges(it->get_name()));

		std::cout << it->get_name() << ":	";	// prints node name

		for(eit = MStree.begin(); eit != MStree.end(); ++eit)	// runs through node edges and prints node B of each one 
		{
			if(it->get_name() == (*eit)->get_nodeAname())
				std::cout << (*eit)->get_nodeBname() << "(" << (*eit)->get_name() << "," << (*eit)->get_dist() << ")	";
			else if(it->get_name() == (*eit)->get_nodeBname())
				std::cout << (*eit)->get_nodeAname() << "(" << (*eit)->get_name() << "," << (*eit)->get_dist() << ")	";
		}	

		std::cout << std::endl;
	}
}

void Collector::Kruskals()
{
	std::list<Edge*> MStree;			// list for minimal spanning tree
	std::list<Edge*> allEdges;			// list of edge pointers to sort edges by distance
	std::list<Edge>::iterator it;		// iterator for edge list
	std::list<Edge*>::iterator eit;		// iterators for edge pointer list
	std::list<Edge*>::iterator eit2;	
	std::string bigRoot;				// bigRoot is the larger sub-tree compared to smallRoot	
	std::string smallRoot;
	int rootAcnt = 0;					// counters for roots to determine cycles
	int rootBcnt = 0;	

	if(!UNDIRECTED)
	{
		std::cout << std::endl << "Kruskal's Algorithm is not available for undirected graphs";
		return;
	}

	for(it = edge_list.begin(); it != edge_list.end(); ++it) {allEdges.push_back(&(*it));}	// fills list with all edges

	allEdges.sort(KruskalsSort());	// sorts edges by weight

	for(eit = allEdges.begin(); eit != allEdges.end(); ++eit)
	{
		if((*eit)->get_nodeAroot() == "" && (*eit)->get_nodeBroot() == "")	// if neither node has a root then neither is in tree
		{
			(*eit)->get_nodeA()->set_root((*eit)->get_nodeA()->get_name());					// sets both nodes to the same root (A)
			(*eit)->get_nodeB()->set_root((*eit)->get_nodeA()->get_name());
			MStree.push_back((*eit));
//			std::cout << std::endl << "ADDED TO MSTREE: " << (*eit)->get_nodeAname() << "-" << (*eit)->get_nodeBname() << " ROOTS: " << (*eit)->get_nodeAroot() << (*eit)->get_nodeBroot();
		}
		else if((*eit)->get_nodeAroot() == "" && (*eit)->get_nodeBroot() != "")	// if node A doesn't have a root but B does
		{
			(*eit)->get_nodeA()->set_root((*eit)->get_nodeBroot());					// sets node A to B's root
			MStree.push_back((*eit));
//			std::cout << std::endl << "ADDED TO MSTREE: " << (*eit)->get_nodeAname() << "-" << (*eit)->get_nodeBname() << " ROOTS: " << (*eit)->get_nodeBroot() << (*eit)->get_nodeAroot();
		}
		else if((*eit)->get_nodeAroot() != "" && (*eit)->get_nodeBroot() == "")	// if node B doesn't have a root but A does
		{
			(*eit)->get_nodeB()->set_root((*eit)->get_nodeAroot());					// sets node B to A's root
			MStree.push_back((*eit));
//			std::cout << std::endl << "ADDED TO MSTREE: " << (*eit)->get_nodeAname() << "-" << (*eit)->get_nodeBname() << " ROOTS: " << (*eit)->get_nodeAroot() << (*eit)->get_nodeBroot();
		}
		else if((*eit)->get_nodeAroot() != (*eit)->get_nodeBroot())					// if node A and B have different roots
		{
			for(eit2 = allEdges.begin(); eit2 != eit; ++eit2)
			{
				if((*eit2)->get_nodeAroot() == (*eit)->get_nodeAroot() && (*eit2)->get_nodeBroot() == (*eit)->get_nodeAroot())		// counts number of roots
					rootAcnt++;																										// corresponding to the roots
				else if((*eit2)->get_nodeAroot() == (*eit)->get_nodeBroot() && (*eit2)->get_nodeBroot() == (*eit)->get_nodeBroot())	// of the eit edge
					rootBcnt++;
			}

			if(rootAcnt >= rootBcnt)
			{	
				bigRoot = (*eit)->get_nodeAroot();			// if there are more root A's in the tree, then all root B's will be changed to root A's (bigRoot)
				smallRoot = (*eit)->get_nodeBroot();
			}
			else
			{
				bigRoot = (*eit)->get_nodeBroot();			// if there are more root B's in the tree, then all root A's will be changed to root B's (bigRoot)
				smallRoot = (*eit)->get_nodeAroot();
			}
			for(eit2 = allEdges.begin(); eit2 != eit; ++eit2)			// goes through all nodes so far in the tree
			{
				if((*eit2)->get_nodeAroot() == bigRoot && (*eit2)->get_nodeBroot() != bigRoot)			// sets all edge's with rootA-rootB to rootA-rootA
					(*eit2)->get_nodeB()->set_root(bigRoot);
				else if((*eit2)->get_nodeBroot() == bigRoot && (*eit2)->get_nodeAroot() != bigRoot)
					(*eit2)->get_nodeA()->set_root(bigRoot);
				else if((*eit2)->get_nodeAroot() == smallRoot || (*eit2)->get_nodeAroot() == smallRoot)
				{
					(*eit2)->get_nodeA()->set_root(bigRoot);
					(*eit2)->get_nodeB()->set_root(bigRoot);
				}
			}
			MStree.push_back((*eit));		// edge is added to MST
//			std::cout << std::endl << "ADDED TO MSTREE: " << (*eit)->get_nodeAname() << "-" << (*eit)->get_nodeBname() << " ROOTS: " << (*eit)->get_nodeAroot() << (*eit)->get_nodeAroot();
	
			rootAcnt = 0;		// counts are reset
			rootBcnt = 0;
		}
		else if((*eit)->get_nodeAroot() == (*eit)->get_nodeBroot())					// if both nodes have the same root then they 
		{																			// are already in the tree and are not added	
//			std::cout << std::endl << "NOT ADDED TO MSTREE: " << (*eit)->get_nodeAname() << "-" << (*eit)->get_nodeBname();
		}		
	}

	printList(MStree);		// calls print list function sending minimal spanning tree
}

Node* Collector::findMinDistance()	// finds node with minimum distance in list of nodes and returns a pointer to it
{
	std::list<Node>::iterator it;
	std::list<Node>::iterator node;

	int min = 999999999;		// minimum distance starts at 999999999, finds nodes with less than this

	for(it = node_list.begin(); it != node_list.end(); ++it)
	{	
		if(!(it->isVisited()) && it->get_dist() <= min)
		{	// if the node has a distance less than 999999999, min is set to this distance and node pointer is set to the node in the list
			min = it->get_dist();
			node = it;
		}
	}

	return &(*node);
}

void Collector::ShortestPath(Node* startNode, Node* targetNode)
{
	std::list<Node>::iterator it;
	std::list<Node*>::iterator nit;
	std::list<Edge*>::iterator eit;		
	std::list<Edge*> node_edges;
	std::list<Node*> spNodes;
	bool targetFound = false;
	Node* this_node;
	Node* prev_node;

	// this algorithm was modified from http://www.geeksforgeeks.org/greedy-algorithms-set-6-dijkstras-shortest-path-algorithm/

	// setup for Dijkstra's algorithm - sets all node's distances to infinity except the start node and all their visited flags to false
	for(it = node_list.begin(); it != node_list.end(); ++it)
	{
		if(it->get_name() == startNode->get_name())
			it->set_dist(0);
		else
			it->set_dist(999999999);

		it->set_isNotVisited();
	}

	// runs through list of nodes and find shortest path to every node from start node
	for (int i = 0; i < (int) node_list.size(); i++)
	{
		// find node with smallest distance in the graph
		this_node = findMinDistance();
		// sets the node to visited
		this_node->set_isVisited();
		// if the node has edges then it run through each edge
		if(this_node->get_edges().size() > 0)
		{
			node_edges = this_node->get_edges();
			// runs through node's edges
			for (eit = node_edges.begin(); eit != node_edges.end(); ++eit)
			{
				if((*eit)->get_nodeA() == this_node)
				{	// if the node on the other end of the edge is unvisited and its distance is smaller than the previous node + the edge's weight
					if(!((*eit)->get_nodeB()->isVisited()) && (this_node->get_dist() + (*eit)->get_dist()) < (*eit)->get_nodeB()->get_dist())
					{	// the node's distance is adjusted to the previous node's distance + the edge's weight
						(*eit)->get_nodeB()->set_dist(this_node->get_dist() + (*eit)->get_dist());
					}
				}
				else if((*eit)->get_nodeB() == this_node)
				{	// if the node on the other end of the edge is unvisited and its distance is smaller than the previous node + the edge's weight
					if(!((*eit)->get_nodeA()->isVisited()) && (this_node->get_dist() + (*eit)->get_dist()) < (*eit)->get_nodeA()->get_dist())
					{	// the node's distance is adjusted to the previous node's distance + the edge's weight
						(*eit)->get_nodeA()->set_dist(this_node->get_dist() + (*eit)->get_dist());
					}
				}
				// once it reaches the target node, the flag is set to true
				if((*eit)->get_nodeA() == targetNode || (*eit)->get_nodeB() == targetNode)
					targetFound = true;
			}
		}
	}

	// if the target node was never reached
	if(!targetFound)
		std::cout << std::endl << "*** ERROR *** Path does not exist from " << startNode->get_name() << " to " << targetNode->get_name();

	// this works backwards from the target node back to the start node and puts the results in spNodes list
	// each node's distance, starting with the target node, is compared to its list of edges to find which edge assigned it that distance
	// the comparison is as follows:
	//
	// if this node's distance == the edge weight + the other node's distance, then we know that is the node for the shortest path
	else 
	{
		this_node = targetNode;

		while(this_node != startNode)
		{
			node_edges = this_node->get_edges();

			for (eit = node_edges.begin(); eit != node_edges.end(); ++eit)
			{	// determines which node is this_node and which is the other node of the edge
				if((*eit)->get_nodeA() == this_node)
				{
					if((*eit)->get_nodeA()->get_dist() == (*eit)->get_nodeB()->get_dist() + (*eit)->get_dist())
					{	// adds node to the spNodes list
						spNodes.push_front((*eit)->get_nodeB());
						this_node = (*eit)->get_nodeB();
					}
				}	
				else if((*eit)->get_nodeB() == this_node)
				{
					if((*eit)->get_nodeB()->get_dist() == (*eit)->get_nodeA()->get_dist() + (*eit)->get_dist())
					{	// adds node to the spNodes list
						spNodes.push_front((*eit)->get_nodeA());
						this_node = (*eit)->get_nodeA();
					}
				}	
			}	
		}

		// this prints the path, starting with the start node ending with the node before the target node
		for(nit = spNodes.begin(); nit != spNodes.end(); ++nit)
		{
			// the if-else statements are due to the expected output of the program (start node and target node are not supposed to print the paths associated
			// with them)
			if((*nit)->get_name() == startNode->get_name())
				std::cout << std::endl << (*nit)->get_name();
			else
			{
				node_edges = (*nit)->get_edges();
				for(eit = node_edges.begin(); eit != node_edges.end(); ++eit)
				{
					if((*eit)->get_nodeA() == prev_node || (*eit)->get_nodeB() == prev_node)
						std::cout << std::endl << (*nit)->get_name() << " " << (*eit)->get_name() << " " << (*eit)->get_dist();
				}
			}
			// prev_node is used to find the edge of the shortest path by comparing the current node to the one previous in the list
			prev_node = (*nit);
		}
		// target node is printed
		std::cout << std::endl << targetNode->get_name() << " (TOTAL: " << targetNode->get_dist() << ")";
	}
}
