/******************************************************************************
*
* FileName: Edge.cpp
*
* Author: Wenbo Guo
*
* Date: Nov 14 2014
*
* Description: Include the definition of class Edge.
*
* Function: Edge(void) is the constructor for class Edge.
*			~Edge(void) is the destructor for class Edge.
*			GetEdge() is to get an edge.
*			GetFlow() is to get the flow of an edge
*			SetFlow() is to set the flow of an edge
*
******************************************************************************/

#include "stdafx.h"
#include "Edge.h"

/*
* @brief Edge(void) is the constructor for class Edge.
*
*/
Edge::Edge(void)
{
}

/*
* @brief ~Edge(void) is the destructor for class Edge.
*
*/
Edge::~Edge(void)
{
}

/*
* @brief GetEdge() is to get an edge. 
*
* @param
*
* @return edgePair an edge.
*
*/
pair<int, int> Edge::GetEdge() const
{
	return this->edgePair;
}

/*
* @brief GetFlow() is to get the flow of an edge 
*
* @param
*
* @return flow the flow of an edge.
*
*/
bool Edge::GetFlow() const
{
	return this->flow;
}

/*
* @brief SetFlow() is to set the flow of an edge
*
* @param flow
*
* @return 0 shows this function has run successfully.
*
*/
int Edge::SetFlow(bool flow)
{
	this->flow = flow;
	return 0;
}
