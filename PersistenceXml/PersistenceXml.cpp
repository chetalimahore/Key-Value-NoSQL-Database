/////////////////////////////////////////////////////////////////////
// PersistenceXml.cpp- Persistence Engine							//
// Version - ver 1.0                                                        //
// Author - Chetali Mahore, CSE687 - Object Oriented Design, Spring 2018    //
/////////////////////////////////////////////////////////////////////

#include "PersistenceXml.h"
#include<iostream>
#include "../../XmlDocument/XmlParser/XmlParser.h"
#include "../../XmlDocument/XmlDocument/XmlDocument.h"
#include "../DbCore/DbCore.h"
#include "../Test/Payload.h"

#if TEST_PERSISTENCEXML

int main()
{
	Persistence<Payload> persist;
	std::string databasepath = "../Database.xml";

	XmlDocument xdoc(databasepath, XmlDocument::file);

	DbCore<Payload> db;
	db = persist.readXml(xdoc, databasepath);

	std::string filepath = "../PersistDatabase.xml";
	persist.writeXml(db, filepath);

	showDb(db);
	getchar();
	return 0;
}
#endif // TEST_PERSISTENCEXML
