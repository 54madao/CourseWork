/*************************************************************************
*
* FileName : GetStableMatching.cpp
*
* Author : Wenbo Guo
*
* Date : Oct 04 2014
*
* Description :Include the definition of function GetStableMatching.
*
* Function : GetStableMatching(People &peo, Pet &pet) is to get a stable 
*			 matching based on each person's preference for each pet and
*			 each pet's preference for each person.
*
*************************************************************************/

#include "stdafx.h"
#include "People.h"
#include "Pet.h"
#include "iostream"

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
int GetStableMatching(People &peo, Pet &pet)
{
	int peoPrefPet; // the indice of the pet a person wants to be with.
	queue<int> waitLine; // the indices of people waiting for a matching.

	// initialize waitLine.
	for (int i = 0; i < peo.GetPeoNum(); i++)
	{	
		waitLine.push(i);
	}

	while (!waitLine.empty())
	{
		int peoNum = waitLine.front(); // the indice of the first person in the waitLine.
		waitLine.pop();
		peoPrefPet = peo.GetPeoPref(peoNum);

		if (peoPrefPet != -1) // there's an indice of a pet in the person's preference list.
			peoPrefPet--; // revised to accomodate the range of the vector.
		else
			return -1;

		if (pet.GetMatchedPeople(peoPrefPet) 
			== -1) // the pet preferred by the person is unmatched
		{
			// match the person and the pet with each other.
			pet.SetMatchedPeople(peoPrefPet, peoNum);
			peo.SetMatchedPet(peoNum, peoPrefPet);
		}
		else if (pet.CompPetPrefRank(peoPrefPet,
			     peoNum)) // the pet prefers the person to its current master
		{
			// let the current master wait in the waitLine, and remove their matching.
			waitLine.push(pet.GetMatchedPeople(peoPrefPet));
			peo.SetMatchedPet(pet.GetMatchedPeople(peoPrefPet), -1);

			// match the pet with a new master it prefers.
			pet.SetMatchedPeople(peoPrefPet, peoNum);
			peo.SetMatchedPet(peoNum, peoPrefPet);
		}
		else // the pet prefers its current master to the person.
		{
			waitLine.push(peoNum); // let the person wait in the waitLine.
		}
	} // while (!people.empty())

	return 0;
}