// 
// File:   SignIntAttributeInfo.h
// Author: serazi
//
// Created on October 13, 2003, 12:51 PM
//

#ifndef _SignIntAttributeInfo_H
#define	_SignIntAttributeInfo_H

#include "IntAttributeInfo.h"


/**
 * Class for Signed Integer attributes. extended from Integer attribute
 */
class SignIntAttributeInfo : public IntAttributeInfo{
public:
	SignIntAttributeInfo();
	SignIntAttributeInfo(int lower, int upper);
	~SignIntAttributeInfo();
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
        virtual void setDomain(int lower, int upper);    
private:

};

#endif	/* _UsignIntAttributeInfo_H */

