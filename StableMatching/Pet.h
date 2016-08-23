/********************************************************************************************
*
* FileName : Pet.h
*
* Author : Wenbo Guo
*
* Date : Oct 04 2014
*
* Description : A header for Pet.ccp. 
*				Include the declaration of class Pet which offers methods for 
*               the objects contain pets' name lists, preference lists, and a matching list.
*
* Function : Pet(string dataSource) is the constructor for class Pet.
*			 ~Pet() is the destructor for class Pet.
*			 GetPetNum() is to get the total number of pets from m_petNumber.
*			 GetPetName(int petIndex) is to get a specified pet's name from m_petName.
*		     CompPetPrefRank(int petIndex, int PropPeoIndice) is to compare the rank 
*			 of two persons in a specified pet's preference rank list based on m_petPrefRank.
*			 GetMatchedPeople(int petIndex) is to get the indice of the person 
*			 that is currently matched with a specified pet.
*			 SetMatchedPeople(int petIndex, int peoIndice) is to mark a specified pet-people match.
*	         GetData() is to get data from a given data source.
*			 PrintData() is to show the data read from the data source.
*
********************************************************************************************/

#include "vector"
#include "string"
#include "queue"
#include "fstream"
#include "sstream"
#include "iostream"

using namespace std;

#ifndef PET_H
#define PET_H

/*
* @brief this class is for the objects contain pets' name lists,
*        preference lists, and a matching list.
*
*/
class Pet
{
public:

	/*
	* @brief Pet(string dataSource) is the constructor function of this class. 
	*
	* @param dataSource name of data file.
	*
	* @return
	*
	*/
	Pet(string dataSource)
	{
		this->m_dataSource = dataSource; 

		if (GetData() == 0) //get the data from the data source
		{
			PrintData(); // show the data read from data source
		}
		else
		{
			cerr << "Fail to get data from a file " << m_dataSource 
				 << "!" << endl;
			system("pause");
			exit(0);
		}
	}

	/*
	* @brief People(string dataSource) is the destructor function of this class. 
	*
	* @param 
	*
	* @return
	*
	*/
	virtual ~Pet(void)
	{
	}

	/*
	* @brief GetPetNum() is to get the total number of pets from m_petNumber. 
	*
	* @param
	*
	* @return  m_petNumber total number of pet.
	*
	*/
	int GetPetNum() const;

	/*
	* @brief GetPetName(int petIndex) is to get a specified pet's name from m_petName.
	*
	* @param petIndex the index of the specified pet in m_petName.
	*
	* @return m_petName[petIndex] the name of the specified pet.
	*
	*/
	string GetPetName(int petIndex) const;

	/*
	* @brief  CompPetPrefRank(int petIndex, int PropPeoIndice) is to compare the rank 
	*		  of two persons in a specified pet's preference rank list based on m_petPrefRank.
	*
	* @param petIndex the index of the specified pet in m_petPrefRank
	* @param PropPeoIndice the indice of the person that wants the specified pet.
	*
	* @return true shows the specified pet perfers the person who wants it
	*		  to its current master.
	* @return false shows the specified pet perfers its current master
	*		  to the person who wants it.
	*
	*/
	bool CompPetPrefRank(int petIndex, int PropPeoPos) const; 

	/*
	* @brief GetMatchedPeople(int petIndex) is to get the indice of the person 
	*		 that is currently matched with a specified pet.
	*
	* @param petIndex the index of the specified pet in m_matchedPeople.
	*
	* @return m_matchedPeople[petIndex] the indice of the person currently 
	*         matched with the specified pet.
	*
	*/
	int GetMatchedPeople(int petIndex) const;

	/*
	* @brief SetMatchedPeople(int petIndex, int peoIndice) is to mark a specified pet-people match.
	*
	* @param petIndex the index of the specified pet in m_matchedPeople.
	* @param peoIndice The indice of a person.
	*
	* @return 0 shows this function has run successfully.
	*
	*/
	int SetMatchedPeople(int petIndex, int peoIndice);

private:

	int m_petNumber; // total number of pets.
	string m_dataSource; // file name of data file.
	vector<string> m_petName; // name list of pets.
	vector<vector<string> > m_petPref; // preference lists of pets
	vector<vector<int> > m_petPrefRank; // preference rank lists of pets
	vector<int> m_matchedPeople; // pet-people matching list.

	/*
	* @brief GetData() function is to get data for member variables of class Pet 
	*        from a given data file.
	*        Line 1: Number of people/pets (n)
	*		 Lines 2n+2 to 3n+1: Names of pets
	*        Lines 3n+2 to 4n+1: Preference lists of pets using indices, not name 
	*                            (n preferences per line)
	*        It is called by the constructor function of class Pet.
	*
	* @param
	*
	* @return 0 shows this function has run successfully.
	* @return -1 shows it failed to open the given data file.
	*
	*/
	int GetData();

	/*
	* @brief PrintData() function is to show the data read from a given data file.
	*        It is called by the constructor function of class Pet.
	*
	* @param
	*
	* @return 0 shows this function has run successfully.
	*
	*/
	int PrintData() const;
};

#endif
