#ifndef BASIC_PT
#define BASIC_PT

#include <vector>
#include <climits>
//#include<stack>
#include<queue>
#include "Util.h"
#include "PTreeInfo.h"
#include "OpIndex.h"

/*
* This class will be capable of generating required 
* Ptree / Operator patterns required for the count method
* in PtreeSet
* 
* This class can be used to manipulate the Ptrees with Ptree operations
* The Ptree operations can be expressed using logical expressions
*
* eg if count for 1011 is required, the pattern canbe expressed as
* p(1,0) & p(1,1) & !p(1,2) & p(1,3)
* 
* Derived Ptrees are indicated with p(index)
* Logical operator presedence applies. parenthis can be used. 
*
* Required all operators to return Pattern/pt
* 
*/

using namespace std;

#define  AND	9999//UINT_MAX
#define  OR	8888//UINT_MAX-1 
#define  NOT 	7777//UINT_MAX-2 

class BasicPt{
public:
	BasicPt();
	/**
	* Constructor with PTreeInfo should be used to 
	*/   
	BasicPt(const PTreeInfo& pi);
	/**
	* Copy constructor for derived ptrees 
	*/
	BasicPt(const int size);        

	/**
	* Copy constructor for BasicPtrees  
	*/
	BasicPt(const BasicPt& other);
	/**
	* assignment operator
	*/       
	BasicPt& operator=(const BasicPt& other);
	/**
	* dtor
	*/  
	~BasicPt();

	/**
	* Operator to indicate Basic ptree 
	*/ 
	BasicPt operator()(int a, int b);

	/**
	* Operator to indicate derived Ptree  
	*/ 
	BasicPt operator()(int a);
	BasicPt operator&(const BasicPt& p1);
	BasicPt operator|(const BasicPt& p1);
	BasicPt operator !();

	size_t size()const;
	OpIndex* BasicPt::getOpIndex(const size_t i)const;
	void print()const;
	string toString()const;
private:
	PTreeInfo pi_; 
	vector <OpIndex*> pfcont;
};
#endif
