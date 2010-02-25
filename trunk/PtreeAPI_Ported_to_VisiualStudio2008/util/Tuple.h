// 
// File:   Tuple.h
// Author: serazi
//
// Created on October 9, 2003, 3:43 PM
//

#ifndef _Tuple_H_
#define _Tuple_H_

#include <iostream>
#include <vector>
#include <string>
#include <boost/dynamic_bitset.hpp>
#include "Item.h"

using namespace std;
/** 
* Tuple is a set of Item s. Is used as the collection of items that was extracted
* from one row of data across the files
* i.e a record 
*/
class Tuple{
public:
	typedef vector<Item*>::size_type size_type;
	typedef vector<Item*>::const_iterator tuple_iterator;

	/**
	* The default constructor
	*/
	Tuple();

	/**
	* The copy constructor
	*/
	Tuple(const Tuple& other);

	/**
	* The assignment operator
	*/
	Tuple& operator=(const Tuple& other);

	/**
	* The destructor
	*/
	~Tuple();

	/**
	* Add an item to the tuple
	* @param itm pointer of Item
	* @see Item 
	*/ 
	void add(Item* const itm); 

	/**
	* Serialize the tuple to an ostreame object
	* @param os ostream object
	*/
	ostream& serialize(ostream& os)const; 

	/**
	* Get the jth bit value of ith item
	*/
	bool get(int i, int j)const;

	/**
	* Get the ith item
	* @return pointer of item
	*/ 
	Item* get(int i)const;

	/**
	* Get number of items in the tuple
	* @return number of items
	*/ 
	size_type degree()const;

	/**
	* Get the constant iterator that points to the first item
	*/ 
	tuple_iterator begin() const;

	/**
	* Get the constant iterator that points to one pass the last item
	*/ 
	tuple_iterator end()const;

private:
	vector<Item*> items;
};

ostream& operator<<(ostream&os, const Tuple& t);
#endif	/* _Tuple_H */

