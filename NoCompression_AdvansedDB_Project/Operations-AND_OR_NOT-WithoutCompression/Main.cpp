#include <iostream>
#include "BasicOperations.h"
#include "boost/timer.hpp"

using namespace std;
using namespace boost;
int main(int argc, char **argv)
{
	int NUMBER_OF_ELEMENTS = 10000000;

	timer time;
	BasicOperations bo;

	dynamic_bitset<> b1(NUMBER_OF_ELEMENTS);
	dynamic_bitset<> b2(NUMBER_OF_ELEMENTS);

	time.restart();
	bo.doAndOperation(b1,b2);
	cout << "AND operation time: " << time.elapsed()*1000 << '\n';


	time.restart();
	bo.doOrOperation(b1,b2);
	cout << "OR operation time: " << time.elapsed()*1000 << '\n';


	time.restart();
	bo.doNotOperation(b1);
	cout << "NOT operation time: " << time.elapsed()*1000 << '\n';

	system("pause");
}