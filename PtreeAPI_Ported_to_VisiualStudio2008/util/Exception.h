#ifndef _EXCEP_H_
#define _EXCEP_H_
#include <stdexcept>
#include <boost/lexical_cast.hpp>

using namespace std;

/**
 * All Exception classes
 * Author: Masum H. Serazi
 */

class NotFoundException : public runtime_error{
   public:
      NotFoundException(const string& what) throw() :runtime_error(what)
   { }
};

class FileFormatException : public runtime_error{
   public:
      FileFormatException(const string& what) throw() :runtime_error(what)
   { }
};

class InvalidMaskException : public runtime_error{
   public:
      InvalidMaskException(const string& what) throw() :runtime_error(what)
   { }
};

class InvalidPatternException : public runtime_error{
   public:
      InvalidPatternException(const string& what) throw() :runtime_error(what)
   { }
};


class IndexOutOfBoundsException : public runtime_error{
   public:
      IndexOutOfBoundsException(const string& what) throw() :runtime_error(what)
   { }
   
     IndexOutOfBoundsException(int size, int i) throw()
     	: runtime_error("ERROR: Maximum index: " + boost::lexical_cast<string>(size) +
     		"given index: " + boost::lexical_cast<string>(i))
     {}	
};


class IllegalArgumentException : public runtime_error{
   public:
      IllegalArgumentException(const string& what) throw() :runtime_error(what)
   { }
};

class InvalidIdException: public runtime_error{
   public:
   InvalidIdException(const string& what) throw() :runtime_error(what)
   { }
};

class InvalidIndexException: public runtime_error{
   public:
   InvalidIndexException(const string& what) throw() :runtime_error(what)
   { }
};

class InvalidNameException: public runtime_error{
   public:
   InvalidNameException(const string& what) throw() :runtime_error(what)
   { }
};
class IdExistsException: public runtime_error{
    public:
    IdExistsException(const string& what) throw() :runtime_error(what)
    { }
};

class InvalidOptionException: public runtime_error{
    public:
    InvalidOptionException(const string& what) throw() :runtime_error(what)
    { }
};

class NotValidStringException: public runtime_error{
    public:
    NotValidStringException(const string& what) throw() :runtime_error(what)
    { }
};

class InvalidSizeException: public runtime_error{
    public:
    InvalidSizeException(const string& what) throw() :runtime_error(what)
    { }
};


class TypeMismatchException: public runtime_error{
    public:
    TypeMismatchException(const string& what) throw() :runtime_error(what)
    { }
};



#endif
