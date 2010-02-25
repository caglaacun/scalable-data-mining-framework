// 
// File:   OpIndex.h
// Author: serazi
//
// Created on November 7, 2003, 12:56 PM
//

#ifndef _OpIndex_H
#define _OpIndex_H

#include <iostream>
using namespace std;
/** 
* OpIndex is used by BasicPt and PtreeSet to identify the respective operator and operands.
* Used to build and evaluate the operator/operand stack
* BASIC = 0  Baic Ptree
* DRVD =  1  Derived Ptree
* OPTR =  2  Operator such as AND OR
*/
class OpIndex {
public:
	static const int BASIC = 0;
	static const int DRVD = 1;
	static const int OPTR = 2;
	OpIndex(size_t index);
	virtual ~OpIndex();
	std::size_t value()const;
	virtual int type() const= 0;
	virtual  OpIndex* clone() = 0;
protected:
	size_t index_;	
};

/** 
* OpIndex used by BasicPt and PtreeSet to identify Baisc Ptree
*/
class BasicIndex : public OpIndex{
public:
	BasicIndex(size_t index);
	virtual int type()const;
	virtual  OpIndex* clone();
	~BasicIndex();
};
/** 
* OpIndex used by BasicPt and PtreeSet to identify Derived Ptree
*/
class DerivedIndex : public OpIndex{
public:
	DerivedIndex(size_t index);
	virtual int type()const;
	virtual  OpIndex* clone();
	~DerivedIndex();
};

/** 
* OpIndex used by BasicPt and PtreeSet to identify Ptree Opernad                 
*/
class Operator : public OpIndex{
public:
	Operator(size_t val);
	virtual int type()const;
	virtual  OpIndex* clone();
	~Operator();
};
#endif	/* _OpIndex_H */

