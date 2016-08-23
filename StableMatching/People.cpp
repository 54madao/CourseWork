/****************************************************************************************
*
* FileName : People.cpp
*
* Author : Wenbo Guo
*
* Date : Oct 04 2014
*
* Description : Include the definition of class People.
*
* Function : GetData() is to get data from a given data source.
*			 PrintData() is to show the data read from the data source.
*            GetPeoNum() is to get the total number of people from m_peoNumber.
*			 GetPeoName(int peoIndex) is to get a specified person's name from m_peoName.
*		     GetPeoPref(int peoIndex) is to get the first pet indice in a 
*			 specified person's preference list from m_peoPref.
*			 GetMatchedPet(int peoIndex) is to get the indice of the pet that 
*			 is currently matched with a specified person.
*			 SetMatchedPet(int peoIndex, int petIndice) is to mark a specified people-pet match. 
*
****************************************************************************************/

#include "stdafx.h"
#include "People.h"

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
int People :: GetData()
{
	int i = 1; // count the number of lines already read from data file.
	char buf[1024]; // save the data read from data source
	string msg;
	string subMsg;
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
				m_peoNumber = atoi(msg.c_str()); // save total number of people.

				// initialization of m_matchedPet.
				vector<int> temp(m_peoNumber, -1);
				m_matchedPet = temp;
			}
			else if ((i >= 2) && (i <= m_peoNumber + 1)) 
			{
				m_peoName.push_back(msg); // save people's names.
			}
			else if ((i >= m_peoNumber + 2) 
				    && (i <= 2 * m_peoNumber + 1)) 
			{
				
				stringstream ss(msg);
				queue<int> eachPeoPref; // preference list of each person.

				while (getline(ss, subMsg, ' '))
				{
					eachPeoPref.push(atoi(subMsg.c_str()));
				}

				m_peoPref.push_back(eachPeoPref); // save people's preference lists.
			}
			else if(i > 2 * m_peoNumber + 1)
			{
				break;
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
*        It is called by the constructor function of class People.
*
* @param
*
* @return 0 shows this function has run successfully.
*
*/
int People :: PrintData()
{
	cout << "Getting data successfully!" << '\n' << endl;
	cout << "The number of people:" << m_peoNumber << '\n' << endl;
	cout << "Preference of each person:" << endl;

	for (int i = 0; i < m_peoNumber; i++)
	{
		cout << m_peoName[i] << " ";

		for (int j = 0; j < m_peoNumber; j++)
		{
			int temp = m_peoPref[i].front();
			cout<< temp << " ";
			m_peoPref[i].pop();
			m_peoPref[i].push(temp);
		}

		cout << endl;
	}
	cout << endl;
	return 0;
}

/*
* @brief GetPeoNum() is to get the total number of people from m_peoNumber. 
*
* @param
*
* @return m_peoNumber total number of people.
*
*/
int People :: GetPeoNum() const
{
	return m_peoNumber;
}

/*
* @brief GetPeoName(int peoIndex) is to get a specified person's name.
*
* @param peoIndex the index of the specified person in m_peoName.
*
* @return m_peoName[peoIndex] the name of the specified person.
*
*/
string People :: GetPeoName(int peoIndex) const
{
	return m_peoName[peoIndex];
}

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
int People :: GetPeoPref(int peoIndex)
{
	if (!m_peoPref[peoIndex].empty())
	{
		int petIndice = m_peoPref[peoIndex].front();
		m_peoPref[peoIndex].pop();
		return petIndice;
	}
	else
		return -1;
}

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
int People :: GetMatchedPet(int peoIndex) const
{
	return m_matchedPet[peoIndex];
}

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
int People :: SetMatchedPet(int peoIndex, int petIndice)
{
	m_matchedPet[peoIndex]=petIndice;
	return 0;
}