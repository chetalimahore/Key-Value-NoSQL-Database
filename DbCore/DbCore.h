#pragma once
/////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database prototype                  //
// ver 1.0                                                         //
// Source: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018 
// Author: Chetali Mahore
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* - DbCore implements core NoSql database operations, but does not
*   provide editing, querying, or persisting.  Those are left as
*   exercises for students.
* - DbElement provides the value part of our key-value database.
*   It contains fields for name, description, date, child collection
*   and a payload field of the template type. 
*
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database
* - deletePair() - deleting the pair from the database
* - editElement() - editing the value of a specified key
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Jan 2018
*/

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "../DateTime/DateTime.h"

namespace NoSqlDb
{
  /////////////////////////////////////////////////////////////////////
  // DbElement class
  // - provides the value part of a NoSql key-value database

  template<typename T>
  class DbElement
  {
  public:
    using Key = std::string;
    using Children = std::vector<Key>;

    // methods to get and set DbElement fields

    std::string& name() { return name_; }
    std::string name() const { return name_; }
    void name(const std::string& name) { name_ = name; }

    std::string& descrip() { return descrip_; }
    std::string descrip() const { return descrip_; }
    void descrip(const std::string& name) { descrip_ = name; }
    
    DateTime& dateTime() { return dateTime_; }
    DateTime dateTime() const { return dateTime_; }
    void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

    Children& children() { return children_; }
    Children children() const { return children_; }
    void children(const Children& children) { children_ = children; }

    T& payLoad() { return payLoad_; }
    T payLoad() const { return payLoad_; }
    void payLoad(const T& payLoad) { payLoad_ = payLoad; }

	bool deleteRelation(Key value);
	bool editData(Key name, Key desc, T payload);

  private:
    std::string name_;
    std::string descrip_;
    DateTime dateTime_;
    Children children_;
    T payLoad_;
  };

  /*----------------To delete the relation from the database----------------------------*/

  template<typename T>
  bool DbElement<T>::deleteRelation(Key value)
  {
	  int i = 0;
	  for (std::vector<std::string>::iterator itr = children_.begin(); itr != children_.end(); ++itr, i++)
	  {
		  if (*itr == value)
		  {
			  children_.erase(children_.begin() + i);
			  return true;
		  }
	  }
	 return false;
  }

  /*---------------To edit the data of the specified key---------------------------------------*/
  template<typename T>
  bool DbElement<T>::editData(Key name, Key desc, T payload)
  {
	  this->name_ = name;
	  this->descrip_ = desc;
	  this->dateTime_ = DateTime().now();
	  this->payLoad_ = payload;
	  return true;
  }

  
  /////////////////////////////////////////////////////////////////////
  // DbCore class
  // - provides core NoSql db operations
  // - does not provide editing, querying, or persistance operations

  template <typename T>
  class DbCore
  {
  public:
    using Key = std::string;
    using Keys = std::vector<Key>;
    using Children = Keys;
    using DbStore = std::unordered_map<Key,DbElement<T>>;
    using iterator = typename DbStore::iterator;

    // methods to access database elements

    Keys keys();
    bool contains(const Key& key);
    size_t size();
    DbElement<T>& operator[](const Key& key);
    DbElement<T> operator[](const Key& key) const; // a read-only function
    typename iterator begin() { return dbStore_.begin(); }
    typename iterator end() { return dbStore_.end(); }

    // methods to get and set the private database hash-map storage

    DbStore& dbStore() { return dbStore_; } // getter with reference
    DbStore dbStore() const { return dbStore_; } // getter with value
    void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }
	bool deletePair(Key key);
	bool editElement(Key key, DbElement<T> dbelem);
	
