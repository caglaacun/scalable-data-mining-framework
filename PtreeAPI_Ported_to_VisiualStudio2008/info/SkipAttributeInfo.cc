// 
// File:   SkipAttributeInfo.cc
// Author: serazi
//
// Created on October 22, 2003, 2:18 PM
//

#include "SkipAttributeInfo.h"

//
// Constructor
///
SkipAttributeInfo::SkipAttributeInfo()
	:AttributeInfo()
{
}

//
// Destructor
//
SkipAttributeInfo::~SkipAttributeInfo()
{
}

int SkipAttributeInfo::type()const
{
	return Type::SKIP;
}

boost::dynamic_bitset<> SkipAttributeInfo::encode(Item* val) 
            throw (OutOfDomainException, InvalidTypeException)
{
	return boost::dynamic_bitset<>();
}

Item* SkipAttributeInfo::decode(const boost::dynamic_bitset<>& bits) 
            throw (InvalidLengthException)
{
	return new SkipItem;
}
