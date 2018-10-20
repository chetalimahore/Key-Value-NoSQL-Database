/////////////////////////////////////////////////////////////////////
// Test.cpp - Provides Test suite for the queries               //
// ver 1.0                                                         //
// Author - Chetali Mahore, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////
/*
Package Operations:
* ------------
*
*  The test package contains functions that demonstrate the requirements. 
*  Each of the function is a requirement.
* 
*
* Required Files:
* ---------------
* Test.h, Query.h, DbCore.h
* StringUtilities.h, TestUtilties.h
* PersistenceXml.h, XmlDocument.h
* Payload.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 29th Jan 2018
*/

#include <iostream>
#include <iomanip>
#include <functional>
#include <limits.h>
#include <stdlib.h>
#include "../Test/Test.h"
#include "../Query/Query.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../PersistenceXml/PersistenceXml.h"
#include "../../XmlDocument/XmlDocument/XmlDocument.h"
#include "Payload.h"

using namespace NoSqlDb;
using namespace Utilities;
using namespace XmlProcessing;
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
	DbCore<Payload>& db() { return db_; }
private:
	static DbCore<Payload> db_;
};

DbCore<Payload> DbProvider::db_; // defining static variable

									 ///////////////////////////////////////////////////////////////////////
									 // test functions

//----< demo requirement #1 and 2>------------------------------------------

bool testR1_2()
{
	Utilities::title("Demonstrating Requirement #8a - Restoring the database from an existing XML file \n ");
	Persistence<Payload> persist;
	std::string path = "../Database.xml";
	

	XmlDocument xdoc(path, XmlDocument::file);
	
	DbCore<Payload> db;
	DbProvider dbp;
	db = persist.readXml(xdoc, path);
	dbp.db() = db;
	showDb(db); std::cout<<std::endl;
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function,	"
		<< "\n  is only valid for C++11 and later versions.";
	std::cout << "\n  passed -- \" Use C++11 \" ";
	putLine();

	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine();

	return true; // would not compile unless C++11
}

//----< demo first part of requirement #3 >----------------------------


bool testR3a()
{
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n  Creating a db element with key \"Fawcett\":";

	// create database to hold std::string payload

	DbProvider dbp;
	DbCore<Payload> db = dbp.db();  //reference to the db

					 // create some demo elements and insert into db

	DbElement<Payload> demoElem;
	std::vector<std::string> categoryList = { "Category3", "Category4" };

	demoElem.name("Jim");
	demoElem.descrip("Instructor for CSE687");
	//demoElem.dateTime(DateTime().now());
	demoElem.dateTime(DateTime::makeTime(2016,3,5,5,4,3));
	demoElem.payLoad().setPathName("../../filepath.xml");
	demoElem.payLoad().setCategory(categoryList);


	if (demoElem.name() != "Jim")
		return false;
	if (demoElem.descrip() != "Instructor for CSE687")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	if (demoElem.payLoad().getPathName() != "../../filepath.xml")
		return false;
	if (demoElem.payLoad().getCategory() != categoryList)
		return false;
	showHeader();
	showElem(demoElem);

	db["Fawcett"] = demoElem;
	dbp.db() = db;
	putLine();
	return true;
}

//----< demo second part of requirement #3 >---------------------------


