// 
// File:   PTreeInfo.h
// Author: serazi
//
// Created on October 20, 2003, 1:13 PM
//
#ifndef _PTREEINFO_H_
#define _PTREEINFO_H_

#include <vector>
#include <string>
#include <boost/dynamic_bitset.hpp>
#include "AttributeInfo.h"
#include "PException.h"
#include "Exception.h"
#include "Item.h"
#include "Tuple.h"
#include "string_token_iterator.h"
/**
 * Keeps track of association between attribute names, sizes and location of dynamic storage (index etc.)
 * Will keep a collection of AttributeInfo objects that will allow 
 * the implementer to manipulate the meta info about the attributes in the PtreeSet Data set
 */
class PTreeInfo{
    public:
     typedef  vector<AttributeInfo*>::iterator att_iterator;
     typedef  vector<AttributeInfo*>::const_iterator const_att_iterator;
     typedef  boost::dynamic_bitset<> boost_bitset;  
       PTreeInfo() throw();
       ~PTreeInfo() throw();
       
       /**
       * Used to add a new AttributeInfo to he collection
       */       
       void addAttributeInfo(AttributeInfo *att_ptr) throw();
       
       /**
       * Used to remove an AttributeInfo from the the collection using the index
       */       
       void removeAttributeInfo(const int index)throw(NotFoundException);
       
       /**
       * Used to remove an AttributeInfo from the the collection using the name
       */ 
       void removeAttributeInfo(const string& name)throw(NotFoundException);
       //throws an exception if name doesn't exist
       
       /**
       * Used to get AttributeInfo from the the collection using the name of attribute
       */ 
       AttributeInfo& getAttributeInfo(const string&  name) const throw(NotFoundException);
       
       /**
       * Used to get an AttributeInfo from the the collection using the index
       */ 
       AttributeInfo& getAttributeInfo(const int index) const throw(NotFoundException);
       
       /**
       * [] operator can be used get an AttributeInfo from the the collection using the index
       * eg AtInfo[2] will return the AttributeInfo with index 2
       */ 
       AttributeInfo& operator[](const int index) const throw(NotFoundException);
      /**
       * Given a name of attribute it will return the index of that attribute
       */  
       size_t getAttributeIndex(const string& name)const throw(NotFoundException);
       /**
       * Used to set the Cardinality (number of tuples) of the dataSet 
       */
       void setCardinality(std::size_t c);
       
       //return the number of attributes of your data set
       
       /**
       * Used to get the number of attributes in the collection
       */ 
       int degree()const;
       
       /**
        * return the total number of PTrees 
        */ 
       int num_ptrees()const;
       
       /**
        * return total number of tuples in the dataset
        */
        
       std::size_t cardinality()const;
       /**
        * Encode a given Tuple to bitset. Each attribute will be 
        * encoded depending on the respective AttributeInfo 
        * After encoding each attribute they will be combined into one
        * bitset
        */
        boost_bitset encode(const Tuple& tuple)
			throw (InvalidLengthException, OutOfDomainException, InvalidTypeException);
       /**
        * Encode a given Tuple to a vector of bitset. Each attribute will be
        * encoded depending on the  respective AttributeInfo.
        * After encodeing each attribute, each will be kept in a vector. They will
        * not be combined
        */
        vector<boost_bitset> encodeS(const Tuple& tuple)
			throw (InvalidLengthException, OutOfDomainException, InvalidTypeException);
        
        vector<boost_bitset> sequential_item_encode(const Tuple& tuple)
			throw (InvalidLengthException, OutOfDomainException, InvalidTypeException);
        /**
        * Decode a given bitset to it's orginal Tuple. Each attribute will be 
        * Decoded depending on the respective AttributeInfo 
        */
        Tuple decode(const boost::dynamic_bitset<>& dbset)
        	throw (OutOfDomainException, InvalidTypeException);
        /**
         * convert a vector of strings to tuple
         * each element of the vector is an item. If there exists one or more multivalued
         * categorical attribute then that element must be a string with comma delimited
         * strings
         * Any "?" mark (missing) will be encoded to default value of an item 
         */
        Tuple PTreeInfo::to_tuple(const vector<string>& str_items)
            throw (OutOfDomainException, InvalidTypeException, InvalidLengthException);


        /**
         * Will return the start index in the Ptree Set for the given attribute index
         * 
         */ 
	size_t getAttributeStartIndex(size_t index) throw(InvalidIndexException);    
        



    private:
       static int refCount;
       int currentSize;
       std::size_t cardinality_;
       vector<AttributeInfo*> attInfo;
};
#endif
