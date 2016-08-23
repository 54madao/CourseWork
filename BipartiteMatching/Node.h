/******************************************************************************
*
* FileName: Node.h
*
* Author: Wenbo Guo
*
* Date: Nov 14 2014
*
* Description: A header for Node.cpp.
*			   Include the declaration of class Node.
*
* Function: Node(void) is the constructor for class Node.
*			Node(int, int) is the constructor for class Node.
*			~Node(void) is the destructor for class Node.
*			GetNodeIndex() is to get the index of a node.
*			GetNodeLevel() is to get the level of a node in the level graph
*			SetNodeIndex() is to set the index of a node.
*			SetNodeLevel() is to set the level of a node in the level graph
*			operator >> () is to overload operator ">>"
*			operator << () is to overload operator "<<"
*
******************************************************************************/

using namespace std;

#pragma once

/*
* @brief this class is for the object contains the information of a node
*
*/
class Node
{
public:

	/*
	* @brief Node(void) is the constructor for class Node.
	*
	*/
	Node(void);

	/*
	* @brief Node(int, int) is the constructor for class Node.
	*
	*/
	Node(int, int);

	/*
	* @brief ~Node(void) is the destructor for class Node.
	*
	*/
	virtual ~Node(void);

	/*
	* @brief GetNodeIndex() is to get the index of a node. 
	*
	* @param
	*
	* @return nodeIndex the index of the node.
	*
	*/
	int GetNodeIndex() const;

	/*
	* @brief GetNodeLevel() is to get the level of a node in the level graph
	*
	* @param
	*
	* @return nodeLevel the level of a node in the level graph.
	*
	*/
	int GetNodeLevel() const;

	/*
	* @brief SetNodeIndex() is to set the index of a node. 
	*
	* @param index
	*
	* @return 0 shows this function has run successfully.
	*
	*/
	int SetNodeIndex(int);

	/*
	* @brief SetNodeLevel() is to set the level of a node in the level graph
	*
	* @param level
	*
	* @return 0 shows this function has run successfully.
	*
	*/
	int SetNodeLevel(int);

	/*
	* @brief operator >> () is to overload operator ">>"
	*
	* @param ss stringstream
	* @param node a node
	*
	* @return ss stringsteam.
	*
	*/
	friend inline stringstream &operator >> (stringstream& ss, Node& node)
	{
		ss >> node.nodeName;
		return ss;
	}

	/* @brief operator << () is to overload operator "<<"
	*
	* @param o ostream
	* @param node a node
	*
	* @return o ostream.
	*
	*/
	friend inline ostream &operator << (ostream& o, Node& node)
	{
		o << node.nodeName;
		return o;
	}

private:
	string nodeName; // name of a node
	int nodeIndex; // index of a node
	int nodeLevel; // level of a node in level graph
};

