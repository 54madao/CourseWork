/************************************************************************
*
* FileName : SixNeighborAbove.h
*
* Author : Wenbo Guo
*
* Date : Oct 18 2014
*
* UpDate : Oct 23 2014
*
* Description : A header for SixNeighborAbove.ccp. 
*				Include the declaration of function SixNeighborAbove.
*
* Function : SixNeighborAbove() is to calculate if the closest 
*            pair of points is across Line by considering six neighbors 
*            above one point.
*
************************************************************************/

#pragma once

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
float SixNeighborAbove(const vector<Point> &leftY, const vector<Point> &rightY, float d);