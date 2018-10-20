#pragma once
/////////////////////////////////////////////////////////////////////
// Query.h - Supports queries to the database								//
// Version - ver 1.0                                                        //
// Author - Chetali Mahore, CSE687 - Object Oriented Design, Spring 2018    //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* - Conditions - Helps to get and set the values present in the database on which queries are performed
* - Query - Supports queries which are performed on the database
*
* The query package also provides functions for displaying:
* - getValueByKey() - returns the DbElement of the specified key
* - getChildren() - returns the children of the specified key
* - getKeys() - returns the keys with the matching regex of key 
* - getName() - returns the keys with the matching regex of name
* - getDescrip() - returns the keys with the matching regex of description
* - getPayload() - returns the keys with the matching payload
* - OR() - returns the result after ORing of the queries
* - AND() - returns the result after ANDing of the queries
* - compareDate - compares the dates and returns true or false
* - comparePayload - compares the payload received as an condition and returns true or false

* Required Files:
* ---------------
* DbCore.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 4th February 2018 
*/



#include "../DbCore/DbCore.h"
#include <string>
#include <iostream>
#include <vector>
#include <regex>

using namespace NoSqlDb;

template<typename T>
class Conditions
{
private:
	std::string key_;
	std::string name_;
	std::string desc_;
	DateTime dateTime_;
	std::string s_;
	std::vector<std::string> children_;
	T payload_;

public:
	Conditions& setName(std::string name) { name_ = name; return *this; }
	Conditions& setDesc(std::string desc) { desc_ = desc; return *this; }
	Conditions& setDateTime(std::string datetime) { dateTime_ = datetime; return *this; }
	Conditions& setKey(std::string key) { key_ = key; return *this; }
	Conditions& setPayload(T payload) { payload_ = payload; return *this; }
	//Conditions& setChildren(std::string key) { key_ = key; DbCore<T>& dbcore; children_ = dbcore[key].children(); return *this; }
	std::string& getName() { return name_; }
	std::string& getDescrip() { return desc_; }
	std::string& getDateTime() { return dateTime_.time(); }
	std::string& getKey() { return key_; }
	std::string& getValueByKey() { return key_; }
	std::vector<std::string>& getChildren() { return children_; }
	T getPayload() { return payload_; }

};


template<typename T>
class Query
{
public:
	Query( DbCore<T>& db) : db_(db) {};
	//Query& from(DbCore<T>::Keys& keys);
	void show(std::string option);
	void initialiseKeys();
	bool regMatch(std::string a, std::string b);
	Query<T>& getValueByKey(Conditions<T>& cond);
	Query<T>& getChildren(Conditions<T>& cond);
	Query<T>& getKeys(Conditions<T>& cond);
	Query<T>& getName(Conditions<T>& cond);
	Query<T>& getDescrip(Conditions<T>& cond);
	Query<T>& getPayload(Conditions<T>& cond);
	bool contains(std::vector<std::string> temp, std::string str);
	Query<T>& OR();
	Query<T>& AND();
	Query<T>& AND(Query<T> &query1, Query<T> &query2);
	Query<T>& OR(Query<T> &query1, Query<T> &query2);
	bool compareDate();
	Query<T>& comparePayload(Conditions<T>& cond);


private:
	DbCore<T>& db_;
	DbElement<T> dbelem;
	std::vector<std::string> keys_ ;
	std::vector<std::string> child_;
	std::string flag = "NONE";
};

/*--------------Display the contents contained by private member 'keys' depending on the input---------------------*/

template<typename T>
void Query<T>::show(std::string option)
{
	if (option == "Value" || option == "Name" || option == "Descrip")
	{
		for (auto it : keys_)
		{
			showElem(db_[it], std::cout);
		}
	}
	else if (option == "Child")
	{
		for (auto itr : child_)
		{
			std::cout << itr << " ";
		}
		std::cout << std::endl;
	}
	else if (option == "Keys" || option == "AND" || option == "OR" || option == "NONE")
	{
		for (auto itr : keys_)
		{
			std::cout << itr << " ";
		}
		std::cout << std::endl;
	}


}

