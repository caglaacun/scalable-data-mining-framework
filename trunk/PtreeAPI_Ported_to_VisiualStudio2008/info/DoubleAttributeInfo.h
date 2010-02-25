// 
// File:   DoubleAttributeInfo.h
// Author: serazi
//
// Created on October 15, 2003, 6:06 PM
//

#ifndef _DoubleAttributeInfo_H
#define	_DoubleAttributeInfo_H
#include <boost/dynamic_bitset.hpp>
#include "AttributeInfo.h"
#include "PException.h"
#include "Item.h"

/**
 * Class for Double attributes.
 */
class DoubleAttributeInfo : public AttributeInfo{
    public:
        DoubleAttributeInfo();
        virtual ~DoubleAttributeInfo();
        DoubleAttributeInfo(double low, double up);
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
        virtual void setDomain(double low, double up) = 0;
        
        /**
        * Set the precision for the double value. Numeber of decimal digits required. 
        * Precision will be used in the encoding scheme. 
        * Higher precision will require more bits, more ptrees.
        */ 
        void setPrecision(int p);
        
        /**
        * Get upper bound. 
        * Highest possible value that canbe encoded in the encoding schema used for this attribute
        * NOTE: This may not be the highest value in the data set
        */
        double upper()const;
        
        /**
        * Get lower bound. 
        * Lowest possible value that canbe encoded in the encoding schema used for this attribute
        * NOTE: This may not be the lowest value in the data set
        */
        double lower()const;
        
        /**
        * Get the precision for the double value. 
        * Numeber of decimal digits preserved. 
        * Precision used in the encoding scheme.  
        */
        int precision()const;
    protected:
        double upper_;
        double lower_;
        int precision_;
};


#endif	/* _DoubleAttributeInfo */

