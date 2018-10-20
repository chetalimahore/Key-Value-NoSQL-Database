/////////////////////////////////////////////////////////////////////
// Payload.h - Implements the payload class which contains the filepath and the category list     //
// ver 1.0                                                         //
// Author : Chetali Mahore, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////
/*
Package Operations:
* ------------
*
*  The Payload class contains the private members like the filepath and the filecategories.
*  A global function of operator overloading is declared in this file.
*
*
* Maintenance History:
* --------------------
* ver 1.0 : 1st February 2018
*/


#pragma once

#include<string>
#include<vector>
#include<iostream>


class Payload
{
	private:
		std::string path;
		std::vector<std::string> categories;

	public:
		std::string& getPathName() { return path; }
		void setPathName(const std::string path_) { path = path_; }

		std::vector<std::string>& getCategory() {
			return categories;
		}
		void setCategory(const std::vector<std::string> category) { categories = category; }

		int compare(Payload& p);
	
};

/*------------Opertor overloading to display the contents of the Payload---------------*/

inline std::ostream& operator<<(std::ostream& out, Payload& p) {
	out << p.getPathName();
	std::vector<std::string> list = p.getCategory();
	for (auto itr : list)
	{
		out << itr << " ";
	}
	return out;
}
