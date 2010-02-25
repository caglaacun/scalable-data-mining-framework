// 
// File:   Item.h
// Author: serazi
//
// Created on October 9, 2003, 3:52 PM
//

#ifndef _Item_H
#define	_Item_H
#include <iostream>
#include <string>
#include <vector>
#include "Type.h"



using namespace std;
/** 
  * Item will indicate each data value. Collection of items will make a tuple 
  * Item is used to generalize each of the specific attribute types
  * Single value in a record is an item 
  *
  * @todo May need to extend Item to heirachial categorical type
 */  
 
class Item{
    public:
        Item();
        virtual ~Item();
        /** 
         * Will return the type of item as defined in Type               
         */
        virtual int type()const = 0;
         /** 
         * Copy constructor for item               
         */
        virtual Item* copy() = 0;
        /** 
         * Will ouput the value of item as a string               
         */
        virtual string to_string()const = 0;
    //private:
        static int ref_count;    
};

/**
 * Unsigned integer type item eg. 4,5,6 not 4.1,-2.
 */
class UsignIntItem : public Item {
    public:
        UsignIntItem(unsigned val);
        /** 
         * Will return the type of item as defined in Type               
         */
        virtual int type()const;
         /** 
         * Copy constructor for item               
         */
        virtual Item* copy();
        /** 
         * Will return the value of item as a string               
         */
        virtual string to_string()const;
        /** 
         * Will return the value of item.                
         */
        unsigned value()const; 
    private:
        unsigned value_;
};

/**
 * Signed integer type item eg. 4,-5, 6 not 4.1,2.1
 */
class SignIntItem : public Item {
    public:
        SignIntItem(int val);
        /** 
         * Will return the type of item as defined in Type               
         */
        virtual int type()const;
        /** 
         * Copy constructor for item               
         */
        virtual Item* copy();
        /** 
         * Will return the value of item as a string               
         */
        virtual string to_string()const;
        /** 
         * Will return the value of item.                
         */
        int value()const;
    private:
        int value_;
};

/**
  * Unsigned double type item eg 4.5,6.4, not -4.5
  */
class UsignDoubleItem : public Item {
    public:
        UsignDoubleItem(double val);
        /** 
         * Will return the type of item as defined in Type               
         */
        virtual int type()const;
        /** 
         * Copy constructor for item               
         */
        virtual Item* copy();
        /** 
         * Will return the value of item as a string               
         */
        virtual string to_string()const;
        /** 
         * Will return the value of item.                
         */
        double value()const;  
    private:
        double value_;
};

/**
  * Signed double type item eg -4.5,3.2 
  */
class SignDoubleItem : public Item {
    public:
        SignDoubleItem(double val);
        /** 
         * Will return the type of item as defined in Type               
         */
        virtual int type()const;
        /** 
         * Copy constructor for item               
         */
        virtual Item* copy();
        /** 
         * Will return the value of item as a string               
         */
        virtual string to_string()const;
        /** 
         * Will return the value of item.                
         */
        double value()const;  
    private:
        double value_;
};

/**
  * Single valued categorical item. Each record has only one value for the item
  * eg. {a},{b},{c}
  */
class SingCatItem : public Item {
    public:
        SingCatItem(const string& value);
        /** 
         * Will return the type of item as defined in Type               
         */
        virtual int type()const;
        /** 
         * Copy constructor for item               
         */
        virtual Item* copy();
        /** 
         * Will return the value of item as a string               
         */
        virtual string to_string()const;
        /** 
         * Will return the value of item as string.                
         */
        string value()const;   
    private:
        string value_;
};


/**
  * Multi valued categorical item. Each record may have more than one value per item
  * eg {a,b,c},{a},{a,b}
  */
class MultiCatItem : public Item {
    public:
        MultiCatItem(const vector<string>& value);
        /** 
         * Will return the type of item as defined in Type               
         */
        virtual int type()const;
        /** 
         * Copy constructor for item               
         */
        virtual Item* copy();
        /** 
         * Will return the value of item as a string               
         */
        virtual string to_string()const;
        /** 
         * Will return the value of item as a vector of strings.                
         */
        vector<string> value()const;  
    private:
        vector<string> value_;
};

/**
  * skip item
  * Items that are not used for processing are designated as skip items
  * Data capture will not capture attributed designated as skip attributes
  */
class SkipItem : public Item {
    public:
        SkipItem();
        /** 
         * Will return the type of item as defined in Type               
         */
        virtual int type()const;
        /** 
         * Copy constructor for item               
         */
        virtual Item* copy();
        virtual string to_string()const;
};

//Global function to compare Items
bool operator==(const Item& first, const Item& second);
bool operator!=(const Item& first, const Item& second);
#endif	/* _Item_H */

