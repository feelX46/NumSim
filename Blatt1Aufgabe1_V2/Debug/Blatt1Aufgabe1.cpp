/*
 * Blatt1Aufgabe1.cpp

 *
 *  Created on: 31.10.2013
 *      Author: felix
 */

#include <iostream>
#include "math.h"
using namespace std;

int main(){
	float wert1, wert2;
	cout << "gib n wert ein" << endl;
	cin>>wert1;
	cout << "gib n größeren wert ein" << endl;
	cin>>wert2;
	for(int i = int(ceil(wert1)) ;i < floor(wert2); i++)
		cout << i << "\n";
	return 0;
}