bool testR3b()
{
	Utilities::title("Demonstrating Requirement #3b - creating DbCore");
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	std::vector<std::string> categoryList = { "Category1", "Category2" };
	DbElement<Payload> demoElem = db["Fawcett"];
	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	demoElem.dateTime(DateTime::makeTime(2013, 5, 4, 2, 8, 9));
	demoElem.payLoad().setPathName("../../Ammar.xml");
	demoElem.payLoad().setCategory(categoryList);
	db["Salman"] = demoElem;
	if (!db.contains("Salman"))
		return false;
	demoElem.name("Jianan");
	demoElem.payLoad().setPathName("../../Jianan.xml");
	demoElem.dateTime(DateTime::makeTime(2016, 3, 5, 5, 4, 3));
	demoElem.payLoad().setCategory(categoryList);
	db["Sun"] = demoElem;
	demoElem.payLoad().setPathName("../../Nikhil.xml");
	demoElem.payLoad().setCategory(categoryList);
	demoElem.name("Nikhil");
	db["Prashar"] = demoElem;
	demoElem.payLoad().setPathName("../../Pranjul.xml");
	demoElem.payLoad().setCategory(categoryList);
	demoElem.name("Pranjul");
	db["Arora"] = demoElem;
	demoElem.payLoad().setPathName("../../Akash.xml");
	demoElem.dateTime(DateTime::makeTime(2018, 3, 5, 5, 4, 3));
	demoElem.payLoad().setCategory(categoryList);
	demoElem.name("Akash");
	db["Anjanappa"] = demoElem;
	std::cout << "\n  after adding elements with keys: ";
	DbCore<std::string>::Keys keys = db.keys();
	showKeys(db);	putLine();
	std::cout << "\n  make all the new elements children of element with key \"Fawcett\"";
	db["Fawcett"].children().push_back("Salman");	db["Fawcett"].children().push_back("Sun"); 	db["Fawcett"].children().push_back("Prashar"); 	db["Fawcett"].children().push_back("Arora");	db["Fawcett"].children().push_back("Anjanappa");
	showHeader();
	showElem(db["Fawcett"]);
	db["Salman"].children().push_back("Sun");	db["Salman"].children().push_back("Prashar"); 	db["Salman"].children().push_back("Arora"); 	db["Salman"].children().push_back("Anjanappa"); 
	dbp.db() = db;	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db); putLine();
	std::cout << "\n  database keys are: ";
	showKeys(db);	putLine();
	return true;
}


//----< demo of requirement #4 >----------------------------


bool testR4()
{
	Utilities::title("Demonstrating Requirement #4a - Addition of key/value pair i.e. Adding key 'Tendulkar' to the database \n ");
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	DbCore<Payload>::DbStore &dbmap = db.dbStore();
	DbElement<Payload> demoElem;
	std::vector<std::string> categoryList = { "Category3", "Category4" };
	demoElem.name("Sachin");
	demoElem.descrip("Cricketer");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad().setPathName("../../Sachin.xml");
	demoElem.payLoad().setCategory(categoryList);
	if (demoElem.name() != "Sachin")
		return false;
	if (demoElem.descrip() != "Cricketer")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	if (demoElem.payLoad().getPathName() != "../../Sachin.xml")
		return false;
	if (demoElem.payLoad().getCategory() != categoryList)
		return false;
	showHeader();
	showElem(demoElem);
	db["Tendulkar"] = demoElem;
	demoElem.descrip("Actor");
	demoElem.payLoad().setPathName("../../Farhan.xml");
	demoElem.name("Akhtar");
	db["Farhan"] = demoElem;
	db["Farhan"].children().push_back("Salman");
	db["Farhan"].children().push_back("Sun");
	dbp.db() = db;   // db is referencing to db_ , updating changes to the database
	putLine();
	std::cout << "\n  database keys are: ";
	showKeys(db);
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);	putLine();
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

bool test5()
{
	std::string s1 = "Fawcett", s2 = "Mahore", s3 = "Tendulkar";
	std::string name = "Ashok", desc = "Father";
	Payload payload;
	payload.setPathName("../../Ashok.xml");
	payload.setCategory({"Category 2", "Category 3"});
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
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
	Utilities::title("\n  showing all the database elements:");
	showDb(db);
	putLine();
	Utilities::title("Demonstrating Requirement #5d - Replacing an existing value's instance with a new instance \n ");
	DbElement<Payload> dbelem;
	dbelem.name("Rohan");
	dbelem.descrip("Brother");
	dbelem.dateTime(DateTime().now());
	dbelem.payLoad().setPathName("../../Rohan.xml");
	dbelem.payLoad().setCategory(categoryList);
	flag = db.editElement(s3, dbelem);
	dbp.db() = db;
	std::cout << "\n  showing the value of :"<< s3 <<" element after replacement \n ";
	showElem(db[s3], std::cout);
	putLine();
	std::cout << "\n  showing all the database elements:";
	showDb(db);
	return true;
}


