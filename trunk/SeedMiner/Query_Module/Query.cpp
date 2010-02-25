#include "Query.h"

int main(int argc, char **argv){
	Query result;
	Query q1;
	q1.x=1;
	Query q2;
	q2.x=2;
	Query q3;
	q3.x=3;
	result=q1(0,0)&q2(1,0)|q3(2,0)|q2(1,1)&q3(2,2);
	cout<<result.x<<endl;
	system("pause");
}