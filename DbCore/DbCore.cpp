/////////////////////////////////////////////////////////////////////
// DbCore.cpp - Test Stub for DbCore          //
// ver 1.0                                                         //
// Source : Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018    
// Author : Chetali Mahore
/////////////////////////////////////////////////////////////////////

#include "DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include "../Test/Payload.h"

using namespace NoSqlDb;

//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

///////////////////////////////////////////////////////////////////////
// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument.

class DbProvider
{
public:
	DbCore<std::string>& db() { return db_; }
private:
	static DbCore<std::string> db_;
};

DbCore<std::string> DbProvider::db_;

///////////////////////////////////////////////////////////////////////
// test functions

//----< demo requirement #1 >------------------------------------------

bool testR1a()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions.";
	putLine();
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------

bool testR2a()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine();
	return true;
}

//----< demo first part of requirement #3 >----------------------------

bool testR3aa()
{
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n  Creating a db element with key \"Fawcett\":";

	// create database to hold std::string payload

	DbCore<std::string> db;
	DbProvider dbp;
	dbp.db() = db;

	// create some demo elements and insert into db

	DbElement<std::string> demoElem;

	demoElem.name("Jim");
	demoElem.descrip("Instructor for CSE687");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad("The good news is ...");

	if (demoElem.name() != "Jim")
		return false;
	if (demoElem.descrip() != "Instructor for CSE687")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	if (demoElem.payLoad() != "The good news is ...")
		return false;

	showHeader();
	showElem(demoElem);

	db["Fawcett"] = demoElem;
	dbp.db() = db;
	putLine();
	return true;
}
//----< demo second part of requirement #3 >---------------------------

bool testR3ba()
{
	Utilities::title("Demonstrating Requirement #3b - creating DbCore");
	DbProvider dbp;
	DbCore<std::string> db = dbp.db();
	DbElement<std::string> demoElem = db["Fawcett"];
	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	demoElem.payLoad("You should try ...");
	db["Salman"] = demoElem;
	if (!db.contains("Salman"))
		return false;
	demoElem.name("Jianan");
	demoElem.payLoad("Dr. Fawcett said ...");
	db["Sun"] = demoElem;
	demoElem.payLoad("You didn't demonstrate Requirement #4");
	demoElem.name("Nikhil");
	db["Prashar"] = demoElem;
	demoElem.payLoad("You didn't demonstrate Requirement #5");
	demoElem.name("Pranjul");
	db["Arora"] = demoElem;
	demoElem.payLoad("You didn't demonstrate Requirement #6");
	demoElem.name("Akash");
	db["Anjanappa"] = demoElem;
	std::cout << "\n  after adding elements with keys: ";
	DbCore<std::string>::Keys keys = db.keys();
	showKeys(db);
	putLine();
	std::cout << "\n  make all the new elements children of element with key \"Fawcett\"";
	db["Fawcett"].children().push_back("Salman");
	db["Fawcett"].children().push_back("Sun");
	db["Fawcett"].children().push_back("Prashar");
	db["Fawcett"].children().push_back("Arora");
	db["Fawcett"].children().push_back("Anjanappa");
	showHeader();
	showElem(db["Fawcett"]);
	db["Salman"].children().push_back("Sun");
	db["Salman"].children().push_back("Prashar");
	db["Salman"].children().push_back("Arora");
	db["Salman"].children().push_back("Anjanappa");
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db); 	putLine();
	std::cout << "\n  database keys are: ";
	showKeys(db); 	putLine();
	return true;
}

//----< demo first part of requirement #4 >---------------------------

