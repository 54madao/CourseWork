/************************************************************************
*
* FileName : GetStableMatching.h
*
* Author : Wenbo Guo
*
* Date : Oct 04 2014
*
* Description : A header for GetStableMatching.ccp. 
*				Include the declaration of function GetStableMatching.
*
* Function : GetStableMatching(People &peo, Pet &pet) is to get a stable 
*			 matching based on each person's preference for each pet and
*			 each pet's preference for each person.
*
************************************************************************/

#include "People.h"
#include "pet.h"

using namespace std;

/*
* @brief GetStableMatching(People &peo, Pet &pet) function is to get a stable 
*		 matching based on people's preference lists and pets' preference lists.
*        It is called by _tmain().
*
* @param  peo an object of class People created in _tmain().
* @param  pet an object of class Pet created in _tmain().
*
* @return 0 shows this function has run successfully.
* @return -1 shows no stable matching exists for the given data 
*            under Gale-Shapley algorithm.
*
*/
int GetStableMatching(People &peo, Pet &pet);