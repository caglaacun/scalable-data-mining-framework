// 
// File:   SignIntAttributeInfo.h
// Author: Masum H. Serazi
//
// Created on October 13, 2003, 12:51 PM
//

#ifndef _SignDoubleAttributeInfo_H
#define _SignDoubleAttributeInfo_H
 
#include "DoubleAttributeInfo.h" 
#include "BinaryMath.h"


/**
 * Class for signed Double attributes. Extended from double attribute
 */
class SignDoubleAttributeInfo : public DoubleAttributeInfo{
	public:
		SignDoubleAttributeInfo();
		SignDoubleAttributeInfo(double lower, double upper);
		~SignDoubleAttributeInfo();
		inline virtual int type() const ;
                
                /**
                * Get the Binary encoded representation of the given Item. 
                * Will use the same encoding scheme when creating the Ptrees for this attribute
                */
		virtual boost::dynamic_bitset<> encode(Item* val)                                
            	throw (OutOfDomainException, InvalidTypeException);
                
                /**
                * Get the original value represented by the given binary encoding. 
                * This is the encoding used when creating the Ptrees 
                */    
		virtual Item* decode(const boost::dynamic_bitset<>& bits) 
            	throw (InvalidLengthException) ;
                
                /**
                * Set the domain of values  
                * Lowest value and the highest possible value will be used in the encoding scheme
                */ 
		virtual void setDomain(double lower, double upper);
};

#endif	/* _SignIntAttributeInfo_H */
