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
#include "Grid/gridfunction.h"

int main(){
	char InputFileName[] = "inputvals.bin";
	char OutputFileName[] = "output.txt";
	IO Reader(InputFileName,OutputFileName);
	return 0;

	/*// Teste Apply Grid Function
	MultiIndexType gridreadbegin;
	gridreadbegin[0]=0;	gridreadbegin[1]=0;

	MultiIndexType gridreadend;
	gridreadend[0]=5; gridreadend[1]=5;

	MultiIndexType gridwritebegin;
	gridwritebegin[0]=0; gridwritebegin[1]=0;

	MultiIndexType gridwriteend;
	gridwriteend[0]=3; gridwriteend[1]=3;

	GridFunction sourcegridfunction;

	GridFunction imagegridfunctinon;

	ApplyStencilOperator(gridreadbegin,
				gridreadend,
				gridwritebegin,
				gridwriteend,
				sourcegridfunction,
				imagegridfunction);
*/
}

