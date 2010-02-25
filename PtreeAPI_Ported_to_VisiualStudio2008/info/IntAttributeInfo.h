// 
// File:   IntAttributeInfo.h
// Author: serazi
//
// Created on October 13, 2003, 12:42 PM
//

#ifndef _IntAttributeInfo_H
#define	_IntAttributeInfo_H
#include <boost/dynamic_bitset.hpp>
#include "AttributeInfo.h"
#include "PException.h"
#include "Item.h"

/**
 * Class for Integer attributes.
 */
class IntAttributeInfo : public AttributeInfo{
    public:
        IntAttributeInfo();
        virtual ~IntAttributeInfo();
        IntAttributeInfo(int low, int up);
        inline virtual int type() const = 0; 
        
        /**
        * Get the Binary encoded representation of the given Item. 
        * Will use the same encoding scheme when creating the Ptrees for this attribute
        */
        virtual boost::dynamic_bitset<> encode(Item* val) 
            throw (OutOfDomainException, InvalidTypeException) = 0;
        
        /**
        * Get the original value (Item) represented by the given binary encoding. 
        * This is the encoding used when creating the Ptrees 
        */ 
        virtual Item* decode(const boost::dynamic_bitset<>& bits) 
            throw (InvalidLengthException) = 0;
         
        /**
        * Set the domain of values  
        * Lowest value and the highest possible value will be used in the encoding scheme
        */ 
        virtual void setDomain(int low, int up) = 0;
        
        /**
        * Get upper bound. 
        * Highest possible value that canbe encoded in the encoding schema used for this attribute
        * NOTE: This may not be the highest value in the data set
        */
        int upper()const;
        
        /**
        * Get lower bound. 
        * Lowest possible value that canbe encoded in the encoding schema used for this attribute
        * NOTE: This may not be the lowest value in the data set
        */
        int lower()const;
    protected:
        int upper_;
        int lower_;
};


#endif	/* _IntAttributeInfo_H */

