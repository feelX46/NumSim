/*
 * main.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: ischinger
 */
#include <iostream>
#include "template.h"
#include "typedef.h"
#include "IO.hpp"
//#include "stencil.h"

int main(){
	char InputFileName[] = "inputvals.bin";
	char OutputFileName[] = "output.txt";
	IO Reader(InputFileName,OutputFileName);
	return 0;
}

