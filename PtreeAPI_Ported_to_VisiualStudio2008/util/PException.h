/// 
// File:   PException.h
// Author: serazi
//
// Created on October 9, 2003, 1:38 PM
//

#ifndef _PException_H
#define	_PException_H
#include <string>
#include <stdexcept>
#include <boost/lexical_cast.hpp>
using namespace std;
/* An exception class to handle out of domain exception. OutOfDomainException 
 * will be thrown when a value of attribute does not belong to the defined 
 * domain of that attribute.
 *
 * derived from standard exception class out_of_rtange.  
 */
class OutOfDomainException : public std::out_of_range {
    public:
        explicit OutOfDomainException(const string& msg): out_of_range(msg){}
        
        explicit OutOfDomainException(double val, double low, double up)
        : out_of_range("Given value: " + boost::lexical_cast<string> (val) 
        	+ " is out of domain: [" +boost::lexical_cast<string>(low)
        	+","+boost::lexical_cast<string>(up)+"]")
        {
        } 
};

////template <class T>
//class DomainException : public std::out_of_range {
//    public:
//        DomainException(/*double val, double low, double up*/const string& msg)
//        	: out_of_range(msg)
//        
//        {
//        	//msg_ = "IntegerDomainException";
//        	
//        	/*msg_ = "Given value: " + boost::lexical_cast<string> (val) 
//        	+ " is out of domain: [" +boost::lexical_cast<string>(low)
//        	+","+boost::lexical_cast<string>(up)+"]";*/
//        	//cout<<"DomainException"<<endl;
//        }
////         virtual const char* what() const throw(){	
////        	cout<<"what()"<<endl;
////        	return "IntegerDomainException";//msg_.c_str();
////        }
////    private:
////     	string msg_;
//};
/**
 * An exception class to handle invalid length between bitset and the domain
 *
 * derived from standard exception length_error
 */
class InvalidLengthException : public length_error {
    public:
        explicit InvalidLengthException(const string& msg): length_error(msg){} 
};

/**
 * If item type amd attribute types double not match
 */
class InvalidTypeException : public logic_error {
    public:
        explicit InvalidTypeException(const string& msg): logic_error(msg){}
};

/**
 * If item type and attribute types double not match
 */
class IOException : public runtime_error {
    public:
        explicit IOException(const string& msg): runtime_error(msg){}
};
/**
 * If item type amd attribute types double not match
 */
class OutOfRangeException : public length_error {
    public:
        explicit OutOfRangeException(const string& msg): length_error(msg){}
};
;
/**
 * If item type amd attribute types double not match
 */
class FormatException : public length_error {
    public:
        explicit FormatException(const string& msg): length_error(msg){}
};

class XMLException : public std::logic_error{

public:
  explicit XMLException(const std::string& s) : std::logic_error(s) {};
 
};
#endif	/* _PException.h_H */

