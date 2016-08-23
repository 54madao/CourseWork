/******************************************************************************
*
* FileName: Graph.cpp
*
* Author: Wenbo Guo
*
* Date: Nov 14 2014
*
* Description: Include the definition of class Graph.
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
*
******************************************************************************/

#include "stdafx.h"
#include "Graph.h"
#include "queue"

/*
* @brief Graph(void) is the constructor for class Graph.
*
*/
Graph::Graph(void)
{
}

/*
* @brief Graph(vector<Node>&, vector<Edge>&) is the constructor 
*		 for class Graph.
*
*/
Graph::Graph(vector<Node>& nodeVec, vector<Edge>& edgeVec)
{
	this->nodeVec = nodeVec;
	this->edgeVec = edgeVec;
}

/*
* @brief ~Graph(void) is the destructor for class Graph.
*
*/
Graph::~Graph(void)
{
}

/*
* @brief CreatResidualGraph() is to creat a residual graph from Graph
*
* @param
*
* @return 0 shows this function has run successfully.
*
*/
int Graph:: CreatResidualGraph()
{
	int nodeNum = this->nodeVec.size();
	int leftNum = nodeNum / 2;
	this->residualGraph.resize(nodeNum + 2);

	//creat source and add edges
	for (int i = 1; i <= leftNum; i++)
	{
		this->residualGraph[0].push_back(i);
	}

	//creat residual graph
	vector<Edge>:: iterator edgeIt;
	for(edgeIt = this->edgeVec.begin(); edgeIt != this->edgeVec.end(); edgeIt++)
	{
		pair<int, int> p = (*edgeIt).GetEdge();
		this->residualGraph[p.first].push_back(p.second);
	}

	//creat sink and add edges
	for (int i = leftNum + 1; i <= nodeNum; i++)
	{
		this->residualGraph[i].push_back(nodeNum + 1);
	}
	residualGraph[nodeNum + 1].push_back(-1);

	//add source node and sink node
	Node nodes(0, 0);
	Node nodet(nodeNum + 1, 0);
	nodeVec.insert(nodeVec.begin(), nodes);
	nodeVec.push_back(nodet);
	return 0;
}

/*
* @brief CreatLevelGraph() is to creat a level graph from the 
*		 residual graph
*
* @param
*
* @return foundSink shows if a path exists from source to sink
*
*/
bool Graph:: CreatLevelGraph()
{
	//check residualGraph
	if (residualGraph.empty())
	{
		cout << "There is no residual graph" << endl;
		return false;
	}

	bool foundSink =false;
	this->levelGraph = this->residualGraph;

	// reset all nodes to having not been visited
	int nodeNum = this->nodeVec.size();
	vector<bool> nodeVisited;
	for (int i = 0; i < nodeNum; i++)
	{
		nodeVisited.push_back(false);
	}

	queue<int> nodeIndex;
	nodeIndex.push(0);

	//if there is a node in the queue
	while(!nodeIndex.empty())
	{
		int nodeV = nodeIndex.front();
		nodeIndex.pop();
		int currentLevel = nodeVec[nodeV].GetNodeLevel();

		//breadth-first search
		for (vector<int>::iterator it = this->levelGraph[nodeV].begin(); 
			it != this->levelGraph[nodeV].end();)
		{
			int nodeW = *it;
			if (nodeW == -1) // found sink
			{
				foundSink = true;
				break;
			}
			// if the node hasn't been visited
			else if (!nodeVisited[nodeW])
			{
				nodeVisited[nodeW] = true;
				nodeVec[nodeW].SetNodeLevel(currentLevel + 1); // update the node level
				nodeIndex.push(nodeW);
				it++;
			}
			//if the node has been visited  and has a lower level, delete this edge
			else if (nodeVec[nodeW].GetNodeLevel() < currentLevel + 1)
			{
				it = this->levelGraph[nodeV].erase(it);
			}
			else
				it++;
		}//for
		nodeVisited[nodeV] = true;
	}//while
	return foundSink;
}

