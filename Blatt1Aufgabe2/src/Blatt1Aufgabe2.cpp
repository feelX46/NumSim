//============================================================================
// Name        : Blatt1Aufgabe2.cpp
// Author      : AFM
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

int fib(int n) {
	if (n==1 || n==2)
		return 1;
	else
		return fib(n-1)+fib(n-2);

}


int main() {
	cout << "Gib Index fuer Fibonacci Zahl ein: "; // prints Hello World!!!
	int n;
	cin >> n;
	cout << "Die zugehoerige Fibonaccie Zahl ist: " << fib(n);
	return 0;
}
