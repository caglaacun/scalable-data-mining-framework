// 
// File:   UsignDoubleAttributeInfo.cc
// Author: Masum H. serazi
//
// Created on October 13, 2003, 12:51 PM
//

#include "UsignDoubleAttributeInfo.h"


//
// Constructor
///
UsignDoubleAttributeInfo::UsignDoubleAttributeInfo()
	: DoubleAttributeInfo(0, 0)
{    
	setPrecision(2);
	setBinaryLength(0);	
}

//
// Constructor with given upper and lower bound of the domain
//
UsignDoubleAttributeInfo::UsignDoubleAttributeInfo(double lower, double upper)
{
	setPrecision(2);
	setDomain(lower, upper);	
}


//
// Destructor
//
UsignDoubleAttributeInfo::~UsignDoubleAttributeInfo()
{
    
}


int UsignDoubleAttributeInfo::type() const{
    return Type::UNSIGNED_DOUBLE;		
}
// encode a given item to bitset using the type and domain of this attribute.
// throws exception if type and domain does not match ...
boost::dynamic_bitset<> UsignDoubleAttributeInfo::encode(Item* itm) 
            throw (OutOfDomainException, InvalidTypeException){ 
    //cout<<"UsignDoubleAttributeInfo::encode() >> "<<endl;        
    int t = itm->type();
    // item and attribute double not have same type
    if(t != type()) throw InvalidTypeException("Invalid type...");
    double value = (dynamic_cast<UsignDoubleItem*>(itm))->value();
    //cout<<"UsignDoubleAttributeInfo::encode() value: " <<value<<endl;
    double up_bound = upper();
    double low_bound = lower();
    //cout<<upper()<<" "<<lower()<<endl;
    if(value < low_bound || value > up_bound) 
        throw OutOfDomainException(value, low_bound, up_bound);
    double diff = value - low_bound;
    //cout<<"different: "<<diff<<endl;
	double adouble =     	diff * pow(10.0, precision());
	//cout << "before Converting to a long " << adouble <<endl;
	unsigned long along = static_cast<unsigned long> (floor(adouble+0.5));
        //cout << "before Converting to binary " << along<<endl;
        boost::dynamic_bitset<> result(binaryLength(), along);
        //cout << "Converting " << along<<" to binary "<<result<<endl;
	//  boost::dynamic_bitset<> result(binaryLength(), 
	//   					static_cast<unsigned long>(diff * 100));

    //cout<<"UsignDoubleAttributeInfo::encode(Item* itm) "<<diff * pow(10.0, precision())<<endl;					
    //cout<<"UsignDoubleAttributeInfo::encode(Item* itm) "<<(unsigned long)(diff * pow(10.0, precision()))<<endl;
    //cout<<"bitLength: "<<binaryLength()<<" Bitset: "<<result<<":"<<result.to_ulong()<<endl;
    return result;
}

/**
 *  decode a bitset to an item
 *  throws if length of bitset does not match with binary length of
 *  the attribute.
 */

Item* 	UsignDoubleAttributeInfo::decode(const boost::dynamic_bitset<>& bits) 
            throw (InvalidLengthException){
	if(binaryLength() != bits.size()) 
		throw InvalidLengthException("Domain values length and bitset length not same"); 
		//cout<<"Before dividing: "<<   bits.to_ulong()<<"Lower: "<<lower()<<endl; 
		//cout<< lower() + bits.to_ulong())/pow(10.0, precision()); 
	//cout<<"UsignDoubleAttributeInfo::decode(const boost::dynamic_bitset<>& bits)"<<bits.to_ulong()<<endl;	       	
		return new UsignDoubleItem(lower() + bits.to_ulong()/pow(10.0, precision()));
}
/**
 * set the upper and lower bound of the domain and also calculate maximum
 * number of bits required for the corresponding domain
 */
void UsignDoubleAttributeInfo::setDomain(double lower, double upper)
{
    lower_ = lower;
    upper_ = upper;
    double diff = upper - lower;
    int nbits = numbits(static_cast<int>((abs(diff * pow(10.0, precision())))));
	setBinaryLength(nbits);
	//cout<<"setting domian done..."<<endl;
}
