#include "Item.h"
#include <sstream>
//initialize the static variable
int Item::ref_count = 0;

Item::Item() {
	//cout<<"Calling Item()"<<"ref_count: "<< ref_count<<endl; 
	ref_count++;
        //cout<<"Item()"<<"new ref_count: "<< ref_count<<endl; 
        
}

Item::~Item() {
    //cout<<"calling ~Item()"<<endl;
    ref_count--;
    //cout<<"ref_count: "<< ref_count<<endl; 
}
////////////////////////////////////////////////////////////////

 
UsignIntItem:: UsignIntItem(unsigned val) : value_(val)
{}

int UsignIntItem::type()const{
    return Type::UNSIGNED_INT;
}

unsigned UsignIntItem::value()const{
    return value_;
}

Item* UsignIntItem::copy(){
	return new UsignIntItem(value_);
}

string UsignIntItem::to_string()const{
    unsigned v = value_;
    stringstream strm;
    strm<<v;
    string str;
    strm>>str;
    return str;
}


////////////////////////////////////////////////////////////////


SignIntItem:: SignIntItem(int val) : value_(val)
{}

int SignIntItem::type()const{
    return Type::SIGNED_INT;
}

int SignIntItem::value()const{
    return value_;
}

Item* SignIntItem::copy(){
	return new SignIntItem(value_);
}
string SignIntItem::to_string()const{
    int v = value_;
    stringstream strm;
    strm<<v;
    string str;
    strm>>str;
    return str;
}


////////////////////////////////////////////////////////////////

UsignDoubleItem::UsignDoubleItem(double val) : value_(val){}

int UsignDoubleItem::type()const{
    return Type::UNSIGNED_DOUBLE;
}

double UsignDoubleItem::value()const{
    return value_;
}

Item* UsignDoubleItem::copy(){
    return new UsignDoubleItem(value_);
}

string UsignDoubleItem::to_string()const{
    double v = value_;
    stringstream strm;
    strm<<v;
    string str;
    strm>>str;
    return str;
}


////////////////////////////////////////////////////////////////

SignDoubleItem::SignDoubleItem(double val) : value_(val){}

int SignDoubleItem::type()const{
    return Type::SIGNED_DOUBLE;
}

double SignDoubleItem::value()const{
    return value_;
}

Item* SignDoubleItem::copy(){
	return new SignDoubleItem(value_);
}

string SignDoubleItem::to_string()const{
    double v = value_;
    stringstream strm;
    strm<<v;
    string str;
    strm>>str;
    return str;
}


////////////////////////////////////////////////////////////////

SingCatItem ::SingCatItem(const string& val) : value_(val) {}

int SingCatItem::type()const{
    return Type::SING_CAT;
}

string SingCatItem ::value()const{
    return value_;
}

Item* SingCatItem::copy(){
	return new SingCatItem(value_);
}

string SingCatItem::to_string()const{
    return value_;
}


////////////////////////////////////////////////////////////////

MultiCatItem ::MultiCatItem(const vector<string>& value)
    : value_(value){}
    
int MultiCatItem::type()const{
    return Type::MULT_CAT;
}

vector<string> MultiCatItem ::value()const{
    return value_;
}

Item* MultiCatItem::copy(){
	vector<string> tmp_value(value_.begin(), value_.end());
	return new MultiCatItem(tmp_value);
}
string MultiCatItem::to_string()const{
    string tmp = "";
    for(int i; i<value_.size()-1; i++){
        tmp +=value_[i]+":";
    }
    tmp +=value_[value_.size()-1];
    
    return tmp;
}


////////////////////////////////////////////////////////////////

SkipItem::SkipItem(){}

int SkipItem::type()const{
    return Type::SKIP;
}

Item* SkipItem::copy(){
	new SkipItem;
}

string SkipItem::to_string()const{
    return "skip";
}


///////////////////////////////////////////////////////////////
//Global function to compare Items
bool operator==(const Item& first, const Item& second){
    if(first.type()!=second.type()) return false;
    return first.to_string()==second.to_string();
    //return first.equal(second);
}
bool operator!=(const Item& first, const Item& second){
    return !(first==second);
}
