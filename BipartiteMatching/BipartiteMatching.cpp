/******************************************************************************
*
* FileName: BipartiteMatching.cpp
*
* Author: Wenbo Guo
*
* Date: Nov 14 2014
*
* UpDate: Nov 19 2014
*
* Description: To get the maximum number of bipartite matching based on a given
*			   data file, and show each pair of matching.
*
* Function: _tmain()
*
******************************************************************************/

#include "stdafx.h"
#include "Graph.h"
#include "Node.h"
#include "Edge.h"

using namespace std;

/*
* @brief _tmain() function is the entrance of the program. It is to get the 
*		 maximum number of bipartite matching. And show each pair of matching.
*
* @param
*
* @return 0 shows the program has run successfully.
*
*/
int _tmain(int argc, _TCHAR* argv[])
{
	int line = 0; // count the line of data read from a file.
	int nodeNum; // number of nodes.
	int edgeNum; // nober of edges.
	int count = 0; // count the matching number.
	char buf[1024]; // buffer for data read from a file
	stringstream ss;
	Node node;
	Edge edge;
	vector<Node> nodeVec; // save the data of nodes
	vector<Edge> edgeVec; // save the data of edges
	ifstream fin("program3data.txt");

	//read data from file into Graph G.
	if (fin.is_open()) // open the file successfully
	{
		while (fin.good() && !fin.eof())
		{
			//read data
			memset(buf, 0, 1024);
			fin.getline(buf, 1024);
			ss << buf;
			line++;

			if (line == 1)
			{
				ss >> nodeNum; // save total number of nodes
			}
			else if (line <= nodeNum + 1)
			{
				ss >> node;
				node.SetNodeIndex(line - 1);
				node.SetNodeLevel(0);
				nodeVec.push_back(node);
			}
			else if (line == nodeNum + 2)
			{
				ss >> edgeNum; // save total number of edges
			}
			else if (line <= nodeNum + edgeNum + 2)
			{
				ss >> edge;
				edge.SetFlow(false); // set the flow to 0;
				edgeVec.push_back(edge);
			}
			else // no more valid data
			{
				break; 
			}

			//reset ss
			ss.clear();
			ss.str("");
		} // while
		fin.close();
	} //if

	//create residual graph Gf from G
	Graph G(nodeVec, edgeVec);
	cout << "Creat a residual graph" << endl;
	G.CreatResidualGraph();

	cout << "Begin matching..." << endl;
	//while there is at least one path from source to sink in residual graph
	while (true)
	{
		//construct level graph LG from Gf using breadth-first search.
		//if no path exists from source to sink , output matching.
		if (!G.CreatLevelGraph())
		{
			cout << "Bipartie Matching: " << endl;

			vector<Edge> BiMatching = G.GetEdgeVec();
			for (vector<Edge>::iterator it = BiMatching.begin(); 
				it != BiMatching.end(); it++)
			{
				Edge match = *it;
				if (match.GetFlow()) // there is a flow on this edge
				{
					count++;
					cout << G.GetNode(match.GetEdge().first) << "/" << G.GetNode(match.GetEdge().second) << endl;
				}
			}
			cout << "Maximum number of matching: " << count <<endl;
			break;
		}//if

		cout << "Creat a new level graph" << endl;

		//initialize location to source node, path to empty.
		int location = 0;
		vector<int> path;
		vector<vector<int> > levelGraph = G.GetLevelGraph();

		//while not stuck at source
		while (!levelGraph[0].empty())
		{		
			//if location is sink
			if (location == nodeNum + 1)
			{
				path.push_back(location); // add node to path
				G.Augment(path); //augment flow with path
				G.UpdateResidualGraph(path); //update Gf
				levelGraph = G.UpdateLevelGraph(path); //delete edges in path from LG
				location = 0; //set location to source
				path.clear(); //clear path.
			}
			//if stuck, retreat
			else if (levelGraph[location].empty())
			{
				levelGraph = G.Retreat(location); //delete current location and incoming edges from LG
				location = path.back(); //reset location to the last node
				path.pop_back(); //delete last edge from path
			}
			//else, advance along some edge in LG that leaves current location.
			else
			{
				vector<int>::iterator it = levelGraph[location].begin();
				path.push_back(location); //Update current path
				location = *it; // update location
			}//if
		}//while
	}//while

	system("pause");
	return 0;
}

