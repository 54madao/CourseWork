/******************************************************************************
*
* FileName: Edge.h
*
* Author: Wenbo Guo
*
* Date: Nov 14 2014
*
* Description: A header for Edge.cpp.
*			   Include the declaration of class Edge.
*
* Function: Edge(void) is the constructor for class Edge.
*			~Edge(void) is the destructor for class Edge.
*			GetEdge() is to get an edge.
*			GetFlow() is to get the flow of an edge
*			SetFlow() is to set the flow of an edge
*			operator >> () is to overload operator ">>"
*			operator << () is to overload operator "<<"
*
******************************************************************************/

using namespace std;

#pragma once

/*
* @brief this class is for the object contains the information of an edge
*
*/
class Edge
{
public:

	/*
	* @brief Edge(void) is the constructor for class Edge.
	*
	*/
	Edge(void);

	/*
	* @brief ~Edge(void) is the destructor for class Edge.
	*
	*/
	virtual ~Edge(void);

	/*
	* @brief GetEdge() is to get an edge. 
	*
	* @param
	*
	* @return edgePair an edge.
	*
	*/
	pair<int, int> GetEdge() const;

	/*
	* @brief GetFlow() is to get the flow of an edge 
	*
	* @param
	*
	* @return flow the flow of an edge.
	*
	*/
	bool GetFlow() const;

	/*
	* @brief SetFlow() is to set the flow of an edge
	*
	* @param flow
	*
	* @return 0 shows this function has run successfully.
	*
	*/
	int SetFlow(bool);

	/*
	* @brief operator >> () is to overload operator ">>"
	*
	* @param ss stringstream
	* @param edge an edge
	*
	* @return ss stringsteam.
	*
	*/
	friend inline stringstream &operator >> (stringstream& ss, Edge& edge)
	{
		int outIndex;
		int inIndex;
		ss >> outIndex; 
		ss >> inIndex;
		edge.edgePair = make_pair(outIndex, inIndex);
		return ss;
	}

	/* @brief operator << () is to overload operator "<<"
	*
	* @param o ostream
	* @param edge an edge
	*
	* @return o ostream.
	*
	*/
	friend inline ostream &operator << (ostream& o, Edge& edge)
	{
		o << edge.edgePair.first << " " << edge.edgePair.second << " " << edge.flow << endl;
		return o;
	}

private:
	pair<int, int> edgePair; // an edge
	bool flow; // the flow of an edge
};

