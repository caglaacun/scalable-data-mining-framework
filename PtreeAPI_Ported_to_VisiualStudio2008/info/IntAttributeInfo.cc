// 
// File:   IntAttributeInfo.cc
// Author: serazi
//
// Created on October 13, 2003, 12:42 PM
//

#include "IntAttributeInfo.h"

//
// Default Constructor
///
IntAttributeInfo::IntAttributeInfo() : lower_(0), upper_(0){}

//
// Constructor
///
IntAttributeInfo::IntAttributeInfo(int lower, int upper) 
: lower_(lower), upper_(upper){}
//
// Destructor
//
IntAttributeInfo::~IntAttributeInfo()
{
}

int IntAttributeInfo::upper()const{
    return upper_;
}

int IntAttributeInfo::lower()const{
    return lower_;
}
