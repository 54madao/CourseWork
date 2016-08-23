/******************************************************************************
*
* FileName : ClosestPairofPoints.cpp
*
* Author : Wenbo Guo
*
* Date : Oct 18 2014
*
* UpDate : Oct 23 2014
*
* Description : To get the distance between the closest pair of points based on 
*               a given data file, and show the output in the order of recursive
*               calls.
*
* Function : _tmain()
* 
******************************************************************************/

#include "stdafx.h"
#include "fstream"
#include "sstream"
#include "iostream"
#include "algorithm"
#include "functional"
#include "string"
#include "vector"
#include "Point.h"
#include "GetClosestPairofPoints.h"

using namespace std;

/*
* @brief _tmain() function is the entrance of the program. It is to the distance 
*        between the closest pair of points by calling GetClosestPairofPoints() 
*        function. And show the distance in every recursive call.
*
* @param
*
* @return 0 shows the program has run successfully.
*
*/
int _tmain(int argc, _TCHAR* argv[])
{
	int line = 0; // count the line of data read from a file.
	int num; // number of points
	float x, y; // x-coordinate, y-coordinate
	char buf[1024]; // buff for data read from a file
	string msg;
	stringstream ss;
	vector<Point> vecP; // save the data of points read from a file
	vector<Point> sortedVecX; // a copy of vecP sorted by x-coordinate
	vector<Point> sortedVecY; // a copy of vecP sorted by y-coordinate
	ifstream fin("program2data.txt");
	
	if (fin.is_open()) // open the file successfully
	{
		while ( fin.good() && !fin.eof())
		{
			// read data.
			memset(buf, 0, 1024);
			fin.getline(buf, 1024);
			msg = buf;
			line++;

			ss << msg;
			if (line == 1)
			{
				ss >> num; // save total number of points.
			}
			else if (line == num + 2)
			{
				break; // already read all data
			}
			else
			{
				ss >> x; // save x-coordinate
				ss >> y; // save y-coordinate
				Point p(x, y); // create a Point
				vecP.push_back(p);
			}

			ss.clear();
			ss.str("");
		} //while
		fin.close();
	} //if

	sort(vecP.begin(), vecP.end(), less<Point>()); // sort vecP by x-coordinate
	for (int i = 0; i < num; i++)
	{
		vecP[i].SetIndex(i); // save the index of each point after sorting
	}
	sortedVecX = vecP;

	sort(vecP.begin(), vecP.end(), less_equal<Point>()); // sort vecP by y-coordinate
	sortedVecY = vecP;

	GetClosestPairofPoints(sortedVecX, sortedVecY); // call GetClosestPairofPoints()
	system("pause");
	return 0;
}

