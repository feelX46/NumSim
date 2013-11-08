/*
 * main.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: ischinger
 */
#include <iostream>
#include "Misc/template.h"
#include "Misc/typedef.h"
#include "IO/IO.hpp"
#include "Stencil/stencil.h"
#include "Grid/Gridfunction.h"

int main(){
	char InputFileName[] = "inputvals.bin";
	char OutputFileName[] = "output.txt";
	IO Reader(InputFileName,OutputFileName);
	return 0;
}

