/******************************************************************************
*
* FileName : StableMatching1.cpp
*
* Author : Wenbo Guo
*
* Date : Oct 04 2014
*
* Description : To get a stable matching based on a given data file, and show 
*               the output in the order the people are listed in the data file.
*
* Function : _tmain()
* 
******************************************************************************/

#include "stdafx.h"
#include "iostream"
#include "GetStableMatching.h"
#include "People.h"
#include "Pet.h"

using namespace std;

/*
* @brief _tmain() function is the entrance of the program. It is to get a stable
*		 matching by calling GetStableMatching(People &peo, Pet &pet) function.
*		 And show the result of stable matching .
*
* @param
*
* @return 0 shows the program has run successfully.
*
*/
int _tmain(int argc, _TCHAR* argv[])
{
	string dataFile = "program1data.txt"; // a local file in the default directory named program1data.txt
	People peo = People(dataFile); //creat an object of class People.
	Pet pet = Pet(dataFile); //creat an object of class Pet.

	if (GetStableMatching(peo, pet) == 0) // call function GetStableMatching()
	{
		cout << '\n' << "Getting a stable matching successfully!" << '\n' 
			 << endl;
	}
	else
	{
		cerr << "Fail to get a stable matching" << endl;
		system("pause");
		exit(0);
	}

	// Show the stable matching 
	cout << "The stable matching is:" << endl;
	for (int i = 0; i < peo.GetPeoNum(); i++)
	{
		cout << peo.GetPeoName(i) << "/" << pet.GetPetName(peo.GetMatchedPet(i)) 
			 << endl;
	}

	system("pause");
	return 0;
}