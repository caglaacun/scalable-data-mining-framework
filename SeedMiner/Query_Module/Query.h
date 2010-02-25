#pragma once
#include <iostream>
using namespace std;
class Query
{
public:
	Query()
	{
		//cout<<"constructor"<<endl;
	}

	~Query()
	{
		//cout<<"destructor"<<endl;
	}

	Query operator()(int a,int b)
	{
		cout<<"operator ()"<<endl;
// 		Query q;
// 		q.x=x;
		return *this;
	}

	Query operator&(Query rhs)
	{
		cout<<"operator &"<<endl;
		Query q;
		q.x=x*rhs.x;
		return q;
	}

	Query operator|(Query rhs)
	{
		cout<<"operator |"<<endl;
		Query q;
		q.x=x+rhs.x;
		return q;
	}

	Query& operator= (const Query& rhs)
	{
		cout<<"operator ="<<endl;
		x=rhs.x;
		return *this;
	}

	int x;
};

