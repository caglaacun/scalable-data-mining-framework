// 
// File:   OpIndex.cc
// Author: serazi
//
// Created on November 7, 2003, 12:56 PM
//

#include "OpIndex.h"

OpIndex::OpIndex(size_t index): index_(index)
{}
size_t  OpIndex::value()const
{
	return index_;
}

OpIndex::~OpIndex(){

}
////////////////////////////////////////////////
BasicIndex ::BasicIndex(size_t index) : OpIndex(index)
{}

BasicIndex ::~BasicIndex(){
    //cout<<"~BasicIndex()"<<endl;
}
int BasicIndex ::type()const{
	return OpIndex::BASIC;
}

OpIndex* BasicIndex ::clone(){
	return new BasicIndex(index_);
}	
/////////////////////////////////////////////////
DerivedIndex ::DerivedIndex(size_t index) : OpIndex(index)
{}

DerivedIndex ::~DerivedIndex(){
    //cout<<"~DerivedIndex()"<<endl;
}
int DerivedIndex ::type()const{
	return OpIndex::DRVD;
}

OpIndex* DerivedIndex ::clone(){
	return new DerivedIndex(index_);
}	
///////////////////////////////////////////////////

Operator ::Operator(size_t index) : OpIndex(index)
{}

Operator ::~Operator()
{
    //cout<<"~Operator()"<<endl;
}
int Operator ::type()const{
	return OpIndex::OPTR;
}

OpIndex* Operator ::clone(){
	return new Operator(index_);
}

