//============================================================================
// Name        : Programme1.cpp
// Author      : Aaron Kr�mer
// Version     :
// Copyright   : no copyright
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "math.h"
using namespace std;

int main() {
	//test
	float wert1, wert2;
	cout << "gib n wert ein" << endl;
	cin>>wert1;
	cout << "gib n gr��eren wert ein" << endl;
	cin>>wert2;
	for(int i = int(ceil(wert1)) ;i < floor(wert2); i++)
		cout << i << "\n";
	return 0;
}