/*
* @brief Retreat() is to retreat a node and delete edges enter this node 
*		 in the level graph
*
* @param nodeIndex the index of the node
*
* @return levelGraph the level graph after retreating.
*
*/
vector<vector<int> > Graph::Retreat(int nodeIndex)
{
	int level = nodeVec[nodeIndex].GetNodeLevel();
	for (vector<Node>::iterator it = nodeVec.begin(); it != nodeVec.end(); it++)
	{
		//find the node with level one less than the level of retreated node
		if ((*it).GetNodeLevel() == level - 1)
		{
			int nodeV = (*it).GetNodeIndex();
			//if there is an edge from the node to the retreated node, delete it
			vector<int>::iterator it1 = find(levelGraph[nodeV].begin(), 
				levelGraph[nodeV].end(), nodeIndex);
			if (it1 != levelGraph[nodeV].end())
			{
				levelGraph[nodeV].erase(it1);
			}
		}
	}
	return levelGraph;
}

/*
* @brief Augment() is to augment flow with path
*
* @param path the path from source to sink
*
* @return 0 shows this function has run successfully.
*
*/
int Graph::Augment(vector<int> const &path)
{
	for (vector<int>::const_iterator it = path.begin(); it != path.end() - 1; it++)
	{
		pair<int, int> nodePair = make_pair(*it, *(it + 1));
		pair<int, int> reverseNodePair = make_pair(*(it + 1), *it);
		for (vector<Edge>::iterator it1 = this->edgeVec.begin(); 
			it1 != this->edgeVec.end(); it1++)
		{
			//if the edge belongs to original edge
			if ((*it1).GetEdge() == nodePair)
			{
				(*it1).SetFlow(true);
			}
			//if the edge belongs to reverse edge
			else if ((*it1).GetEdge() == reverseNodePair)
			{
				(*it1).SetFlow(false);
			}
		}
	}
	return 0;	
}

/*
* @brief UpdateResidualGraph() is to update the residual graph with path
*
* @param path the path from source to sink
*
* @return 0 shows this function has run successfully.
*
*/
int Graph::UpdateResidualGraph(vector<int> const &path)
{
	for (vector<int>::const_iterator it = path.begin(); it != path.end() - 1; it++)
	{
		int nodeV = *it;
		int nodeW = *(it + 1);

		//reverse the edge from (v,w) to (w,v)
		vector<int>::iterator it1 = find(residualGraph[nodeV].begin(), 
			residualGraph[nodeV].end(), nodeW);
		if (it1 != residualGraph[nodeV].end())
		{
			residualGraph[nodeV].erase(it1);
		}
		residualGraph[nodeW].push_back(nodeV);
	}
	return 0;
}

/*
* @brief UpdateLevelGraph() is to update the level graph after 
*		 finding a path
*
* @param path the path from source to sink
*
* @return levelGraph the level graph after updating
*
*/
vector<vector<int> > Graph::UpdateLevelGraph(vector<int> const &path)
{
	//delete the path from source to sink
	for (vector<int>::const_iterator it = path.begin(); 
		it != path.end() - 1; it++)
	{
		int nodeV = *it;
		int nodeW = *(it + 1);
		vector<int>::iterator it1 = find(levelGraph[nodeV].begin(), 
			levelGraph[nodeV].end(), nodeW);
		if (it1 != levelGraph[nodeV].end())
		{
			levelGraph[nodeV].erase(it1);
		}
	}
	return levelGraph;
}

/*
* @brief GetLevelGraph() is to get the level graph.
*
* @param
*
* @return levelGraph the level graph
*
*/
vector<vector<int> > Graph::GetLevelGraph() const
{
	return this->levelGraph;
}

/*
* @brief GetEdgeVec() is to get the edges in Graph. 
*
* @param
*
* @return edgeVec the edges in Graph.
*
*/
vector<Edge> Graph::GetEdgeVec() const
{
	return this->edgeVec;
}

/*
* @brief GetNode() is to get a node from Graph. 
*
* @param nodeIndex the index of a node
*
* @return nodeVec[nodeIndex] a node
*
*/
Node Graph::GetNode(int nodeIndex) const
{
	return this->nodeVec[nodeIndex];
}