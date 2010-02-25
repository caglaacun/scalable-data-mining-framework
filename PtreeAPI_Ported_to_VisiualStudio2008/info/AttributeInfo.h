// 
// File:   AttributeInfo.h
// Author: Masum H. Serazi
// Date: 10.09.03
//
// Created on October 8, 2003, 6:20 PM
//

#ifndef _AttributeInfo_H
#define	_AttributeInfo_H

#include <iostream>
#include <string>
#include <boost/dynamic_bitset.hpp>

#include "Item.h"
#include "PException.h"

using namespace std;
/**
 * AttributeInfo class holds and manipulates information about an attribute.
 * This class is also responsible to encode and decode an atribute value to 
 * bit_set/bit_string or vice versa.
 *  
 */
class AttributeInfo {  
    public:
        AttributeInfo();
         AttributeInfo(const string& name);
        ~AttributeInfo();
        /**
        * Used to set the name of the attribute
        */       
        void setName(const string& name);
        
        /**
        * Get Binary length of the attribute
        * i.e. the numebr of bits used to represent the attribute
        * i.e. the number of Ptrees used to represen this attribute
        */   
        int binaryLength()const;
        
        /**
        * Get the name of the attribute.
        */   
        string name()const;
        
        /**
        * Get type of attribute
        */   
        inline virtual int type() const = 0;
        /**
        * Get the Binary encoded representation of the given Item. 
        * This is the encoding used when creating the Ptrees 
        */   
        virtual boost::dynamic_bitset<> encode(Item* val) 
            throw (OutOfDomainException, InvalidTypeException) = 0;
        
        /**
        * Get the original vlaue represented by the given binary encoding. 
        * This is the encoding used when creating the Ptrees 
        */ 
        virtual Item* decode(const boost::dynamic_bitset<>& bits) 
            throw (InvalidLengthException) = 0;
    protected:
        void setBinaryLength(int length);
    private:
        // name of this attribute
        string name_;
        // numbers of bit required to represent this attribute
        int binary_length_;
};

#endif	/* _AttributeInfo_H */

