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
	GridFunction sourcegridfunction(5,5);
	return 0;

	// ToDo Liste
	/*
	 * Stencil testen - kommen die richtigen Matrizen raus?
	 * Visualisierung ausprobieren
	 * Zeititeration
	 * Destruktor GridFunction
	 * Solver testen?
	 * Computation fertig machen
	 * Stencil andere Ableitungen
	 */

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