  private:
    DbStore dbStore_; // unordered hash map
  };

  /////////////////////////////////////////////////////////////////////
  // DbCore<T> methods

  //----< does db contain this key? >----------------------------------

  template<typename T>
  bool DbCore<T>::contains(const Key& key) // checks if a key is present
  {
    iterator iter = dbStore_.find(key);
    return iter != dbStore_.end();
  }

  //----< returns current key set for db >-----------------------------

  template<typename T> // simply collects all the keys in one container and returns the container
  typename DbCore<T>::Keys DbCore<T>::keys() // returns the collection of keys
  {
    DbCore<T>::Keys dbKeys; // std::vector<string> dbkeys
    DbStore& dbs = dbStore();
    size_t size = dbs.size();
    dbKeys.reserve(size);
    for (auto item : dbs)
    {
      dbKeys.push_back(item.first);
    }
    return dbKeys;
  }
  //----< return number of db elements >-------------------------------

  template<typename T>
  size_t DbCore<T>::size()
  {
    return dbStore_.size();
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes non-const db objects
  */
  template<typename T>
  DbElement<T>& DbCore<T>::operator[](const Key& key)
  {
    if (!contains(key))
    {
      dbStore_[key] = DbElement<T>();
    }
    return dbStore_[key];  //returns the value associated with this key
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes const db objects
  */
  template<typename T>
  DbElement<T> DbCore<T>::operator[](const Key& key) const
  {
    if (!contains(key))
    {
      dbStore_[key] = DbElement<T>();
    }
    return dbStore_[key];
  }

  /*---------------To delete the key value pair from the database-----------------------*/

  template<typename T>
  bool DbCore<T>::deletePair(Key key)
  {
	  if (this->contains(key))
	  {
		  DbCore<T>::DbStore &dbmap = this->dbStore();
		  for (auto it : dbmap)
		  {
			  DbCore<T>::Children child = it.second.children();
			  int i = 0;
			  for (std::vector<std::string>::iterator itr = child.begin(); itr != child.end(); ++itr, i++)
			  {
				  if (*itr == key)
				  {
					  dbmap[it.first].children().erase(dbmap[it.first].children().begin() + i);
					  break;
				  }
			  }
		  }
		  dbmap.erase(key);
		  std::cout << "You have deleted the key successfully";
		  return true;
	  }
	  else
	  {
	      std::cout << "You cannot find the key";
	  }
	  return false;
  }

  /*-----------To replace the value of the specified key-------------------*/

  template<typename T>
  bool DbCore<T>::editElement(Key key, DbElement<T> dbelem)
  {
	  dbStore_[key] = dbelem;
	  return true;
  } 
  
  /////////////////////////////////////////////////////////////////////
  // display functions

  //----< display database key set >-----------------------------------

  template<typename T>
  void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
  {
    out << "\n  ";
    for (auto key : db.keys())
    {
      out << key << " ";
    }
  }
  //----< show record header items >-----------------------------------

  inline void showHeader(std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << "DateTime";
    out << std::setw(10) << std::left << "Name";
    out << std::setw(25) << std::left << "Description";
    out << std::setw(30) << std::left << "Payload Path";
	out << std::setw(25) << std::left << "Payload Categories";
    out << "\n  ";
    out << std::setw(26) << std::left << "------------------------";
    out << std::setw(10) << std::left << "--------";
    out << std::setw(25) << std::left << "-----------------------";
    out << std::setw(30) << std::left << "-----------------------";
	out << std::setw(25) << std::left << "-----------------------";
  }
  //----< display DbElements >-----------------------------------------

  template<typename T>
  void showElem(const DbElement<T>& el, std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << std::string(el.dateTime());
    out << std::setw(10) << std::left << el.name();
    out << std::setw(25) << std::left << el.descrip();
	out << std::setw(25) << std::left << el.payLoad();
	
    typename DbElement<T>::Children children = el.children();
    if (children.size() > 0)
    {
      out << "\n    child keys: ";
      for (auto key : children)
      {
        out << " " << key;
      }
    }

  }
  //----< display all records in database >----------------------------

  template<typename T>
  void showDb(const DbCore<T>& db, std::ostream& out = std::cout)
  {
    //out << "\n  ";
    //out << std::setw(26) << std::left << "DateTime";
    //out << std::setw(10) << std::left << "Name";
    //out << std::setw(25) << std::left << "Description";
    //out << std::setw(25) << std::left << "Payload";
    //out << "\n  ";
    //out << std::setw(26) << std::left << "------------------------";
    //out << std::setw(10) << std::left << "--------";
    //out << std::setw(25) << std::left << "-----------------------";
    //out << std::setw(25) << std::left << "-----------------------";
    showHeader(out);
    typename DbCore<T>::DbStore dbs = db.dbStore();
    for (auto item : dbs)
    {
      showElem(item.second, out);
    }
  }

}
