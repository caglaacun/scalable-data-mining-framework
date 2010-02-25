#ifndef UTIL_H
#define UTIL_H
 

#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <sstream>
using namespace std;
typedef long long int block_type;
typedef boost::dynamic_bitset<> PTree;
typedef boost::dynamic_bitset<>::size_type size_type; 
typedef boost::dynamic_bitset<>::size_type count_type;  

void serialize(ostream& out, PTree const& pt)throw();       
bool deserialize(istream& in, PTree &pt)throw();

void str_serialize(ostream& out, const std::string &s)throw();
bool str_deserialize(istream& in, std::string & s)throw();

template <typename T>
string to_string(T value){
    string str;
    stringstream strm;
    strm << value;
    strm >> str;

    return str;
}
//string to_string(double value);
string to_string(const bit_vector & v);

#endif

