// 
// File:   SignDoubleAttributeInfo.cc
// Author: Masum H. serazi
//
// Created on October 13, 2003, 12:51 PM
//
#include "SignDoubleAttributeInfo.h"   


//
// Constructor
///
SignDoubleAttributeInfo::SignDoubleAttributeInfo()
	: DoubleAttributeInfo(0, 0)
{    
	setPrecision(2);
	setBinaryLength(0);	
}

//
// Constructor with given upper and lower bound of the domain
//
SignDoubleAttributeInfo::SignDoubleAttributeInfo(double lower, double upper)
{
	// set defaulty precision to 2
	setPrecision(2);
	setDomain(lower, upper);	
}


//
// Destructor
//
SignDoubleAttributeInfo::~SignDoubleAttributeInfo()
{
    
}


int SignDoubleAttributeInfo::type() const{
    return Type::SIGNED_DOUBLE;		
}
// encode a given item to bitset using the type and domain of this attribute.
// throws exception if type and domain does not match ...
boost::dynamic_bitset<> SignDoubleAttributeInfo::encode(Item* itm) 
            throw (OutOfDomainException, InvalidTypeException){  
    //cout<<"SignDoubleAttributeInfo::encode(Item* itm)"<<endl;        	          
    int t = itm->type();
    // item and attribute double do not have same type
    if(t != type()) throw InvalidTypeException("ERROR: Invalid type...");
    double value = (dynamic_cast<SignDoubleItem*>(itm))->value();  
    double up_bound = upper();
    double low_bound = lower();
   // cout<<upper()<<" "<<lower()<<endl;
    if(value < low_bound || value > up_bound) 
        throw OutOfDomainException(value, lower(), upper());  
    double abs_val = abs(value) * pow(10.0, precision()); 
    unsigned long along =  static_cast<unsigned long>(floor(abs_val +0.5 ));
    boost::dynamic_bitset<> result(binaryLength(), along);
     if(value < 0) result[binaryLength()-1] = true; //for negative highest order bit is 1
   else result[binaryLength() -1] = false; //for possitive highest order bit is 1 
    return result;
}

/**
 *  decode a bitset to an item
 *  throws if length of bitset does not match with binary length length of
 *  the attribute.
 */
Item* 	SignDoubleAttributeInfo::decode(const boost::dynamic_bitset<>& bits) 
            throw (InvalidLengthException){
	if(binaryLength() != bits.size()) 
		throw InvalidLengthException("Domain values length and bitset length not same"); 
		boost::dynamic_bitset<> acopy ( bits);
		if(acopy[binaryLength() -1]){
			acopy[binaryLength() -1] = false;
			return new SignDoubleItem(acopy.to_ulong() * -1.0/pow(10.0, precision()));
		}	  	
		return new SignDoubleItem(bits.to_ulong()/pow(10.0, precision()));
}
/**
 * set the upper and lower bound of the domain and also calculate maximum
 * number of bits required for the corresponding domain
 */
void SignDoubleAttributeInfo::setDomain(double low, double up)
{
    lower_ = low;
    upper_ = up;
    double max_val = max(abs(upper()), abs(lower())); 
    int nbits = numbits(static_cast<int>((abs(max_val * pow(10.0, precision()))))); 
    // One more bit for sign
	setBinaryLength(nbits + 1);
}
