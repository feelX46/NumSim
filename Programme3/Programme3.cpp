/*
 * Programme3.cpp
 *
 *  Created on: 31.10.2013
 *      Author: Aaron
 */
/*
 * Problematik momentan bei mir auf dem PC: Programm reagiert ab $n \approx 750$ nicht mehr
 * hoffe, es liegt an meinem PC und es läuft bei einem von euch
 *
 */
#include <iostream>
#include "math.h"
#include "matrixVectorMultiplikation.h"

int main(void){
	int n;
	cout<<"Wie groß soll die Matrix sein?"<< endl;
	cin >> n;
	float matrixA[n*n];
	float vectorb[n];
	float vectory[n];

	//generate matrix and vector SOMEHOW
	for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
        	 matrixA[i*n+j] = 1.0 +
        	 (float(i+2)+float(j)*float(i-1))*cos((float(n)/2.0-float(i))/float(n)*3.14159);
		}
        vectorb[i]= - float(n+i);
    }

	for(int i = 0; i < n; i++)
	{
	    vectory[i]= 0.0;
	}

	cout << "A ="<<endl;
	writeMatrix(matrixA, n);

	cout << "b ="<<endl;
	writeVector(vectorb, n);

	cout << "A*b = y ="<<endl;
	Ax(matrixA, vectorb, vectory, n);
	writeVector(vectory, n);

    return 0; //
}

