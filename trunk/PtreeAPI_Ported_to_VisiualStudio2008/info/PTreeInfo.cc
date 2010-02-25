// 
// File:   PTreeInfo.cc
// Author: serazi
//
// Created on October 20, 2003, 1:13 PM
//

#include "PTreeInfo.h"
#include "UsignIntAttributeInfo.h"
#include "SignIntAttributeInfo.h"
#include "UsignDoubleAttributeInfo.h"
#include "SignDoubleAttributeInfo.h"
#include "SingCatAttributeInfo.h"
#include "MultiCatAttributeInfo.h"
#include "Util.h"
//constructor has nothing to do
PTreeInfo::PTreeInfo()throw()
    :currentSize(0){
    refCount ++;
    cardinality_ = 0;
}

/**
 * PTreeInfo is the owner of all AttributeInfo pointer
 * Clean all of them when you are done
 */
PTreeInfo::~PTreeInfo()throw(){
   //cout<<"~PTreeInfo()"<<endl;	
   if(refCount == 1){
      for(int i=0; i<attInfo.size(); i++){
          delete attInfo[i];
      }
      refCount--;
   }
}

/**
 * Add a AttributeInfo referance to the list
 * No exception will be thrown
 */
void PTreeInfo::addAttributeInfo(AttributeInfo *att_ptr)throw(){
    //bandPtr->setStartIndex(currentSize);
    attInfo.push_back(att_ptr);
    //currentSize += att_ptr->getBinaryLength();
}

//void PTreeInfo::removeAttributeInfo(const string& name)throw(NotFoundException){
//   cout<<"Not implemented yet..."<<endl;
//}


void PTreeInfo::removeAttributeInfo(const int index)throw(NotFoundException){
    if(index>=attInfo.size()) throw NotFoundException("Index is out of bound");
    else{
      attInfo.erase(attInfo.begin()+index);
   }
}
/**
 * Return a AttributeInfo referance for a given name
 * if name does not exist throws an exception
 */
AttributeInfo& PTreeInfo::getAttributeInfo(const string& nm)const 
	throw(NotFoundException){
    for(int i=0; i<attInfo.size(); i++){
    	//int tmp=attInfo[i]->binaryLength();
    	//string tmp=attInfo[i]->name();
        if(attInfo[i]->name() == nm)
            return *attInfo[i];
    }
    throw NotFoundException("Attribute with name: "+ nm +" does not exist");
}

/**
 * Return a AttributeInfo referance for a given index
 * if index does not exist throws an exception
 */
AttributeInfo& PTreeInfo::getAttributeInfo(const int index) const 
	throw(NotFoundException){
    if(index>=attInfo.size()) throw NotFoundException("ERROR: Index is out of bound");
    else return *attInfo[index];
}

/**
 * Return a AttributeInfo referance for a given index
 * if index does not exist throws an exception
 */
AttributeInfo& PTreeInfo::operator[](const int index) const throw(NotFoundException){
    if(index>=attInfo.size()) throw NotFoundException("ERROR: Index is out of bound");
    else return *attInfo[index];
}
/**
 * Given a name of attribute it will return the index of that attribute
 */  
 size_t PTreeInfo::getAttributeIndex(const string& name)const 
 	throw(NotFoundException){
 	PTreeInfo::const_att_iterator ait = attInfo.begin();
 	size_t i = 0;
 	for(;ait!=attInfo.end();++ait, i++){
 		if((*ait)->name() == name) return i;
 	}
 	throw NotFoundException("ERROR: "+name + " is not a valid attribute name");
 }
void PTreeInfo::setCardinality(std::size_t c){
	cardinality_ = c;
 }
/**
 * return the degree (number of attributes) of your data set
 */
int PTreeInfo::degree()const {
    return attInfo.size();
}

/**
* return total number of ptrees in the set
*/
int PTreeInfo::num_ptrees()const{
	int size= degree();
    int sum = 0;
    for (int i=0; i< size;i++)
    	sum +=attInfo[i]->binaryLength();
   return sum; 
}
 /**
  * return total number of tuples in the dataset
  */
 std::size_t PTreeInfo::cardinality()const{
 	return cardinality_;
 }
