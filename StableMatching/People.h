/**************************************************************************************
*
* FileName : People.h
*
* Author : Wenbo Guo
*
* Date : Oct 04 2014
*
* Description : A header for People.ccp. 
*				Include the declaration of class People which offers methods for the 
*               objects contain pets' name lists, preference lists, and a matching list.
*
* Function : People(string dataSource) is the constructor for class People.
*			 ~People() is the destructor for class People.
*			 GetPeoNum() is to get the total number of people from m_peoNumber.
*			 GetPeoName(int peoIndex) is to get a specified person's name from m_peoName.
*		     GetPeoPref(int peoIndex) is to get the first pet indice in a 
*			 specified person's preference list from m_peoPref.
*			 GetMatchedPet(int peoIndex) is to get the indice of the pet that 
*			 is currently matched with a specified person.
*			 SetMatchedPet(int peoIndex, int petIndice) is to mark a specified people-pet match.
*			 GetData() is to get data from a given data source.
*			 PrintData() is to show the data read from the data source.
*
**************************************************************************************/

#include "vector"
#include "string"
#include "queue"
#include "fstream"
#include "sstream"
#include "iostream"

using namespace std;

#ifndef PEOPLE_H
#define PEOPLE_H

/*
* @brief this class is for the objects contain people's name lists,
*        preference lists, and a matching list.
*
*/
class People
{
public:

	/*
	* @brief People(string dataSource) is the constructor function of this class. 
	*
	* @param dataSource name of data file.
	*
	* @return
	*
	*/
	People(string dataSource)
	{
		this->m_dataSource = dataSource; 

		if (GetData() == 0) //get the data from the data source
		{
			PrintData(); // show the data read from data source
		}
		else
		{
			cerr << "Fail to get data from the file " << m_dataSource
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
	virtual ~People(void)
	{
	}

	/*
	* @brief GetPeoNum() is to get the total number of people from m_peoNumber. 
	*
	* @param
	*
	* @return m_peoNumber total number of people.
	*
	*/
	int GetPeoNum() const;

	/*
	* @brief GetPeoName(int peoIndex) is to get a specified person's name.
	*
	* @param peoIndex the index of the specified person in m_peoName.
	*
	* @return m_peoName[peoIndex] the name of the specified person.
	*
	*/
	string GetPeoName(int peoIndex) const;

	/*
	* @brief GetPeoPref(int peoIndex) is to get the first pet indice in a 
	*			 specified person's preference list from m_peoPref.
	*
	* @param peoIndex the index of the specified person in m_peoPref.
	*
	* @return petIndice the first pet indice in the specified person's preference list
	* @return -1 shows no pet indice is in the specified person's preference list
	*	
	*/
	int GetPeoPref(int peoIndex);

	/*
	* @brief GetMatchedPet(int peoIndex) is to get the indice of the pet that 
	*		 is currently matched with a specified person from m_matchedPet.
	*
	* @param peoIndex the index of the specified person in m_matchedPet.
	*
	* @return m_matchedPet[peoIndex] the indice of the pet currently matched
	*         with the specified person.
	*
	*/
	int GetMatchedPet(int peoIndex) const;

	/*
	* @brief SetMatchedPet(int peoIndex, int petIndice) is to mark a specified 
	*        people-pet match. 
	*
	* @param peoIndex the index of the specified person in m_matchedPet.
	*        petIndice the indice of a pet.
	*
	* @return 0 shows this function has run successfully.
	*
	*/
	int SetMatchedPet(int peoIndex, int petIndice); 

private:

	int m_peoNumber; // total number of people.
	string m_dataSource; // file name of data file.
	vector<string> m_peoName; // name list of people
	vector<queue<int> > m_peoPref; // preference lists of people
	vector<int> m_matchedPet;// people-pet matching list. 

	/*
	* @brief GetData() function is to get data for member variables of class People 
	*        from a given data file.
	*		 Line 1: Number of people/pets (n)
	*        Lines 2 to n+1: Names of people
	*        Lines n+2 to 2n+1: Preference lists of people using indices, not names 
	*                           (n preferences per line)
	*        It is called by the constructor function of class People.
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
	*        It is called by the constructor function of class People.
	*
	* @param
	*
	* @return 0 shows this function has run successfully.
	*
	*/
	int PrintData(); 
};

#endif