//----< demo of requirement #6 >----------------------------


bool test6()
{
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	Query<Payload> query(db);
	putLine();
	Utilities::title("Demonstrating Requirement #6a - Retrieving value of a specified key by passing Regex (F)(.*) \n");
	std::cout << "\n Value of key 'Fawcett' : " << std::endl;
	Conditions<Payload> cond1;
	cond1.setKey("(F)(.*)");
	query.getValueByKey(cond1).show("Value");
	putLine();

	Utilities::title("Demonstrating Requirement #6b - Retrieving child of a specified key by passing Regex (F)(.*) \n");
	Conditions<Payload> cond2;
	cond2.setKey("(F)(.*)");
	query.getChildren(cond2);
	putLine();

	Utilities::title("Demonstrating Requirement #6c - Retrieving set of keys by passing Regex (F)(.*) \n");
	putLine();
	std::cout << "\n Set of keys with regex '(F)(.*)' : \n" << std::endl;
	Conditions<Payload> cond3;
	cond3.setKey("(F)(.*)");
	query.getKeys(cond3).show("Keys");
	putLine();

	Utilities::title("Demonstrating Requirement #6d - Retrieving set of keys by passing Regex (A)(.*) \n");
	putLine();
	std::cout << "\n Set of keys with regex '(A)(*)' : \n" << std::endl;
	Conditions<Payload> cond4;
	cond4.setName("(A)(.*)");
	query.getName(cond4).show("Keys");
	putLine();

	Utilities::title("Demonstrating Requirement #6e - Retrieving set of keys by passing Regex (TA)(.*) \n");
	putLine();
	std::cout << "\n All keys with specific description in the form of regex '(TA)(.*)' : \n" << std::endl;
	Conditions<Payload> cond5;
	cond5.setDesc("(TA)(.*)");
	query.getDescrip(cond5).show("Keys");
	putLine();
	Utilities::title("Demonstrating Requirement #6f - Retrieving set of keys based within a specified time-date interval \n ");
	putLine();
	bool flag = query.compareDate();
	if (flag == false) { std::cout << "No key is in that range"; }
	return true;
}


//----< demo of requirement #7 >----------------------------


bool test7()
{
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	Query<Payload> query1(db);
	Query<Payload> query2(db);
	Query<Payload> query3(db);
	Query<Payload> query4(db);
	putLine();
	Utilities::title("Demonstrating Requirement #7a - ANDing of condition : Desc - (TA)(.*) , Name - (A)(.*)" );
	putLine();
	Conditions<Payload> cond1;
	Conditions<Payload> cond2;
	cond1.setDesc("(TA)(.*)"); 
	cond2.setName("(A)(.*)");
	query1.getDescrip(cond1).AND().getName(cond2).show("AND");

	Utilities::title("Demonstrating Requirement #7b - ORing of condition :Desc - (TA)(.*) , Desc - (I)(.*) ");
	putLine();
	cond1.setDesc("(TA)(.*)");
	cond2.setDesc("(I)(.*)");
	query2.getDescrip(cond1).OR().getDescrip(cond2).show("OR");

	Utilities::title("Demonstrating Requirement #7c - ANDing of queries : ANDing of 7a and 7b query result");
	putLine();
	query1.AND(query1, query2).show("AND");

	Utilities::title("Demonstrating Requirement #7d - ORing of queries: ORing of 7a and 7b query result");
	putLine();
	query1.OR(query1, query2).show("OR");

	cond1.setName("(A)(.*)");
	cond2.setDesc("(I)(.*)");
	query3.getName(cond1).OR().getDescrip(cond2);

	Utilities::title("Demonstrating Requirement #7e -  ORing, ANDing of queries : 7c and 7d results are first ORed and then ANDed with query - Name(A)(.*), Desc(I)(.*)\n ");
	putLine();
	query1.OR(query1, query2).AND(query2, query3).show("NONE");

	Utilities::title("Demonstrating Requirement #7f - Query on Payload .i.e. Return the set of keys who have category1 in their payload\n");
	std::cout << std::endl;
	Payload payload;
	payload.setPathName("../../Chetali.xml");
	std::vector<std::string> category = { "Category1"};
	payload.setCategory(category);
	Conditions<Payload> cond5;
	cond5.setPayload(payload);
	Query<Payload> query5(db);
	query5.comparePayload(cond5).show("Keys");
	return true;
}