/*-------------------To perform a regex matching between two strings-------------------*/

template<typename T>
bool Query<T>::regMatch(std::string a, std::string b)
{
	std::regex regexb(b);
	return std::regex_match(a, regexb);
}

/*-------------------To get the value of a specified key with regex matching-----------------------*/

template<typename T>
Query<T>& Query<T>::getValueByKey(Conditions<T>& cond)
{
	std::vector<std::string> keys = db_.keys();
	for (auto itr : keys)
	{
		if (regMatch(itr, cond.getKey()))
		{
			//std::string str = keys[i];
			DbElement<T> dbelem = db_[itr];
			keys_.push_back(itr);
		}
	}
	return *this;
}

/*-------------------To get the children of a specified key with regex matching--------------------*/

template<typename T>
Query<T>& Query<T>::getChildren(Conditions<T>& cond)
{
	std::vector<std::string> keys = db_.keys();
	for (auto itr : keys)
	{
		if (regMatch(itr, cond.getKey()))
		{
			std::cout << "\n Children of key :" << itr << std::endl;
			std::vector<std::string> child = db_[itr].children();
			for (auto i : child)
			{
				std::cout << i << " ";
				child_.push_back(i);
			}
		}
	}
	return *this;
}

/*---------------------To get the set of keys by matching the regex ----------------*/

template<typename T>
Query<T>& Query<T>::getKeys(Conditions<T>& cond)
{
	keys_.clear();
	std::vector<std::string> keys = db_.keys();
	for (auto itr : keys)
	{
		if (regMatch(itr, cond.getKey()))
		{
			keys_.push_back(itr);
		}
	}
	return *this;
}

/*----------------------To get the set of keys by matching the name given by a regex function-------------*/

template<typename T>
Query<T>& Query<T>::getName(Conditions<T>& cond)
{
	if (flag == "NONE")
	{
		keys_.clear();
		std::vector<std::string> keys = db_.keys();
		for (auto itr : keys)
		{
			if (regMatch(db_[itr].name(), cond.getName()))
			{
				keys_.push_back(itr);
			}
		}
		return *this;
	}
	else if (flag == "AND")
	{
		std::vector<std::string> temp_keys = keys_;
		keys_.clear();
		for (auto itr : temp_keys)
		{
			if (regMatch(db_[itr].name(), cond.getName()) && !contains(keys_, itr))
			{
				keys_.push_back(itr);
			}
		}
		return *this;
	}
	else if (flag == "OR")
	{
		std::vector<std::string> temp_keys = db_.keys();
		for (auto itr : temp_keys)
		{
			if (regMatch(db_[itr].name(), cond.getName()) && !contains(keys_, itr))
			{
				keys_.push_back(itr);
			}
		}
		return *this;
	}
	return *this;
}

/*-----------------------To get the set of keys by matching the description given by a regex function-------------*/

template<typename T>
Query<T>& Query<T>::getDescrip(Conditions<T>& cond)
{
	if (flag == "NONE")
	{
		keys_.clear();
		std::vector<std::string> temp_keys = db_.keys();
		for (auto itr : temp_keys)
		{
			if (regMatch(db_[itr].descrip(), cond.getDescrip()))
			{
				keys_.push_back(itr);
			}
		}
		return *this;
	}
	else if (flag == "AND")
	{
		std::vector<std::string> temp_keys = keys_;
		keys_.clear();
		for (auto itr : temp_keys)
		{
			if (regMatch(db_[itr].descrip(), cond.getDescrip()) && !contains(keys_, itr))
			{
				keys_.push_back(itr);
			}
		}
		return *this;
	}
	else if (flag == "OR")
	{
		std::vector<std::string> temp_keys = db_.keys();
		for (auto itr : temp_keys)
		{
			if (regMatch(db_[itr].descrip(), cond.getDescrip()) && !contains(keys_, itr))
			{
				keys_.push_back(itr);
			}
		}
		return *this;
	}
	return *this;
}

/*-----------------------To get the set of keys by matching the payload given by a regex function-------------*/

