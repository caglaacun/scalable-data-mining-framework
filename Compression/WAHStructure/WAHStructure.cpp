// WAHStructure.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "boost\dynamic_bitset\dynamic_bitset.hpp"
#include "Structure2.h"
#include "Math.h"

using namespace boost;
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{	
	
	Structure2 * st1 = new Structure2();
	Structure2 * st2 = new Structure2();

	dynamic_bitset<> x(67);
	for (int i = 0 ; i < x.size() ; i++)
	{
		x[i] = 1;
	}
	st1->buildArray(x);
	x[30] = 0;
	st2->buildArray(x);
	st1->compressWords();
	st2->compressWords();
	st1->printCompressedStream();
	st2->printCompressedStream();
	
	Structure2 * st3 = *st1 | *st2;
	st3->printCompressedStream();
//	int spaces = st3->getSpaceUtilisation();
//cout << spaces << endl;
	
	
}
