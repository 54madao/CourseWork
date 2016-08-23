/************************************************************************
*
* FileName : SixNeighborAround.h
*
* Author : Wenbo Guo
*
* Date : Oct 18 2014
*
* UpDate : Oct 23 2014
*
* Description : A header for SixNeighborAround.ccp. 
*				Include the declaration of function SixNeighborAround.
*
* Function : SixNeighborAround() is to calculate if the closest 
*            pair of points is across Line by considering six neighbors 
*            around one point.
*
************************************************************************/

#pragma once

using namespace std;

/*
* @brief SixNeighborAround() function is to calculate if the closest 
*        pair of points is across Line by considering six neighbors 
*        around one point.
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
float SixNeighborAround(const vector<Point> &leftY, const vector<Point> &rightY, float d);