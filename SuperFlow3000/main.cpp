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
	std::cout << "#### SuperFlow3000 ####\n";
	char InputFileName[] = "inputvals.bin";
	char OutputFileName[] = "output.txt";
	// load simparam
	IO Reader(InputFileName,OutputFileName);
	Simparam simparam = Reader.getSimparam();

	// initialize grids
	const MultiIndexType griddimension(8,8);
	//const MultiIndexType griddimension (simparam.iMax+2,simparam.iMax+2);
	MultiIndexType grid_ll(0,0); //lower left
	MultiIndexType grid_ur(7,7);
	GridFunction u(griddimension,simparam.UI);
	GridFunction v(griddimension,simparam.UV);
	GridFunction p(griddimension,simparam.PI);

	// set some values inside the grid
	MultiIndexType MIT1(1,2);
	MultiIndexType MIT2(3,5);
	u.SetGridFunction(MIT1,MIT2, 5);
	u.PlotGrid();

	const PointType delta(simparam.xLength/simparam.iMax , simparam.yLength/simparam.jMax);
	//int step = 1;
	//std::cout << simparam.xLength<<"/"<< simparam.iMax<<"="<< delta[0];
	//Reader.writeVTKFile(griddimension,u.GetGridFunction(),v.GetGridFunction(), p.GetGridFunction(), delta, step);
	return 0;

	// ToDo Liste
	/*
	 * Stencil testen - kommen die richtigen Matrizen raus?
	 * Visualisierung ausprobieren
	 * Zeititeration
	 * Solver testen?
	 * Computation fertig machen
	 * Stencil andere Ableitungen
	 */

	/*// Teste Apply Grid Function
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