bool testR4a()
{
	Utilities::title("Demonstrating Requirement #4a - Addition of key/value pair i.e. Adding key 'Mahore' to the database \n ");
	DbProvider dbp;
	DbCore<std::string> db = dbp.db();
	DbCore<std::string>::DbStore &dbmap = db.dbStore();
	DbElement<std::string> demoElem;
	std::vector<std::string> categoryList = { "Category1", "Category2" };
	demoElem.name("Chetali");
	demoElem.descrip("Student");
	demoElem.payLoad("The good news is..");
	demoElem.dateTime(DateTime().now());
	if (demoElem.name() != "Chetali")
		return false;
	if (demoElem.descrip() != "Student")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	if (demoElem.payLoad() != "The good news is..")
		return false;
	showHeader();
	showElem(demoElem);
	db["Mahore"] = demoElem;
	dbp.db() = db;   // db is referencing to db_ , updating changes to the database
	putLine();
	std::cout << "\n  database keys are: ";
	showKeys(db);
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();
	Utilities::title("Demonstrating Requirement #4b - Deletion of key/value pair i.e deleting key 'Arora' from children vectors of all keys and deleting the key 'Arora'\n ");
	std::string s = "Arora";
	bool flag = db.deletePair(s);
	dbp.db() = db;
	putLine();
	std::cout << "\n  database keys are: ";
	showKeys(db);
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	return true;
}


//----< demo of requirement #5 >----------------------------

bool test5a()
{
	std::string s1 = "Fawcett", s2 = "Mahore";
	std::string name = "Ashok", desc = "Father", payload = "He is a great man";
	DbProvider dbp;
	DbCore<std::string> db = dbp.db();
	bool flag;
	std::vector<std::string> categoryList = { "Category1", "Category2" };
	putLine();
	Utilities::title("Demonstrating Requirement #5a - Adding the child relationship i.e. adding child 'Mahore' to 'Fawcett's' children vector \n ");

	db[s1].children().push_back(s2);
	dbp.db() = db;
	std::cout << "\n  showing the children of :" << s1;
	showElem(db[s1], std::cout);

	putLine();
	Utilities::title("Demonstrating Requirement #5b - Deleting the child relationship i.e. deleting child 'Mahore' from 'Fawcett's' children vector \n");
	flag = db[s1].deleteRelation(s2);
	dbp.db() = db;
	std::cout << "\n  showing the children of :" << s1;
	showElem(db[s1], std::cout);

	putLine();
	Utilities::title("Demonstrating Requirement #5c - Editing the metadata of 'Mahore' key \n ");
	flag = db[s2].editData(name, desc, payload);
	dbp.db() = db;
	std::cout << "\n  showing the metadata of :" << s2;
	showElem(db[s2], std::cout);
	showDb(db);

	putLine();
	Utilities::title("Demonstrating Requirement #5d - Replacing an existing value's instance with a new instance \n ");
	DbElement<std::string> dbelem;
	dbelem.name("Chetali");
	dbelem.descrip("Student");
	dbelem.dateTime(DateTime().now());
	flag = db.editElement(s2, dbelem);
	dbp.db() = db;
	std::cout << "\n  showing the value of :" << s2 << "element after replacement \n ";
	showElem(db[s2], std::cout);

	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	return true;
}

//----< test stub >----------------------------------------------------

#ifdef TEST_DBCORE

using namespace Utilities;

int main()
{
	Utilities::Title("Testing DbCore - He said, she said database");
	putLine();

	TestExecutive ex;

	// define test structures with test function and message

	TestExecutive::TestStr ts1{ testR1a, "Use C++11" };
	TestExecutive::TestStr ts2{ testR2a, "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ testR3aa, "Create DbElement<std::string>" };
	TestExecutive::TestStr ts3b{ testR3ba, "Create DbCore<std::string>" };
	TestExecutive::TestStr ts4b{ testR4a, "Adding and deleting key/value pairs" };
	TestExecutive::TestStr ts5{ test5a, "Adding and deleting the relationships" };

	// register test structures with TestExecutive instance, ex

	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts4b);
	ex.registerTest(ts5);

	// run tests

	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";

	putLine(2);

	getchar();
	return 0;
}
#endif
