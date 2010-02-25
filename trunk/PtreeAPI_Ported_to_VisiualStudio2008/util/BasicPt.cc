#include "BasicPt.h"
#include <vector>
BasicPt::BasicPt(){
	//cout<<"BasicPt::BasicPt()"<<endl;
}
BasicPt::BasicPt(const BasicPt& other){
	//cout<<"BasicPt::BasicPt(const BasicPt& other)"<<endl;
	pi_ = other.pi_;
	vector <OpIndex*>::const_iterator it = other.pfcont.begin(); 
	for(;it!=other.pfcont.end(); ++it){
		//cout<<(*it)->value()<<endl;
		pfcont.push_back((*it)->clone());
	}
}
BasicPt::BasicPt(const PTreeInfo& pi):pi_(pi){ 
	//cout<<"BasicPt::BasicPt(const PTreeInfo& pi)"<<endl;
}
 
BasicPt::~BasicPt() {
	vector <OpIndex*>::iterator it = pfcont.begin();  
	for(;it!=pfcont.end(); ++it){
		delete *it;
	} 
}
BasicPt& BasicPt::operator=(const BasicPt& other){
	//cout<<"BasicPt::operator=(const BasicPt& other)"<<endl;
	pi_ = other.pi_;
	pfcont = vector <OpIndex*>();
	vector <OpIndex*>::const_iterator it = other.pfcont.begin(); 
	for(;it!=other.pfcont.end(); ++it){
		//cout<<(*it)->value()<<endl;
		pfcont.push_back((*it)->clone());
	}
	//tmp.print();
	return *this;
}
BasicPt BasicPt::operator()(int a, int b) {
	BasicPt bc(pi_);
	if(a>=pi_.degree()) return bc;//*this;//throw(...)
	if(b>=pi_[a].binaryLength())return bc;
	size_t i = 0;
	for(size_t k=0; k<=a; k++){
		i +=pi_[k].binaryLength();				
	}
	size_t my_index = i - b -1;
	if( my_index >= pi_.num_ptrees()) return bc;
	OpIndex *bi = new BasicIndex(my_index); 
	bc.pfcont.push_back(bi);
	return bc;
}

BasicPt BasicPt::operator()(int a) {
	BasicPt bc(pi_);
	OpIndex *di = new DerivedIndex(a); 
	bc.pfcont.push_back(di);
	return bc;
}

BasicPt BasicPt::operator&(const BasicPt& p1) {
	BasicPt tmp(*this);
	vector<OpIndex*>::const_iterator it = p1.pfcont.begin();
	for(;it!=p1.pfcont.end(); ++it){
		tmp.pfcont.push_back((*it)->clone());
	}
	tmp.pfcont.push_back(new Operator(AND));
	return tmp;					
}
BasicPt BasicPt::operator|(const BasicPt& p1){
	BasicPt tmp(*this);
	vector<OpIndex*>::const_iterator it = p1.pfcont.begin();
	for(;it!=p1.pfcont.end(); ++it){
		tmp.pfcont.push_back((*it)->clone());
	}
	tmp.pfcont.push_back(new Operator(OR));
	return tmp;
}

BasicPt BasicPt::operator !(){
	BasicPt tmp(*this);
    tmp.pfcont.push_back(new Operator(NOT));
    
	return tmp;		
}

size_t BasicPt::size()const{
	return pfcont.size();	
}
OpIndex* BasicPt::getOpIndex(const size_t i)const{
	return pfcont[i];
}

/**
 * This method is only for testing purpose
 */ 
void BasicPt::print()const{
    /*
	vector <OpIndex*>::const_iterator it = pfcont.begin(); 
	//cout<<"BasicPt......................."<<endl;
	for(;it!=pfcont.end(); ++it){
		OpIndex* op = *it;
		int t = op->type();
		size_t val ;
		switch(t){
    			case OpIndex::OPTR:
    				val = op->value();
    				switch(val){
    					case AND:
    					    cout<<"AND ";					
    						break;
    					case OR:
    					   cout<<"OR ";
    						break;
    					case NOT:
    					   cout<<"NOT ";
    						break;
    					}
    					break;
    			case OpIndex::BASIC:
    				val = op->value();
    				cout<<val<<" ";
    				break;
    			case OpIndex::DRVD:
    				val = op->value();
    				val = op->value();
    				cout<<val<<" ";
    				break;   				
    		}
	} 
	//cout<<endl<<".............................."<<endl;
    */
    cout<<toString()<<endl;
}

string BasicPt::toString()const{
    string s = "";
    vector <OpIndex*>::const_iterator it = pfcont.begin(); 
	//cout<<"BasicPt......................."<<endl;
	for(;it!=pfcont.end(); ++it){
		OpIndex* op = *it;
		int t = op->type();
		size_t val ;
		switch(t){
    			case OpIndex::OPTR:
    				val = op->value();
    				switch(val){
    					case AND:
    					    s +="AND ";					
    						break;
    					case OR:
    					   s +="OR ";
    						break;
    					case NOT:
    					   s +="NOT ";
    						break;
    					}
    					break;
    			case OpIndex::BASIC:
    				val = op->value();
    				s += to_string(val)+" ";
    				break;
    			case OpIndex::DRVD:
    				val = op->value();
    				//val = op->value();
    				s += to_string(val)+" ";
    				break;   				
    		}
	}
    return s;
}
