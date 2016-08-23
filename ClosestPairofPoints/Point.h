/**************************************************************************************
*
* FileName : Point.h
*
* Author : Wenbo Guo
*
* Date : Oct 18 2014
*
* UpDate : Oct 23 2014
*
* Description : A header for Point.ccp. 
*				Include the declaration of class Point which offers methods for the 
*               object contains x-coordinate, y-coordinate, and index.
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

#pragma once

/*
* @brief this class is for the object contains x-coordinate,
*        y-coordinate, and index.
*
*/
class Point
{
public:

	/*
	* @brief Point() is the constructor of this class. 
	*
	*/
	Point(void);

	/*
	* @brief Point() is the constructor of this class. 
	*
	* @param x the x-coordinate of the point.
	* @param y the y-coordinate of the point.
	*
	*/
	Point(float, float);

	/*
	* @brief ~Point() is the destructor of this class. 
	*
	*/
	virtual ~Point(void);

	/*
	* @brief GetX() is to get the x-coordinate of a point.
	*
	* @return x the x-coordinate of the point.
	*
	*/
	float GetX() const;

	/*
	* @brief GetY() is to get the y-coordinate of a point.
	*
	* @return y the y-coordinate of the point.
	*
	*/
	float GetY() const;

	/*
	* @brief GetIndex() is to get the index of a point.
	*
	* @return index the index of the point.
	*
	*/
	int GetIndex() const;

	/*
	* @brief SetX() is to set the x-coordinate of a point. 
	*
	* @param x the x-coordinate of a point.
	*
	* @return 0 shows this function has run successfully.
	*
	*/
	int SetX(float);

	/*
	* @brief SetY() is to set the y-coordinate of a point. 
	*
	* @param y the y-coordinate of a point.
	*
	* @return 0 shows this function has run successfully.
	*
	*/
	int SetY(float);

	/*
	* @brief SetIndex() is to set the index of a point. 
	*
	* @param index the index of a point.
	*
	* @return 0 shows this function has run successfully.
	*
	*/
	int SetIndex(int);

	/*
	* @brief operator <() is to overload operator "<". 
	*
	* @param p a point whose x-coordinate is compared.
	*
	* @return true shows p's x-coordinate is larger.
	* @return false shows p's x-coordinate is smaller.
	*/
	bool operator <(const Point& p) const;

	/*
	* @brief operator <=() is to overload operator "<=".
	*
	* @param p a point whose y-coordinate is compared.
	*
	* @return true shows p's y-coordinate is larger.
	* @return false shows p's y-coordinate is smaller.
	*/
	bool operator <=(const Point& p) const;

private:
	float x; // x-coordinate
	float y; // y-coordinate
	int index; // index
};

