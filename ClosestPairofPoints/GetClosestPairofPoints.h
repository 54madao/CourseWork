/************************************************************************
*
* FileName : GetClosestPairofPoints.h
*
* Author : Wenbo Guo
*
* Date : Oct 18 2014
*
* UpDate : Oct 23 2014
*
* Description : A header for GetClosestPairofPoints.ccp. 
*				Include the declaration of function GetClosestPairofPoints.
*
* Function : GetClosestPairofPoints() is to get the distance between the 
*            closest pair of points.
*
************************************************************************/

#pragma once

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
float GetClosestPairofPoints(const vector<Point> &vecX, const vector<Point> &vecY);
