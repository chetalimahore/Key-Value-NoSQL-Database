//
/////////////////////////////////////////////////////////////////////
// Executive.cpp -  Defines the entry point for the console application          //
// ver 1.0																		//
// Author : Chetali Mahore, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////
/*
* The package is the starting project. It creates an object of TestClass which executes all the test functions.
*
* Required Files:
* ---------------
* Test.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 18th Jan 2018
*/

#include "../Test/Test.h"
#include <iostream>
#include <iomanip>
#include <functional>

using namespace NoSqlDb;

#if TEST_EXECUTIVE
int main()
{
	TestClass test;
	test.execute();
	getchar();
    return 0;
}

#endif