//----< demo of requirement #8 and 9 >----------------------------


bool test8_9()
{
	Utilities::title("Demonstrating Requirement #8b - Augmenting the database from an XML file \n ");
	Persistence<Payload> persist;
	std::string path = "../Augment.xml";
	XmlDocument xdoc(path, XmlDocument::file);
	DbCore<Payload> dbcore_;
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	dbcore_ = persist.readXml(xdoc, path);

	for (auto itr : dbcore_.keys())
	{
		if (!db.contains(itr))
		{
			db[itr] = dbcore_[itr];
		}
	}
	std::cout << "\n  showing all the database elements:";
	showDb(db);

	dbp.db() = db;

	Utilities::title("Demonstrating Requirement #8c - Persisting the database to an XML file \n");
	std::string filepath = "../PersistDatabase.xml";
	db = dbp.db();
	persist.writeXml(db, filepath);

	std::cout << "\n  passed -- \"Restoring, Augmenting and Persisting Database into XML \" \n";

	putLine();
	Utilities::Title("Demonstrating Requirement #9 - Implemented a Payload class that contains filepath and filecategories \n ");
	std::cout << "\n The files are : Payload.h , Payload.cpp \n";
	std::cout << "\n The path to these files are : ../Payload \n";

	return true;
}

//----< demo of requirement #10 and 11 >----------------------------

bool test10_11()
{
	
	Utilities::title("Demonstrating Requirement #10 - Implemented packages - Executive, DbCore, Query and Test \n");
	putLine();
	std::cout << "\n  passed -- \" Implemented Executive, DBCore, Query, Test packages \" \n ";


	Persistence<Payload> persist;
	Utilities::title("Demonstrating Requirement #11 - Displaying the Project Dependency after reading from XML file \n");
	std::string path = "../ProjectDependency.xml";
	std::cout << "\n File path of Xml file displaying the project dependency : " << path<< std::endl; 
	XmlDocument xdoc(path, XmlDocument::file);
	DbCore<Payload> dbcore_;
	DbProvider dbp;
	DbCore<Payload> db = dbp.db();
	dbcore_ = persist.readXml(xdoc, path);
	showDb(dbcore_);
	
	putLine();
	std::cout << "\n--------- Final Database ---------- \n" << std::endl;
	showDb(db);
	return true;
}


void TestClass :: execute()
{
	Utilities::Title("Testing DbCore ");
	putLine();

	TestExecutive ex;

	// define test structures with test function and message

	//testR1, testR2 are function pointers

	TestExecutive::TestStr ts1_2{ testR1_2, " Use streams and new and delete" };    
	TestExecutive::TestStr ts3a{ testR3a, "Create DbElement<Payload>" };
	TestExecutive::TestStr ts3b{ testR3b, "Create DbCore<std::string>" };
	TestExecutive::TestStr ts4b{ testR4, "Adding and deleting key/value pairs" };
	TestExecutive::TestStr ts5{ test5, "Adding and deleting the relationships" };
	TestExecutive::TestStr ts6{ test6, "Retrieving data based on a specified key" };
	TestExecutive::TestStr ts7{ test7, "Nested Queries" };
	TestExecutive::TestStr ts8_9{ test8_9, " Created a Payload class that contains filepath and filecategories " };
	TestExecutive::TestStr ts10_11{ test10_11, " Displaying the project's package structure and dependency relationships" };

	// register test structures with TestExecutive instance, ex

	ex.registerTest(ts1_2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts4b);
	ex.registerTest(ts5);
	ex.registerTest(ts6);
	ex.registerTest(ts7);
	ex.registerTest(ts8_9);
	ex.registerTest(ts10_11);
			// run tests

	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";

	putLine(2);
}
//#endif
