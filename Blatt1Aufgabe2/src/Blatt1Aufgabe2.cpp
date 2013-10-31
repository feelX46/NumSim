/*
 * Programme2.cpp
 *
 *  Created on: 31.10.2013
 *      Author: Aaron
 */

#include <iostream>

using namespace std;

int FibonacciZahl(int in){
	if(in > 1){
		return (FibonacciZahl(in - 1) + FibonacciZahl(in - 2));
	}
	else if (in == 1)
		return 1;
	else
		return 0;
};

int main(){
	int eingabe;
	cout<<"Geben Sie eine natürliche nichtnegative Zahl ein"<<endl;
	cin >>eingabe;
	if (eingabe < 0)
		cout<<"Eingabe nicht erlaubt";
	else
	    cout<<"Die Fibonacci-Zahl zu " <<eingabe<<" ist "<<FibonacciZahl(eingabe)<<endl;

	return 0;
}
