// 
// File:   DoubleAttributeInfo.cc
// Author: serazi
//
// Created on October 13, 2003, 12:42 PM
//

#include "DoubleAttributeInfo.h"
                 //DoubleAttributeInfo.h

//
// Default Constructor
///
 DoubleAttributeInfo:: DoubleAttributeInfo() : lower_(0), upper_(0){}

//
// Constructor
///
 DoubleAttributeInfo::DoubleAttributeInfo(double low, double up) 
: lower_(low), upper_(up){}
//
// Destructor
//
 DoubleAttributeInfo::~ DoubleAttributeInfo()
{
}
void DoubleAttributeInfo::setPrecision(int p){
	precision_ = p;
	setDomain(lower(), upper());
}
double DoubleAttributeInfo::upper()const{
    return upper_;
}

double DoubleAttributeInfo::lower()const{
    return lower_;
}

int DoubleAttributeInfo::precision()const{
	return precision_;
}
