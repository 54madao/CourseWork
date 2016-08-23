/******************************************************************************
*
* FileName: Graph.h
*
* Author: Wenbo Guo
*
* Date: Nov 14 2014
*
* Description: A header for Graph.cpp.
*			   Include the declaration of class Graph.
*
* Function: Graph(void) is the constructor for class Graph.
*			Graph(vector<Node>&, vector<Edge>&) is the constructor for class Graph.
*			~Graph(void) is the destructor for class Graph.
*			CreatResidualGraph() is to creat a residual graph from Graph
*			CreatLevelGraph() is to creat a level graph from the residual graph
*           Retreat() is to retreat a node and edges enter this node in the level graph
*			Augment() is to augment flow with path
*			UpdateResidualGraph() is to update the residual graph with path
*			UpdateLevelGraph() is to update the level graph after finding a path
*			GetLevelGraph() is to get the level graph.
*			GetEdgeVec() is to get the edges in Graph.
*			operator << () is to overload operator "<<"
*
******************************************************************************/

#include "Node.h"
#include "Edge.h"

using namespace std;

#pragma once

/*
* @brief this class is for the object contains the information of a node
*
*/
class Graph
{
public:

	/*
	* @brief Graph(void) is the constructor for class Graph.
	*
	*/
	Graph(void);

	/*
	* @brief Graph(vector<Node>&, vector<Edge>&) is the constructor 
	*		 for class Graph.
	*
	*/
	Graph(vector<Node>&, vector<Edge>&);

	/*
	* @brief ~Graph(void) is the destructor for class Graph.
	*
	*/
	virtual ~Graph(void);

	/*
	* @brief CreatResidualGraph() is to creat a residual graph from Graph
	*
	* @param
	*
	* @return 0 shows this function has run successfully.
	*
	*/
	int CreatResidualGraph();

	/*
	* @brief CreatLevelGraph() is to creat a level graph from the 
	*		 residual graph
	*
	* @param
	*
	* @return foundSink shows if a path exists from source to sink
	*
	*/
	bool CreatLevelGraph();

	/*
	* @brief Retreat() is to retreat a node and delete edges enter this node 
	*		 in the level graph
	*
	* @param nodeIndex the index of the node
	*
	* @return levelGraph the level graph after retreating.
	*
	*/
	vector<vector<int> > Retreat(int);

	/*
	* @brief Augment() is to augment flow with path
	*
	* @param path the path from source to sink
	*
	* @return 0 shows this function has run successfully.
	*
	*/
	int Augment(vector<int> const&);

	/*
	* @brief UpdateResidualGraph() is to update the residual graph with path
	*
	* @param path the path from source to sink
	*
	* @return 0 shows this function has run successfully.
	*
	*/
	int UpdateResidualGraph(vector<int> const&);

	/*
	* @brief UpdateLevelGraph() is to update the level graph after 
	*		 finding a path
	*
	* @param path the path from source to sink
	*
	* @return levelGraph the level graph after updating
	*
	*/
	vector<vector<int> > UpdateLevelGraph(vector<int> const &);

	/*
	* @brief GetLevelGraph() is to get the level graph.
	*
	* @param
	*
	* @return levelGraph the level graph
	*
	*/
	vector<vector<int> > GetLevelGraph() const;

	/*
	* @brief GetEdgeVec() is to get the edges in Graph. 
	*
	* @param
	*
	* @return edgeVec the edges in Graph.
	*
	*/
	vector<Edge> GetEdgeVec() const;

	/*
	* @brief GetNode() is to get a node from Graph. 
	*
	* @param nodeIndex the index of a node
	*
	* @return nodeVec[nodeIndex] a node
	*
	*/
	Node GetNode(int) const;

	/* @brief operator << () is to overload operator "<<"
	*
	* @param o ostream
	* @param graph a graph
	*
	* @return o ostream.
	*
	*/
	friend inline ostream &operator << (ostream& o,Graph& graph)
	{
		for (vector<vector<int> >::iterator it = graph.residualGraph.begin();
			it != graph.residualGraph.end(); it++)
		{
			for (vector<int>::iterator it1 = it->begin(); 
				it1 != it->end(); it1++)
			{
				o << *it1 << " ";
			}
			o << endl;
		}
		o << endl;

		for (vector<vector<int> >::iterator it = graph.levelGraph.begin();
			it != graph.levelGraph.end(); it++)
		{
			for (vector<int>::iterator it1 = it->begin(); 
				it1 != it->end(); it1++)
			{
				o << *it1 << " ";
			}
			o << endl;
		}
		o << endl;
		return o;
	}

private:
	vector<Node> nodeVec; // nodes in Graph
	vector<Edge> edgeVec; //edges in Graph
	vector<vector<int> > residualGraph; // a residual graph
	vector<vector<int> > levelGraph;// a level graph
};

