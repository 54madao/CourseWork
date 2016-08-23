/************************************************************************
*
* FileName : GetDistance.h
*
* Author : Wenbo Guo
*
* Date : Oct 18 2014
*
* UpDate : Oct 23 2014
*
* Description : A header for GetDistance.ccp. 
*				Include the declaration of function GetDistance.
*
* Function : GetDistance() is to get the distance between two 
*            points.
*
************************************************************************/

#pragma once

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
float GetDistance(const Point &p1, const Point &p2);