template<typename T>
Query<T>& Query<T>::getPayload(Conditions<T>& cond)
{
	// TODO: insert return statement here
	return *this;
}

/*-----------------------Returns true if the input key matches the database keys------------*/

template<typename T>
bool Query<T>::contains(std::vector<std::string> temp, std::string str)
{
	for (auto itr : temp)
	{
		if (itr == str)
		{
			return true;
		}
	}
	return false;
}

/*------------------------Returns the flag with "AND" string---------------------------------*/

template<typename T>
Query<T>& Query<T>::AND()
{
	// TODO: insert return statement here
	flag = "AND";
	return *this;
}


/*------------------------Performs ADDing of queries---------------------------------*/

template<typename T>
Query<T>& Query<T>::AND(Query<T> &query1, Query<T> &query2)
{
	std::vector<std::string> keys_1 = query1.keys_;
	std::vector<std::string> keys_2 = query2.keys_;
	keys_.clear();
	for (auto itr : keys_1)
	{
		if (contains(keys_2, itr))
		{
			keys_.push_back(itr);
		}
	}
	return *this;
}

/*------------------------Performs ORing of queries---------------------------------*/

template<typename T>
Query<T>& Query<T>::OR(Query<T>& query1, Query<T> &query2)
{
	std::vector<std::string> keys_1 = query1.keys_;
	std::vector<std::string> keys_2 = query2.keys_;

	for (auto itr : keys_1)
	{
		if (!contains(keys_, itr))
		{
			keys_.push_back(itr);
		}
	}

	for (auto iter : keys_2)
	{
		if (!contains(keys_, iter))
		{
			keys_.push_back(iter);
		}
	}
	return *this;
}

/*------------------------Returns the flag with "OR" string---------------------------------*/

template<typename T>
Query<T>& Query<T>::OR()
{
	flag = "OR";
	return *this;
}

/*---------------------Initialising the keys of the query object----------------------------*/

template<typename T>
void Query<T>::initialiseKeys()
{
	std::vector<std::string> keys = db_(keys);
	for (auto itr : keys)
	{
		keys_.push_back(itr->first);
	}
}

/*---------------------Comparing the dates----------------------------*/

template<typename T>
bool Query<T>::compareDate()
{
	DbElement<Payload> dbelem1;
	dbelem1.dateTime(DateTime().now());
	DateTime &d1 = dbelem1.dateTime();   // returns today's timestamp
	DbElement<Payload> dbelem2;
	dbelem2.dateTime(DateTime().makeTime(2016, 01, 3, 4, 5, 6));
	DateTime &d2 = dbelem2.dateTime();    // returns 2016 timestamp
	std::cout << "\n Keys in the range of 2016 and present" << std::endl;
	bool flag = false;
	for (auto itr : db_.keys()) {
		if (d1.operator> (db_[itr].dateTime()) && d2.operator< (db_[itr].dateTime())) {
			std::cout << itr << std::endl;
			flag = true;
		}
	}
	std::cout << std::endl;
	DbElement<Payload> dbelem3;
	dbelem3.dateTime(DateTime().makeTime(2017, 01, 3, 4, 5, 6));
	DateTime &d3 = dbelem3.dateTime();   // returns today's timestamp
	DbElement<Payload> dbelem4;
	dbelem4.dateTime(DateTime().makeTime(2016, 01, 3, 4, 5, 6));
	DateTime &d4 = dbelem4.dateTime();    // returns 2016 timestamp
	std::cout << "\n Keys in the range of 2016 and 2017" << std::endl;
	for (auto itr : db_.keys()) {
		if (d3.operator> (db_[itr].dateTime()) && d4.operator< (db_[itr].dateTime())) {
			std::cout << itr << std::endl;
			flag = true;
		}
	}
	return flag;
}

/*-----------------Comparing the Payload--------------------- */
template<typename T>
Query<T>& Query<T>::comparePayload(Conditions<T>& cond)
{
	keys_.clear();
	for (auto itr : db_.keys()) {
		if(db_[itr].payLoad().compare(cond.getPayload()))
		{
			keys_.push_back(itr);
		}
	}

	return *this;

}