/**
 * encode a given Tuple to bitset 
 */
boost::dynamic_bitset<> PTreeInfo::encode(const Tuple& tuple)
	throw (InvalidLengthException, OutOfDomainException, InvalidTypeException){
	if(tuple.degree() != attInfo.size())
		throw InvalidLengthException("ERROR: Number of items in a tuple and number of attribute must be same");
	try{		
		Tuple::tuple_iterator it = tuple.begin();
		PTreeInfo::att_iterator ait= attInfo.begin();
		//cout<<(*ait)->name()<<endl;
	    boost::dynamic_bitset<> total(num_ptrees(), false);
	    int j = 0;
		for(;it!=tuple.end() || ait!=attInfo.end();++it, ++ait){
			//cout<<"inside ptreeinfo::encode(): type = "<<(*ait)->type()<<endl;
			//cout<<"inside ptreeinfo::encode(): type = "<<attInfo[m]->type()<<endl; 
			boost::dynamic_bitset<> bs((*ait)->encode(*it));
			//cout<<(*ait)->name()<<" "<<(*ait)->encode(*it)<<endl;
			for(int i=0; i <bs.size(); i++){
				total[num_ptrees() - 1 - j] = bs[bs.size()-1 - i]; 
				//cout<<total<<endl;
				j++;
			}	
		}
		return total;
	}
	catch(const exception& e){ 
		//cout<<
		throw;
	}	
}
/**
 * encode a given Tuple to bitset 
 */
vector<PTreeInfo::boost_bitset> PTreeInfo::encodeS(const Tuple& tuple)
	throw (InvalidLengthException, OutOfDomainException, InvalidTypeException){
	if(tuple.degree() != attInfo.size())
		throw InvalidLengthException("ERROR: Number of items in a tuple and number of attribute must be same");
	try{		
		Tuple::tuple_iterator it = tuple.begin();
		PTreeInfo::att_iterator ait= attInfo.begin();
		vector<PTreeInfo::boost_bitset> v;
		for(;it!=tuple.end() || ait!=attInfo.end();++it, ++ait){
			PTreeInfo::boost_bitset bs((*ait)->encode(*it));
			v.push_back(bs);
		}
		return v;
	}
	catch(const exception& e){ 
		throw;
	}	
}
/**
 * encode a given Tuple to bitset 
 */
vector<PTreeInfo::boost_bitset> PTreeInfo::sequential_item_encode(const Tuple& tuple)
	throw (InvalidLengthException, OutOfDomainException, InvalidTypeException){
	if(tuple.degree() != attInfo.size())
		throw InvalidLengthException("ERROR: Number of items in a tuple and number of attribute must be same");
	try{		
		Tuple::tuple_iterator it = tuple.begin();
		PTreeInfo::att_iterator ait= attInfo.begin();
		vector<PTreeInfo::boost_bitset> v;
		for(;it!=tuple.end() || ait!=attInfo.end();++it, ++ait){
			PTreeInfo::boost_bitset bs((*ait)->encode(*it));
			v.push_back(bs);
		}
		return v;
	}
	catch(const exception& e){ 
		throw;
	}	
}
/**
 * decode a bitset to a tuple
 */ 
 Tuple PTreeInfo::decode(const boost::dynamic_bitset<>& dbset)
 	throw (OutOfDomainException, InvalidTypeException){
 		try{
 			Tuple t;
 			PTreeInfo::att_iterator ait = attInfo.begin();
 			int j = 0;
 			for(int i=0; i<degree(); i++, ++ait){
 				boost::dynamic_bitset<> bs((*ait)->binaryLength(), false);
 				for(int k=(*ait)->binaryLength() -1; k>=0; k--){
 					bs[k] = dbset[ dbset.size() -1 - j]; 
 					j++;
 			  }
 			 // cout<<bs<<endl;
 			 t.add((*ait)->decode(bs));	
 		  }
 		return t;
 		}
 		catch(const exception& e){
 		 throw;
 	}
 }
 
