/**************************************************************************************
*
* FileName : Point.cpp
*
* Author : Wenbo Guo
*
* Date : Oct 18 2014
*
* UpDate : Oct 23 2014
*
* Description : Include the definition of class Point.
*
* Function : Point() is the constructor for class Point.
*			 ~Point() is the destructor for class Point.
*			 GetX() is to get the x-coordinate of a point.
*			 GetY() is to get the y-coordinate of a point.
*		     GetIndex() is to get the index of a point. 
*			 SetX() is to set the x-coordinate of a point.
*			 SetY() is to set the y-coordinate of a point.
*			 SetIndex() is to set the index of a point.
*            operator <() is to overload operator "<".
*            operator <=() is to overload operator "<=".
*
**************************************************************************************/

#include "stdafx.h"
#include "Point.h"

/*
* @brief Point() is the constructor of this class. 
*
*/
Point::Point(void)
{
}

/*
* @brief Point() is the constructor of this class. 
*
* @param x the x-coordinate of the point.
* @param y the y-coordinate of the point.
*
*/
Point::Point(float x, float y)
{
	this->x = x;
	this->y = y;
}

/*
* @brief ~Point() is the destructor of this class. 
*
*/
Point::~Point(void)
{
}

/*
* @brief GetX() is to get the x-coordinate of a point.
*
* @return x the x-coordinate of the point.
*
*/
float Point::GetX() const
{
	return this->x;
}

/*
* @brief GetY() is to get the y-coordinate of a point.
*
* @return y the y-coordinate of the point.
*
*/
float Point::GetY() const
{
	return this->y;
}

/*
* @brief GetIndex() is to get the index of a point.
*
* @return index the index of the point.
*
*/
int Point::GetIndex() const
{
	return this->index;
}

/*
* @brief SetX() is to set the x-coordinate of a point. 
*
* @param x the x-coordinate of a point.
*
* @return 0 shows this function has run successfully.
*
*/
int Point::SetX(float x)
{
	this->x = x;
	return 0;
}

/*
* @brief SetY() is to set the y-coordinate of a point. 
*
* @param y the y-coordinate of a point.
*
* @return 0 shows this function has run successfully.
*
*/
int Point::SetY(float y)
{
	this->y = y;
	return 0;
}

/*
* @brief SetIndex() is to set the index of a point. 
*
* @param index the index of a point.
*
* @return 0 shows this function has run successfully.
*
*/
int Point::SetIndex(int index)
{
	this->index = index;
	return 0;
}

/*
* @brief operator <() is to overload operator "<". 
*
* @param p a point whose x-coordinate is compared.
*
* @return true shows p's x-coordinate is larger.
* @return false shows p's x-coordinate is smaller.
*/
bool Point::operator <(const Point& p) const
{
	return this->x < p.x;
}

/*
* @brief operator <=() is to overload operator "<=".
*
* @param p a point whose y-coordinate is compared.
*
* @return true shows p's y-coordinate is larger.
* @return false shows p's y-coordinate is smaller.
*/
bool Point::operator <=(const Point& p) const
{
	return this->y < p.y;
}