// 
// File:   UsignIntAttributeInfo.h
// Author: serazi
//
// Created on October 13, 2003, 12:51 PM
//

#ifndef _UsignDoubleAttributeInfo_H
#define _UsignDoubleAttributeInfo_H
#include <cmath>
#include "DoubleAttributeInfo.h"
#include "BinaryMath.h"


class UsignDoubleAttributeInfo : public DoubleAttributeInfo{
public:
	UsignDoubleAttributeInfo();
	UsignDoubleAttributeInfo(double lower, double upper);
	~UsignDoubleAttributeInfo();
        inline virtual int type() const ;
        
        /**
        * Get the Binary encoded representation of the given Item. 
        * Will use the same encoding scheme when creating the Ptrees for this attribute
        */  
        virtual boost::dynamic_bitset<> encode(Item* val) 
            throw (OutOfDomainException, InvalidTypeException);
        
        /**
        * Get the original value (Item) represented by the given binary encoding. 
        * This is the encoding used when creating the Ptrees 
        */ 
        virtual Item* decode(const boost::dynamic_bitset<>& bits) 
            throw (InvalidLengthException) ;
        
        /**
        * Set the domain of values  
        * Lowest value and the highest possible value will be used in the encoding scheme
        */ 
        virtual void setDomain(double lower, double upper);      
private:

};

#endif	/* _UsignIntAttributeInfo_H */

