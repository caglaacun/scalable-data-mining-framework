// 
// File:   RelationalDataFeeder.cc
// Author: serazi
//
// Created on October 22, 2003, 11:31 AM
//

#include "RelationalDataFeeder.h" 
#include <cstring>
#include <cstdlib>  
//
// Constructor
///
RelationalDataFeeder::RelationalDataFeeder(const MetaFileParser& parser)                        
	:tuple_counter(0)  
{
	attInfo = parser.getAttributeInfo();
	delimiter_ = parser.getDelimiter(); 
	cardinality_ = parser.getCardinality();
	cat_delimiter_ = parser.getCatDelimiter(); 
	meta_file = parser.getMetaFileName();
	vector<AttributeInfo*>::iterator it = attInfo.begin();                      
	string file_name = "";
	int atrib_cnt = 0;
	
	for(;it!=attInfo.end();++it){
		string current_file_name =parser.getAttributeFile((*it)->name());
		//cout<<current_file_name<<endl;
		if (current_file_name!=file_name)
		{
			files.push_back(current_file_name);
			file_name=current_file_name;
			if ( it!=attInfo.begin() ) att_counts.push_back(atrib_cnt); 
			atrib_cnt=0;
		}
		atrib_cnt++;	
	}
	att_counts.push_back(atrib_cnt);
	instream_ = new fstream[files.size()];
	for(int i=0; i<files.size(); i++){
		instream_[i].open(files[i].c_str(), ios::in);  
		if(!instream_[i])
			throw IOException("Error when opening file "+ files[i]); 
	}	
}

RelationalDataFeeder::RelationalDataFeeder(const string& metafile)
	throw(XMLException){
	try {
		MetaFileParser parser(metafile);
	}
	catch(const XMLException& e){
		throw;
	}
}
//
// Destructor
//
RelationalDataFeeder::~RelationalDataFeeder()  
{
	for(int i=0; i<files.size(); i++){
		instream_[i].close();  
	}
	delete []instream_;
}

Tuple RelationalDataFeeder::getTuple() throw (OutOfRangeException, InvalidLengthException)
{	
	//cout<<tuple_counter<<" "<<cardinality_<<endl;  
	if(tuple_counter >= cardinality_) 
		throw OutOfRangeException("Asking for a tuple beyond the end at: " + tuple_counter );
	vector<string> str_items;
	int j = 0; 
	for(int i=0; i<files.size(); i++){
		string line("");
                //cout<<"Reading line: "<<i<<endl;
		getline(instream_[i], line);
		//cout<<line<<endl;
		string_token_iterator b(line, delimiter_), e;
		vector<string> tmp(b, e);
		//vector<string>::iterator n=str_items.end();
		str_items.resize(str_items.size()+tmp.size());	
		copy(tmp.begin(), tmp.end(), str_items.end() - tmp.size()); 	
	}
	//copy(str_items.begin(), str_items.end(), ostream_iterator<string>(cout, " "));
	//cout<<endl;
	tuple_counter++;
	try{  
		//cout<<convert(str_items)<<endl; 
		return convert(str_items);
	}
	catch(InvalidLengthException e){
	 throw;
	}			
}

/**
  * create the ptree info set cardinality and return it
  */ 
PTreeInfo RelationalDataFeeder::getPTreeInfo(){
	//cout<<"getPTreeInfo() has been called.."<<endl;
	PTreeInfo pi;
	vector<AttributeInfo*>::iterator it = attInfo.begin();
	for(;it!=attInfo.end();++it){
		//cout<<"Type in getPTreeInfo() "<<(*it)->type()<<endl;
		if((*it)->type() != Type::SKIP) pi.addAttributeInfo(*it);
	}
	pi.setCardinality(cardinality_);  
	return pi;
}

std::size_t RelationalDataFeeder::getCardinality() const{        
	return cardinality_;
}
/**
  * 
  */  
Tuple RelationalDataFeeder::convert(const vector<string>& str_items)
	 throw (FormatException, InvalidLengthException){
	//cout<<"What is this:" <<str_items.size() <<" "<<attInfo.size()<<endl;	
	if(str_items.size() != attInfo.size())
		throw  InvalidLengthException("Number of item mismatch...,");           	
	//attInfo = parser_.getAttributeInfo();	
	vector<AttributeInfo*>::iterator it = attInfo.begin(); 
	size_t i = 0;
	Tuple tp;	
	//cout<<"Going to for loop:"<<endl;           
	for(; it!=attInfo.end();++it, i++){
		AttributeInfo* att = *it;  
		//cout<<"Before type...."<<endl;
		//cout<<attInfo[i]->name()<<endl;
		int t =  att->type();
		//cout<<"Type: "<<t<<endl;
		double d;
		unsigned n;
		vector<string> vals;
		string val_string;
		char * endAdd;
		//const char* del = "\t";
		string s;  
		string_token_iterator b, e;
		//cout<<str_items[i]<<":"<<t<<endl;
		switch(t){
			case Type::UNSIGNED_INT:
				//cout<<str_items[i]<<":"<<t<<endl;
				n = static_cast<unsigned>(atoi(str_items[i].c_str())); 
				tp.add(new UsignIntItem(n));
				break;
			case Type::SIGNED_INT:
			 //cout<<str_items[i]<<":"<<t<<endl;
				tp.add(new SignIntItem(atoi(str_items[i].c_str())));                          
				break;
			case Type::UNSIGNED_DOUBLE:
			       //cout<<str_items[i]<<":"<<t<<":";			
				d = strtod(str_items[i].c_str(), &endAdd);
                                //cout<<d<<endl;
                                
				tp.add(new UsignDoubleItem(d));
                                //cout<<"double item value: "<<(new UsignDoubleItem(d))->value()<<endl;
				break;		 
			case Type::SIGNED_DOUBLE:
			 //cout<<str_items[i]<<":"<<t<<endl;
				d = strtod(str_items[i].c_str(), &endAdd);
				tp.add(new SignDoubleItem(d));
				break;
			case Type::SING_CAT:
			 //cout<<str_items[i]<<":"<<t<<endl;
				tp.add(new SingCatItem(str_items[i]));   
				break;
			case Type::MULT_CAT:
			    //cout<<str_items[i]<<":"<<t<<endl;
				b = 	string_token_iterator(str_items[i], cat_delimiter_);
				vals = vector<string>(b, e);  
			    tp.add(new MultiCatItem(vals));
				break;					
			case Type::SKIP:
				break;
		}
		//cout<<str_items[i]<<":"<<t<<endl;
	}
	//cout<<"Converting done..."<<endl;
        //cout<<"Converted tuple:"<<tp<<endl;
	return tp;	 
}
std::string RelationalDataFeeder::getMetaFileName()const{
	return meta_file;
}

