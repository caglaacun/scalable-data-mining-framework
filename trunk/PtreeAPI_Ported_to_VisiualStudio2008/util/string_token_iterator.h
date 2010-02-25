#ifndef STRING_TOKEN_ITERATOR
#define STRING_TOKEN_ITERATOR

#include <string>
#include <iterator>

using namespace std;

class string_token_iterator
{
	public:
 		typedef std::string					value_type;
		typedef ptrdiff_t                     	difference_type;
		typedef const std::string*       pointer;
		typedef const std::string&      reference;
		typedef input_iterator_tag		iterator_category;
  
  
		string_token_iterator():str(0), start(0), end(0){}
  		string_token_iterator(const std::string & str_, const char * separator_= " ") 
  			: str(&str_), separator(separator_), end(0)
  		{
    		find_next();
  		}

		string_token_iterator(const string_token_iterator& rhs) :
			str(rhs.str), separator(rhs.separator), start(rhs.start), end(rhs.end)
		{}

		string_token_iterator& operator ++()
		{
    		find_next();
    		return *this;
  		}

  		string_token_iterator operator ++(int)
  		{
    		string_token_iterator temp(*this);
      		++(*this);
      		return temp;
  		}

  		std::string operator*()const
  		{
    		return std::string(*str, start, end - start);
  		}

  		bool operator==(const string_token_iterator & rhs)const
  		{
    		return (rhs.str == str && rhs.start == start && rhs.end == end);
  		}

  		bool operator !=(const string_token_iterator & rhs)const
  		{
    		return !(rhs == *this);
  		}

	private:
  		void find_next()
  		{
    		start = str->find_first_not_of(separator, end);
    		if(start == std::string::npos)
    		{
	     		start = end = 0;
			    str = 0;
      			return;
    		}
    		end = str->find_first_of(separator, start);
  		}
  
  		const char *separator;
  		const std::string *str;
  		std::string::size_type start;
  		std::string::size_type end;
};

#endif
