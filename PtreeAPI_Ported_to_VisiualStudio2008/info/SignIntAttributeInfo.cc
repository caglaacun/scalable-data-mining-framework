// 
// File:   UsignIntAttributeInfo.cc
// Author: Masum H. serazi
//
// Created on October 13, 2003, 12:51 PM
//

#include "SignIntAttributeInfo.h"
#include "BinaryMath.h"
#include <algorithm> 

//
// Default Constructor
///
SignIntAttributeInfo::SignIntAttributeInfo() : IntAttributeInfo()
{ 
	setBinaryLength(0);
}
//
// Constructor with given upper and lower bound of the domain
//
SignIntAttributeInfo::SignIntAttributeInfo(int low, int upp)
{
	setDomain(low, upp);	
}
//
// Destructor
//
SignIntAttributeInfo::~SignIntAttributeInfo()
{
    
}
//
// return SIGNED_INT type
//
int SignIntAttributeInfo::type() const{
    return Type::SIGNED_INT;		
}


void SignIntAttributeInfo::setDomain(int low, int upp)
{
    lower_ = low;
    upper_ = upp;
    int max_value = max(abs(upper()), abs(lower()));
	// For sign bit 1 more bit is needed
	setBinaryLength(numbits(max_value) + 1);
}

// encode a given item to bitset using the type and domain of this attribute.
// throws exception if type and domain does not match ...
boost::dynamic_bitset<> SignIntAttributeInfo::encode(Item* itm) 
            throw (OutOfDomainException, InvalidTypeException){            
    int t = itm->type();
    // item and attribute double do not have same type
    if(t != type()) throw InvalidTypeException("Invalid type...");
    int value = (dynamic_cast<SignIntItem*>(itm))->value();
   // cout<<"value :"<< value<<endl;
    int up_bound = upper();
    int low_bound = lower();
    if(value < low_bound || value > up_bound) 
        throw OutOfDomainException("Out of domain...");
    int abs_value = abs(value); 
    boost::dynamic_bitset<> result(binaryLength() , 
    				static_cast<unsigned long>(abs_value));
   // cout<<"result before negation: "<<result;
   if(value < 0) result[binaryLength()-1] = true; //for negative highest order bit is 1
   else result[binaryLength() -1] = false; //for possitive highest order bit is 1
   //cout<<"result after negation: "<<result;
   return result;
}

// decode a bitset to an item
// throws if length of bitset does not match with binary length length of
// the attribute.

Item* 	SignIntAttributeInfo::decode(const boost::dynamic_bitset<>& bits) 
            throw (InvalidLengthException){
	if(binaryLength() != bits.size()) 
		throw InvalidLengthException(" Domain values length and bitset length not same");
		boost::dynamic_bitset<> acopy ( bits);
		if(acopy[binaryLength() -1]){
			acopy[binaryLength() -1] = false;
			return new SignIntItem( acopy.to_ulong() * -1);
		}             	
		return new SignIntItem(bits.to_ulong());
}

