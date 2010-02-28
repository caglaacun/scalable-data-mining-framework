#ifndef OLAP
#define OLAP

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "DataExtraction.h"
#include "Query.h"

class Olap
{
public:
	int count(Query* query)
	{
		srand ( time(NULL) );
		bitstring result;
		return (rand() % 150 + rand() % 250);
	}
};
#endif