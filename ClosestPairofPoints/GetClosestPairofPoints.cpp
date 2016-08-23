/*************************************************************************
*
* FileName : GetClosestPairofPoints.cpp
*
* Author : Wenbo Guo
*
* Date : Oct 18 2014
*
* UpDate : Oct 23 2014
*
* Description :Include the definition of function GetClosestPairofPoints.
*
* Function : GetClosestPairofPoints() is to get the distance between the 
*            closest pair of points.
*
*************************************************************************/

#include "stdafx.h"
#include "iostream"
#include "vector"
#include "Point.h"
#include "GetDistance.h"
#include "SixNeighborAbove.h"

using namespace std;

/*
* @brief GetClosestPairofPoints() function is to get the distance between the 
*        closest pair of points.
*        It is called by _tmain().
*
* @param  vecX a group of points sorted by x-coordinate.
* @param  vecY a group of points sorted by y-coordinate.
*
* @return d shows the distance between the closest pair of points.
*
*/
float GetClosestPairofPoints(const vector<Point> &vecX, const vector<Point> &vecY)
{
	float d = -1; // save the distance between the closest pair of points
	int index1, index2; // save the range
	int length = vecX.size();
	int size = vecY.size();

	if (length <= 3) 
	{
		//calculate the distances using brute-force with three or fewer points
		for (int i = 0; i < length - 1; i++)
		{
			int j = i + 1;

			while (j < length)
			{
				// calculate the distance between two points.
				float disP = GetDistance(vecX[i], vecX[j]);

				if (d == -1) // no distance of the closest pair of points for now
				{
					d = disP;
				}
				else if (disP < d)
				{
					d = disP;
				}

				j++;
			}
		}//for

		// show the result of the recursive call
		index1 = vecX[0].GetIndex();
		index2 = vecX[length - 1].GetIndex();
		cout.precision(4); 
        cout.setf(ios::fixed);
		cout << "D[" << index1 << "," << index2 << "]: " << d << endl;
		return d;
	}//if
	else
	{
		int mid = length / 2 + length % 2; // mid index in vecX
		float upperBound = vecX.back().GetX();
		float lowerBound = vecX.front().GetX();
		vector<Point> leftVecX; // left group of points sorted by x-coordinate
		vector<Point> leftVecY; // left group of points sorted by y-coordinate
		vector<Point> rightVecX; // right group of points sorted by x-coordinate
		vector<Point> rightVecY; // right group of points sorted by y-coordinate

		// separate vecX into two groups
		leftVecX.assign(vecX.begin(), vecX.begin() + mid);
		rightVecX.assign(vecX.begin() + mid, vecX.end());

		float Line = rightVecX[0].GetX(); // x-coordinate that separates points

		// recursive call
		float dl = GetClosestPairofPoints(leftVecX, vecY);
		float dr = GetClosestPairofPoints(rightVecX, vecY);

		// get the distance between the closest pair of points from recursive calls
		if (dl <= dr)
		{
			d = dl;
		}
		else
		{
			d = dr;
		}

		// get groups of points within d length from Line sorted by y-coordinate
		for (int i = 0; i < size; i++)
		{
			float x = vecY[i].GetX();
			if ( x >= Line && x <= Line + d && x <= upperBound)
			{
				rightVecY.push_back(vecY[i]);
			}
			else if ( x >= Line - d && x < Line && x >= lowerBound)
			{
				leftVecY.push_back(vecY[i]);
			}
		}

		// calculate if the closest pair of points is across the Line
		// SixNeighborAround(), FourNeighborAbove() are optional
		d = SixNeighborAbove(leftVecY, rightVecY, d);
	}

	// show the result of the recursive call
	index1 = vecX.front().GetIndex();
	index2 = vecX.back().GetIndex();
	cout.precision(4); 
    cout.setf(ios::fixed);
	cout << "D[" << index1 << "," << index2 << "]: " << d << endl;
	return d;
}

