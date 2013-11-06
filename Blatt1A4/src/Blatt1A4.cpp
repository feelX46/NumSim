//============================================================================
// Name        : Blatt1A4.cpp
// Author      : AFM
// Version     :
// Copyright   : no copyright to harm
// Description : Main for SuperFlow3000, C++, Betriebssystem-unabhängig
//============================================================================

#include <iostream>
using namespace std;
#include "IO.hpp"

int main() {
	cout << "Blatt1A4" << endl;				//
    char input_[] = "inputvals.txt";
	char *pointer1 = input_;
	char* pointer2 = 0;
	IO Objekt(pointer1, pointer2);		// how to allokate the object
	return 0;
}
