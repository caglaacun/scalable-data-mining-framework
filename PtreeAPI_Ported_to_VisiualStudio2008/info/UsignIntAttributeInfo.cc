// 
// File:   UsignIntAttributeInfo.cc
// Author: Masum H. serazi
//
// Created on October 13, 2003, 12:51 PM
//

#include "UsignIntAttributeInfo.h"


//
// Constructor
///
UsignIntAttributeInfo::UsignIntAttributeInfo()
{    
}

//
// Constructor with given upper and lower bound of the domain
//
UsignIntAttributeInfo::UsignIntAttributeInfo(int lower, int upper)
{
	setDomain(lower, upper);	
}


//
// Destructor
//
UsignIntAttributeInfo::~UsignIntAttributeInfo()
{
    
}


int UsignIntAttributeInfo::type() const{
    return Type::UNSIGNED_INT;		
}
// encode a given item to bitset using the type and domain of this attribute.
// throws exception if type and domain does not match ...
boost::dynamic_bitset<> UsignIntAttributeInfo::encode(Item* itm) 
            throw (OutOfDomainException, InvalidTypeException){            
    int t = itm->type();
    // item and attribute double not have same type
   // cout<<t<<" "<<type()<<endl;
    if(t != type()) throw InvalidTypeException("Invalid type...");
    int value = (dynamic_cast<UsignIntItem*>(itm))->value();
    int up_bound = upper();
    int low_bound = lower();
    if(value < low_bound || value > up_bound) 
        throw OutOfDomainException("Given value of the item is out of domain...");
    int diff = value - low_bound; 
    boost::dynamic_bitset<> result(binaryLength(), static_cast<unsigned long>(diff)); 
    return result;
}

// decode a bitset to an item
// throws if length of bitset does not match with binary length length of
// the attribute.

Item* 	UsignIntAttributeInfo::decode(const boost::dynamic_bitset<>& bits) 
            throw (InvalidLengthException){
	if(binaryLength() != bits.size()) 
		throw InvalidLengthException(" Domain values length and bitset length not same");             	
		return new UsignIntItem(lower() + bits.to_ulong());
}

void UsignIntAttributeInfo::setDomain(int lower, int upper)
{
    lower_ = lower;
    upper_ = upper;
    int diff = upper - lower;
	setBinaryLength(numbits(abs(diff)));
}
