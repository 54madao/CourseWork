/*************************************************************************
*
* FileName : SixNeighborAbove.cpp
*
* Author : Wenbo Guo
*
* Date : Oct 18 2014
*
* UpDate : Oct 23 2014
*
* Description :Include the definition of function SixNeighborAbove.
*
* Function : SixNeighborAbove() is to calculate if the closest 
*            pair of points is across Line by considering six neighbors
*            above one point.
*
*************************************************************************/

#include "stdafx.h"
#include "vector"
#include "Point.h"
#include "iostream"
#include "GetDistance.h"

using namespace std;

/*
* @brief SixNeighborAbove() function is to calculate if the closest 
*        pair of points is across Line by considering six neighbors
*        above one point.
*        It is called by GetClosestPairofPoints().
*
* @param  leftY a group of points sorted by y-coordinate.
* @param  rightY a group of points sorted by y-coordinate.
* @param  d the distance between the closest pair of points in left 
*         and right groups
*
* @return d shows the distance between the closest pair of points.
*
*/
float SixNeighborAbove(const vector<Point> &leftY, const vector<Point> &rightY, float d)
{
	float dBtw; // distance between two points
	int i,j,k;
	int leftLen = leftY.size();
	int rightLen = rightY.size();

	// either left or right part is empty
	if( leftLen <= 0 || rightLen <= 0 )
		return d;

	for (i = 0, j = 0; i < leftLen && j < rightLen; )
	{
		if ( leftY[i] <= rightY[j]) // y-coordinate of left point is smaller
		{
			//calculate distances with six neighbors respectively
			for (k = j; k < j + 6 && k < rightLen; k++)
			{
				dBtw = GetDistance(leftY[i], rightY[k]);
				if (dBtw < d)
				{
					d = dBtw;
				}
			}
			i++;
		}
		else // y-coordinate of right point is smaller
		{
			//calculate distances with six neighbors respectively
			for (k = i; k < i + 6 && k < leftLen; k++)
			{
				dBtw = GetDistance(leftY[k], rightY[j]);
				if (dBtw < d)
				{
					d = dBtw;
				}
			}
			j++;
		}
	}//for
	return d;
}