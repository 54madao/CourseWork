/************************************************************************
*
* FileName : FourNeighborAbove.h
*
* Author : Wenbo Guo
*
* Date : Oct 18 2014
*
* UpDate : Oct 23 2014
*
* Description : A header for FourNeighborAbove.ccp. 
*				Include the declaration of function FourNeighborAbove.
*
* Function : FourNeighborAbove() is to calculate if the closest 
*            pair of points is across Line by considering four neighbors 
*            above one point.
*
************************************************************************/

#pragma once

using namespace std;

/*
* @brief FourNeighborAbove() function is to calculate if the closest 
*        pair of points is across Line by considering four neighbors
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
float FourNeighborAbove(const vector<Point> &leftY, const vector<Point> &rightY, float d);