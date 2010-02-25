#include <cmath>
#include <iostream>
#include "BinaryMath.h"

using namespace std;
/**
 * calculate maximum number of bits required to represent 
 * an integer number
 */
int numbits(int num){
	return (num == 0 || num == 1) ? 1 
        : static_cast<int>(ceil(log(abs(num * 1.0))/log(2.0)));
}


/*int main(int argc, char **argv){
	double x = 110.00;
//	for(int i=0; i<9;i++){
//		((x<<i) & 0x1 ) == 1 ? cout<<1:cout<<0;
//	}
	cout<<static_cast<unsigned long>(x)<<endl;
	return (EXIT_SUCCESS);
}*/
