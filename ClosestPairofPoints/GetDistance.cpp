/*************************************************************************
*
* FileName : GetDistance.cpp
*
* Author : Wenbo Guo
*
* Date : Oct 18 2014
*
* UpDate : Oct 23 2014
*
* Description :Include the definition of function GetDistance.
*
* Function : GetDistance() is to get the distance between two 
*            points.
*
*************************************************************************/

#include "stdafx.h"
#include "Point.h"
#include "algorithm"

using namespace std;

/*
* @brief GetDistance() function is to get the distance between two 
*        points.
*        It is called by GetClosestPairofPoints().
*
* @param  p1 a point.
* @param  p2 a point.
*
* @return disP shows the distance between the closest pair of points.
*
*/
float GetDistance(const Point &p1,const Point &p2)
{
	float disX = powf(p1.GetX() - p2.GetX(), 2);
	float disY = powf(p1.GetY() - p2.GetY(), 2);
	float disP = sqrtf(disX + disY);
	return disP;
}