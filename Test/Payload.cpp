
/////////////////////////////////////////////////////////////////////
// Payload.cpp - Implements the compare method defined in payload.h file   //
// ver 1.0                                                         //
// Author: Chetali Mahore, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////
/*
Package Operations:
* --------------
* 
*  A global function of operator overloading is declared in this file.
*
*
* Maintenance History:
* --------------------
* ver 1.0 : 1st February 2018
*/



#include "Payload.h"
#include<vector>
#include<string>
#include <iomanip>


/*-------------------To compare the payload and return true or false--------------*/

int Payload::compare(Payload& p)
{
	std::vector<std::string> currentList = this->getCategory(); // caller object is payload from dbelement
	std::vector<std::string> keyList = p.getCategory();// checking against the payload object fetched form cond
		for (auto iter1 : currentList)
		{
			for (auto iter2 : keyList)
			{
				if (iter1 == iter2) {
					return 1;
				}
			}
		}
	return 0;
}
