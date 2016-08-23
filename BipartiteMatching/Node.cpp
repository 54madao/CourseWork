/******************************************************************************
*
* FileName: Node.cpp
*
* Author: Wenbo Guo
*
* Date: Nov 14 2014
*
* Description: Include the definition of class Node.
*
* Function: Node(void) is the constructor for class Node.
*			Node(int, int) is the constructor for class Node.
*			~Node(void) is the destructor for class Node.
*			GetNodeIndex() is to get the index of a node.
*			GetNodeLevel() is to get the level of a node in the level graph
*			SetNodeIndex() is to set the index of a node.
*			SetNodeLevel() is to set the level of a node in the level graph
*
******************************************************************************/

#include "stdafx.h"
#include "Node.h"

/*
* @brief Node(void) is the constructor for class Node.
*
*/
Node::Node(void)
{
}

/*
* @brief Node(int, int) is the constructor for class Node.
*
*/
Node::Node(int nodeIndex, int nodeLevel)
{
	this->nodeIndex = nodeIndex;
	this->nodeLevel = nodeLevel;
}

/*
* @brief ~Node(void) is the destructor for class Node.
*
*/
Node::~Node(void)
{
}

/*
* @brief GetNodeIndex() is to get the index of a node. 
*
* @param
*
* @return nodeIndex the index of the node.
*
*/
int Node::GetNodeIndex() const
{
	return this->nodeIndex;
}

/*
* @brief GetNodeLevel() is to get the level of a node in the level graph
*
* @param
*
* @return nodeLevel the level of a node in the level graph.
*
*/
int Node::GetNodeLevel() const
{
	return this->nodeLevel;
}

/*
* @brief SetNodeIndex() is to set the index of a node. 
*
* @param index
*
* @return 0 shows this function has run successfully.
*
*/
int Node::SetNodeIndex(int index)
{
	this->nodeIndex = index;
	return 0;
}

/*
* @brief SetNodeLevel() is to set the level of a node in the level graph
*
* @param level
*
* @return 0 shows this function has run successfully.
*
*/
int Node::SetNodeLevel(int level)
{
	this->nodeLevel = level;
	return 0;
}