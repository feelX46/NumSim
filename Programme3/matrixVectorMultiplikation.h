/*
 * matrixVectorMultiplikation.h
 *
 *  Created on: 31.10.2013
 *      Author: Aaron
 */

#ifndef MATRIXVECTORMULTIPLIKATION_H_
#define MATRIXVECTORMULTIPLIKATION_H_

#include <iostream>
using namespace std;

void Ax(float *matrix, float *vector, float *result, const int n){
	for(int i = 0; i < n; i++)
	{
	    for(int j = 0; j < n; j++){
	    	result[i] += matrix[i*n+j]*vector[j];
	    }
	}
};

void writeMatrix(float *matrix, const int n){
	for(int i = 0; i < n; i++){
		cout<<"|";
		for(int j = 0; j < n; j++){
			cout << matrix[i*n+j];
			if(j<n-1)
				cout <<"\t";
	    }
		cout<<"|"<<endl;
	}

};

void writeVector(float *vector, const int n){
	for(int i = 0; i < n; i++){
	    cout <<"|"<< vector[i] <<"|"<<endl;
	}
};

#endif /* MATRIXVECTORMULTIPLIKATION_H_ */
