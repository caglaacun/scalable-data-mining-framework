#include "Tuple.h"

// constructor
Tuple::Tuple(){
    //cout<<"default constractor has been called"<<endl;
}
/**
  * copy ctor 
  */ 
Tuple::Tuple(const Tuple& other){
    //cout<<"Copy constractor has been called"<<endl;
	for(int i=0; i<other.degree(); i++){
		add(other.items[i]->copy());
	}
}

/**
  * dtor 
  * It is necessary to check that all or some of the items are not 
  * deleted by some other owner ...
  */
Tuple::~Tuple(){
    //cout<<"calling ~Tuple()"<<endl;
    Tuple::tuple_iterator it;
    if(Item::ref_count > 0)
    	for(it = items.begin(); it != items.end(); ++it){
                //cout<<"calling ~Tuple()::item delete"<<endl;
        	delete *it; 
    	}    
}

Tuple& Tuple::operator=(const Tuple& other){
    //cout<<"Assignment has been called"<<endl;
	for(int i=0; i<other.degree(); i++){
		add(other.items[i]->copy());
	}
	return *this;
}
/**
  * Add an item to the tuple. When we add an item we also pass the owenership
  * to the tuple. So wherever the item has been created does not matter. The 
  * tuple is responsinble to destroy the item.
  */
void Tuple::add(Item* const itm){
    items.push_back(itm);
}
ostream& Tuple::serialize(ostream& os)const{
	Tuple::tuple_iterator it = items.begin();
	for(;it!=items.end();++it){
		int t = (*it)->type();
		switch(t){
			case Type::UNSIGNED_INT:
				os<<(dynamic_cast<UsignIntItem*>(*it))->value()<<" \t";
				break;
			case Type::SIGNED_INT:
				os<<(dynamic_cast<SignIntItem*>(*it))->value()<<" \t";
				break;	
			case Type::SIGNED_DOUBLE:
				os<<(dynamic_cast<SignDoubleItem*>(*it))->value()<<" \t";
				break;
			case Type::UNSIGNED_DOUBLE:
				os<<(dynamic_cast<UsignDoubleItem*>(*it))->value()<<" \t";
				break;
			case Type::SING_CAT:
				os<<(dynamic_cast<SingCatItem*>(*it))->value()<<" \t";
				break;
			case Type::MULT_CAT:
			    vector<string> v = (dynamic_cast<MultiCatItem*>(*it))->value();
			    os<<"(";
			    for(int i=0; i<v.size()-1; i++){
			    	os<<v[i]<<",";
			    } 
				os<<v[v.size()-1]<<")"<<" \t";
				break;						
		}
	}
	return os;
}
/*
string Tuple::to_string()const{
    string s("");
    Tuple::tuple_iterator it = items.begin();
    for(;it!=items.end();++it){
            int t = (*it)->type();
            switch(t){
                    case Type::UNSIGNED_INT:
                            os<<(dynamic_cast<UsignIntItem*>(*it))->value()<<" \t";
                            break;
                    case Type::SIGNED_INT:
                            os<<(dynamic_cast<SignIntItem*>(*it))->value()<<" \t";
                            break;	
                    case Type::SIGNED_DOUBLE:
                            os<<(dynamic_cast<SignDoubleItem*>(*it))->value()<<" \t";
                            break;
                    case Type::UNSIGNED_DOUBLE:
                            os<<(dynamic_cast<UsignDoubleItem*>(*it))->value()<<" \t";
                            break;
                    case Type::SING_CAT:
                            os<<(dynamic_cast<SingCatItem*>(*it))->value()<<" \t";
                            break;
                    case Type::MULT_CAT:
                        vector<string> v = (dynamic_cast<MultiCatItem*>(*it))->value();
                        cout<<"(";
                        for(int i=0; i<v.size()-1; i++){
                            os<<v[i]<<",";
                        } 
                            os<<v[v.size()-1]<<")"<<" \t";
                            break;						
            }
	}
	return os;
}*/
/**
 *  return the value of ith attrubute at jth bit
 */ 
bool Tuple::get(int i, int j)const{
  cout<<"ERROR:Tuple.get(i,j) does not work:code missing"
}
/**
 *  returns a pointer to a copy of item, 
 *  NEED to EXPLICITLY delete the pointer once done
 *  i.e.
 *  Item* it = t.get(j);
 *  delete it;  
 */ 
Item* Tuple::get(int i)const{
     return items[i]->copy();
}
/**
 * returns number of items in the tuple
 */ 
Tuple::size_type Tuple::degree()const{
    return items.size();
}

Tuple::tuple_iterator Tuple::begin() const{
	return items.begin();
}

Tuple::tuple_iterator Tuple::end()const{
	return items.end();
}

ostream& operator<<(ostream&os, const Tuple& t){
	return t.serialize(os);
}
