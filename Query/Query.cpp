
//
/////////////////////////////////////////////////////////////////////
// Query.cpp - Query Engine - Test stub for query				//
// Version - ver 1.0                                                        //
// Author - Chetali Mahore, CSE687 - Object Oriented Design, Spring 2018    //
/////////////////////////////////////////////////////////////////////

#include "Query.h"
#include "../Test/Payload.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"

#if TEST_QUERY
int main()
{
	DbCore<Payload> db ;

	std::vector<std::string> categoryList = { "Category1", "Category2" };
	DbElement<Payload> demoElem = db["Fawcett"];

	demoElem.name("Jim");
	demoElem.descrip("Instructor for CSE687");
	//demoElem.dateTime(DateTime().now());
	demoElem.dateTime(DateTime::makeTime(2016, 3, 5, 5, 4, 3));
	demoElem.payLoad().setPathName("../../filepath.xml");
	demoElem.payLoad().setCategory(categoryList);

	db["Fawcett"] = demoElem;

	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	demoElem.dateTime(DateTime::makeTime(2013, 5, 4, 2, 8, 9));
	demoElem.payLoad().setPathName("../../filepathAmmar.xml");
	demoElem.payLoad().setCategory(categoryList);
	db["Salman"] = demoElem;
	if (!db.contains("Salman"))
		return false;

	demoElem.name("Jianan");
	demoElem.payLoad().setPathName("../../filepathJianan.xml");
	demoElem.dateTime(DateTime::makeTime(2016, 3, 5, 5, 4, 3));
	demoElem.payLoad().setCategory(categoryList);
	db["Sun"] = demoElem;

	demoElem.payLoad().setPathName("../../filepathNikhil.xml");
	demoElem.dateTime(DateTime::makeTime(2016, 3, 5, 5, 4, 3));
	demoElem.payLoad().setCategory(categoryList);
	demoElem.name("Nikhil");
	db["Prashar"] = demoElem;

	demoElem.payLoad().setPathName("../../filepathPranjul.xml");
	demoElem.dateTime(DateTime::makeTime(2016, 3, 5, 5, 4, 3));
	demoElem.payLoad().setCategory(categoryList);
	demoElem.name("Pranjul");
	db["Arora"] = demoElem;

	demoElem.payLoad().setPathName("../../filepathAkash.xml");
	demoElem.dateTime(DateTime::makeTime(2016, 3, 5, 5, 4, 3));
	demoElem.payLoad().setCategory(categoryList);
	demoElem.name("Akash");
	db["Anjanappa"] = demoElem;

	db["Fawcett"].children().push_back("Salman");
	db["Fawcett"].children().push_back("Sun");
	db["Fawcett"].children().push_back("Prashar");
	db["Fawcett"].children().push_back("Arora");
	db["Fawcett"].children().push_back("Anjanappa");

	db["Salman"].children().push_back("Sun");
	db["Salman"].children().push_back("Prashar");
	db["Salman"].children().push_back("Arora");
	db["Salman"].children().push_back("Anjanappa");

	Query<Payload> query(db);

	Utilities::title("Demonstrating Requirement #6a - Retrieving value of a specified key \n ");
	std::cout << "\n Value of key 'Fawcett' : " << std::endl;
	Conditions<Payload> cond1;
	cond1.setKey("(F)(.*)");
	query.getValueByKey(cond1).show("Value");

	Utilities::title("Demonstrating Requirement #6b - Retrieving child of a specified key \n");
	std::cout << "\n Children of key 'Fawcett' \n" << std::endl;
	Conditions<Payload> cond2;
	cond2.setKey("(F)(.*)");
	query.getChildren(cond2).show("Child");

	Utilities::title("Demonstrating Requirement #6c - Retrieving set of keys based on a specific regex expression \n");

	std::cout << "\n Set of keys with regex '(F)(.*)' : \n" << std::endl;
	Conditions<Payload> cond3;
	cond3.setKey("(F)(.*)");
	query.getKeys(cond3).show("Keys");

	Utilities::title("Demonstrating Requirement #6d - Retrieving set of keys based with a specific string in their name \n ");

	std::cout << "\n Set of keys with regex '(A)(*)' : \n" << std::endl;
	Conditions<Payload> cond4;
	cond4.setName("(A)(.*)");
	query.getName(cond4).show("Name");

	Utilities::title("Demonstrating Requirement #6e - Retrieving set of keys based with a specific string in their description \n");

	std::cout << "\n All keys with specific description in the form of regex '(TA)(.*)' : \n" << std::endl;
	Conditions<Payload> cond5;
	cond5.setDesc("(TA)(.*)");
	query.getDescrip(cond5).show("Descrip");
	
	Utilities::title("Demonstrating Requirement #6f - Retrieving set of keys based within a specified time-date interval \n ");
	bool flag = query.compareDate();
	if (flag == false) { std::cout << "No key is in that range"; }
	getchar();
	return true;
}
#endif // TEST_QUERY
