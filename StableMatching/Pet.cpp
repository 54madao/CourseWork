/*****************************************************************************************
*
* FileName : Pet.cpp
*
* Author : Wenbo Guo
*
* Date : Oct 04 2014
*
* Description : Definition of class Pet.
*
* Function : GetData() is to get data from a given data source.
*			 PrintData() is to show the data read from the data source.	
*            GetPetNum() is to get the total number of pets from m_petNumber.
*			 GetPetName(int petIndex) is to get a specified pet's name from m_petName.
*		     CompPetPrefRank(int petIndex, int PropPeoIndice) is to compare the rank 
*			 of two persons in a specified pet's preference rank list based on m_petPrefRank.
*			 GetMatchedPeople(int petIndex) is to get the indice of the person 
*			 that is currently matched with a specified pet.
*			 SetMatchedPeople(int petIndex, int peoIndice) is to mark a specified pet-people match.
*
*****************************************************************************************/

#include "stdafx.h"
#include "Pet.h"

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
int Pet :: GetData()
{
	int i = 1; // count the number of lines already read from data file.
	int rank = 1; // mark the rank of a person in pet preference lists.
	char buf[1024]; // save the data read from data source
	string msg;
	string subMsg;
	vector<string> eachPetPref; // preference list of each pet.
	vector<int> eachPetPrefRank; // preference rank list of each pet.
	ifstream fin(m_dataSource);

	if (fin.is_open()) // open the file successfully
	{
		while (fin.good() && !fin.eof())
		{
			// read data.
			memset(buf, 0, 1024);
			fin.getline(buf, 1024);
			msg = buf;

			if (i == 1) 
			{
				m_petNumber = atoi(msg.c_str()); // save total number of pets.

				// initialize m_matchedPeople.
				vector<int> temp(m_petNumber, -1);
				m_matchedPeople = temp;
			}
			else if ((i >= 2 * m_petNumber + 2) 
				    && (i <= 3 * m_petNumber + 1)) 
			{
				m_petName.push_back(msg); // save pets' names.
			}
			else if ((i >= 3 * m_petNumber + 2) 
				    && (i <= 4 * m_petNumber + 1)) 
			{
				stringstream ss(msg);
				eachPetPrefRank.resize(m_petNumber);

				while (getline(ss, subMsg, ' '))
				{
					eachPetPref.push_back(subMsg);

					// make a preference rank list
					int pos = atoi(subMsg.c_str());
					eachPetPrefRank[pos-1] = rank;
					rank++;
				}

				// save pets' preference lists and preference rank lists.
				m_petPref.push_back(eachPetPref);
				m_petPrefRank.push_back(eachPetPrefRank);

				eachPetPref.clear();
				eachPetPrefRank.clear();
				rank = 1; // reset rank to 1 for the next pet.
			}

			i++;
		} // while (fin.good()&&!fin.eof())

		fin.close();
		return 0;
	} // if (fin.is_open())
	else
		return -1;
}

/*
* @brief PrintData() function is to show the data read from a given data file.
*        It is called by the constructor function of class Pet.
*
* @param
*
* @return 0 shows this function has run successfully.
*
*/
int Pet :: PrintData() const
{
	cout << "Getting data successfully!" << '\n' << endl;
	cout << "The number of pets:" << m_petNumber << '\n' << endl;
	cout << "Preference of each pet:" << endl;

	for (int i = 0; i < m_petNumber; i++)
	{
		cout << m_petName[i] << " ";

		for (int j = 0; j < m_petNumber; j++)
		{
			cout << m_petPref[i][j] << " ";
		}

		cout << endl;
	}
	cout << endl;
	return 0;
}

/*
* @brief GetPetNum() is to get the total number of pets from m_petNumber. 
*
* @param
*
* @return  m_petNumber total number of pet.
*
*/
int Pet :: GetPetNum() const
{
	return m_petNumber;
}

/*
* @brief GetPetName(int petIndex) is to get a specified pet's name from m_petName.
*
* @param petIndex the index of the specified pet in m_petName.
*
* @return m_petName[petIndex] the name of the specified pet.
*
*/
string Pet :: GetPetName(int petIndex) const
{
	return m_petName[petIndex];
}

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
bool Pet :: CompPetPrefRank(int petIndex, int PropPeoIndice) const
{
	if (m_petPrefRank[petIndex][PropPeoIndice] 
		< m_petPrefRank[petIndex][GetMatchedPeople(petIndex)])
		return true;
	else
		return false;
}

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
int Pet :: GetMatchedPeople(int petIndex) const
{
	return m_matchedPeople[petIndex];
}

/*
* @brief SetMatchedPeople(int petIndex, int peoIndice) is to mark a specified pet-people match.
*
* @param petIndex the index of the specified pet in m_matchedPeople.
* @param peoIndice The indice of a person.
*
* @return 0 shows this function has run successfully.
*
*/
int Pet :: SetMatchedPeople(int petIndex, int peoIndice)
{
	m_matchedPeople[petIndex]=peoIndice;
	return 0;
}