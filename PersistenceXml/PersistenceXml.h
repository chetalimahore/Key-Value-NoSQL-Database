/////////////////////////////////////////////////////////////////////
// Persistence.h - Provides functionalities of reading and writing the contents to XML file         //
// ver 1.0                                                                     //
// Author - Chetali Mahore, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a class:
* Persistence :
* - readXml() - reading the XML and storing it in the database object
* - setElement() - setting the value of the DbElement corresponding to the key
* - setPayloadType() - setting the payload of the DbElement corresponding to the key
* - setChild() - setting the child vector of the DbElement corresponding to the key
* - writeXml() - writing the contents of the database into an Xml file

* Required Files:
* ---------------
* XmlParser.h 
* XmlDocument.h
* DbCore.h
* Payload.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 27th Jan 2018
*/


#include<iostream>
#include "../../XmlDocument/XmlParser/XmlParser.h"
#include "../../XmlDocument/XmlDocument/XmlDocument.h"
#include "../DbCore/DbCore.h"
#include "../Test/Payload.h"

using namespace NoSqlDb;
using namespace XmlProcessing;

template<typename T>
class Persistence
{
public:
	DbCore<T> readXml(XmlDocument &xdoc, std::string filepath);
	void setElement(std::string &key, std::vector<std::shared_ptr<AbstractXmlElement>> pChildren, DbElement<T>& elem);
	void setPayloadType(std::shared_ptr<AbstractXmlElement> pValueChild, DbElement<T>& elem);
	void setChild(std::shared_ptr<AbstractXmlElement> pValueChild, DbElement<T>& elem);
	void writeXml(DbCore<T> dbcore, std::string filepath);
};

/*---------------------------reading the XML and storing it in the database object-----------------*/

template<typename T>
inline DbCore<T> Persistence<T>::readXml(XmlDocument &xdoc, std::string filepath) {
	std::cout << " \n Reading the file from the path: " << filepath <<"\n"<< std::endl;
	DbCore<T> dbcore;
	using Key = std::string;
	using Sptr = std::shared_ptr<AbstractXmlElement>;
	std::vector<Sptr> records = xdoc.descendents("dbrecord").select();
	for (auto pRecord : records) {
		Key key;
		DbElement<T> elem;
		std::vector<Sptr> pChildren = pRecord->children();
		setElement(key, pChildren, elem);
		dbcore[key] = elem;
	}
	std::cout << "Database : \n" <<  std::endl;
	return dbcore;
}

/*--------------------------setting the value of the DbElement corresponding to the key-------------------*/

template<typename T>
inline void Persistence<T>::setElement(std::string &key, std::vector<std::shared_ptr<AbstractXmlElement>> pChildren, DbElement<T>& elem) {
	using Sptr = std::shared_ptr<AbstractXmlElement>;
	for (auto pChild : pChildren) {
		Payload pay;
		if (pChild->tag() == "key") { key = pChild->children()[0]->value(); }
		else {
			std::vector<Sptr> pValueChildren = pChild->children();
			for (auto pValueChild : pValueChildren) {
				if (pValueChild->tag() == "name") { elem.name(pValueChild->children()[0]->value()); }
				if (pValueChild->tag() == "description") { elem.descrip(pValueChild->children()[0]->value()); }
				if (pValueChild->tag() == "DateTime") {
					if (pValueChild->children().size() > 0) {
						elem.dateTime(pValueChild->children()[0]->value());
					}
					else elem.dateTime(DateTime().now());
				}
				setPayloadType(pValueChild, elem);
				setChild(pValueChild, elem);
				

			}
		}
	}
}

/*-----------------setting the payload of the DbElement corresponding to the key---------------------*/

template<typename T>
inline void Persistence<T>::setPayloadType(std::shared_ptr<AbstractXmlElement> pValueChild, DbElement<T>& elem)
{
	using Sptr = std::shared_ptr<AbstractXmlElement>;
	Payload pay;
	if (pValueChild->tag() == "Payload") {
		if (pValueChild->children().size() > 0) {
			std::vector<Sptr> child_name = pValueChild->children();
			for (auto child : child_name) {
				if (child->tag() == "Path") {
					if (child->children().size() > 0) {
						pay.setPathName(child->children()[0]->value());
					}
				}
				if (child->tag() == "Category") {
					if (child->children().size() > 0) {
						std::vector<Sptr> temp_ = child->children();
						std::vector<std::string> temp;
						for (auto grandchild : temp_) {
							if (grandchild->tag() == "name") { temp.push_back(grandchild->children()[0]->value()); }
						}
						pay.setCategory(temp);
					}
				}
			}
		}
		elem.payLoad(pay);
	}
}

/*------------------------------setting the child vector of the DbElement corresponding to the key----------------*/

template<typename T>
inline void Persistence<T>::setChild(std::shared_ptr<AbstractXmlElement> pValueChild, DbElement<T>& elem)
{
	using Sptr = std::shared_ptr<AbstractXmlElement>;
	if (pValueChild->tag() == "Children") {
		std::vector<Sptr> child_name = pValueChild->children();
		for (auto itr : child_name) {
			if (itr->tag() == "name") {
				if (itr->children().size() > 0) {
					elem.children().push_back(itr->children()[0]->value());
				}
			}
		}
	}
}


/*---------------------------------writing the contents of the database into an Xml file---------------*/

template<typename T>
inline void Persistence<T>::writeXml(DbCore<T> dbcore, std::string filepath) {
	using Sptr = std::shared_ptr<AbstractXmlElement>;
	Sptr pDb = makeTaggedElement("db");
	Sptr pDocElem = makeDocElement(pDb);
	XmlDocument xDoc(pDocElem);
	for (auto itr : dbcore) {
		Sptr pRecord = makeTaggedElement("dbrecord");
		pDb->addChild(pRecord);
		Sptr pKey = makeTaggedElement("key", itr.first);
		pRecord->addChild(pKey);
		Sptr pValue = makeTaggedElement("value");
		pRecord->addChild(pValue);
		Sptr pName = makeTaggedElement("Name", itr.second.name());
		pValue->addChild(pName);
		Sptr pDescrip = makeTaggedElement("Description", itr.second.descrip());
		pValue->addChild(pDescrip);
		Sptr pDatetime = makeTaggedElement("DateTime", itr.second.dateTime());
		pValue->addChild(pDatetime);
		Sptr pPayload = makeTaggedElement("Payload");
		pValue->addChild(pPayload);
		Sptr pPath = makeTaggedElement("Path", itr.second.payLoad().getPathName());
		pPayload->addChild(pPath);
		std::vector<std::string> categoryList = itr.second.payLoad().getCategory();
		Sptr pCategory = makeTaggedElement("Category");
		pPayload->addChild(pCategory);
		for (auto itr : categoryList) {
			Sptr pCategoryName = makeTaggedElement("Name", itr);
			pCategory->addChild(pCategoryName);
		}
	}
	std::string Xml = xDoc.toString();
	std::cout << " \n Writing the file to the path: " << filepath << std::endl;
	std::ofstream outputFile(filepath);
	outputFile << Xml;
	outputFile.close();
}