/**
 * convert a vector of strings to a tuple
 * each element of the vector is an item. If there exists one or more multivalued
 * categorical attribute then that element must be a string with "|" delimited
 * strings
 * Any "?" mark (missing) will be encoded to default value of an item 
 */
 
 Tuple PTreeInfo::to_tuple(const vector<string>& str_items)
    throw (OutOfDomainException, InvalidTypeException, InvalidLengthException)
 {
     //cout<<"str_items.size(): "<<str_items.size()<<" attInfo.size(): "<<attInfo.size()<<endl;
     if(str_items.size() != attInfo.size())
		throw  InvalidLengthException("PTreeInfo: Expected number of items "+to_string(attInfo.size())+ " Given is "+to_string(str_items.size()));           	
	//attInfo = parser_.getAttributeInfo();	
	//vector<string>::iterator it = str_items.begin(); 
        PTreeInfo::att_iterator att_it = attInfo.begin();
	size_t i = 0;
	Tuple tp;          
	for(; att_it!=attInfo.end();++att_it, i++){
		AttributeInfo* att = *att_it; 
		int t =  att->type();
                if(t == Type::UNSIGNED_INT){
                    if(str_items[i] == "?"){
                       unsigned n = (dynamic_cast<UsignIntAttributeInfo*>(att))->lower();
                       tp.add(new UsignIntItem(n));
                    }
                    else{
                        unsigned n = static_cast<unsigned>(atoi(str_items[i].c_str())); 
			tp.add(new UsignIntItem(n));
                    }
                }
                else if(t == Type::SIGNED_INT){
                    if(str_items[i] == "?"){
                        tp.add(new SignIntItem((dynamic_cast<SignIntAttributeInfo*>(att))->lower()));
                    }
                    else{
                       tp.add(new SignIntItem(atoi(str_items[i].c_str())));  
                    }
                }
                else if(t == Type::UNSIGNED_DOUBLE){
                     if(str_items[i] == "?"){
                        tp.add(new UsignDoubleItem((dynamic_cast<UsignDoubleAttributeInfo*>(att))->lower()));
                    }
                    else{
                       char * endAdd; 
                       double d = strtod(str_items[i].c_str(), &endAdd);
		       tp.add(new UsignDoubleItem(d));
                    }
                }
                else if(t == Type::SIGNED_DOUBLE){
                    if(str_items[i] == "?"){
                        tp.add(new SignDoubleItem((dynamic_cast<SignDoubleAttributeInfo*>(att))->lower()));
                    }
                    else{
                       char * endAdd; 
                       double d = strtod(str_items[i].c_str(), &endAdd);
		       tp.add(new SignDoubleItem(d));
                    }
                }
                else if(t == Type::SING_CAT){
                    if(str_items[i] == "?"){
                        vector<string> dom = (dynamic_cast<SingCatAttributeInfo*>(att))->getDomain();
                        tp.add(new SingCatItem(dom[0]));
                    }
                    else{
                        tp.add(new SingCatItem(str_items[i])); 
                    }
                }
                else if(t == Type::MULT_CAT){
                    if(str_items[i] == "?"){
                        vector<string> dom = (dynamic_cast<MultiCatAttributeInfo*>(att))->getDomain();
                        tp.add(new MultiCatItem(dom));
                    }
                    else{
                       string_token_iterator b, e; 
                       b = string_token_iterator(str_items[i], "|");
		       vector<string> vals = vector<string>(b, e);  
		       tp.add(new MultiCatItem(vals));
                    }
                }
	}
     return tp;
 }


/**
 * Will return the start index in the Ptree Set for the given attribute index
 * 
 */ 
size_t PTreeInfo::getAttributeStartIndex(size_t index) throw(InvalidIndexException)
{
if( index >= attInfo.size() ) 
            throw InvalidIndexException("ERROR: Given index is not a valid index");
 else{
   size_t count=0;
   PTreeInfo::att_iterator ait = attInfo.begin();
   PTreeInfo::att_iterator indexIt = attInfo.begin()+index;
   for ( ; ait<indexIt;ait++)
     count+= (*ait)->binaryLength();
   return count;
 }
}
        

//initilaize the static variable
int PTreeInfo::refCount = 0; 


