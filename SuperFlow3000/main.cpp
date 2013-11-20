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
#include "Computation/computation.h"

int main(){
	std::cout << "#### SuperFlow3000 ####\n";
	char InputFileName[] = "inputvals.bin";
	char OutputFolderName[] = "output";  // output folder! -> be careful, if folder is not there, no data are saved
	// load simparam
	IO Reader(InputFileName,OutputFolderName);
	Simparam simparam = Reader.getSimparam();

	// initialize grids
    MultiIndexType griddimension (simparam.iMax+2,simparam.jMax+2);
    GridFunction p(griddimension,simparam.PI);
    GridFunction rhs(griddimension);
    GridFunction v(griddimension,simparam.UV);
    GridFunction G(griddimension);
    GridFunction u(griddimension,simparam.UI);
    GridFunction F(griddimension);


    MultiIndexType bb(1,1); //lower left
    MultiIndexType ee(simparam.iMax,simparam.jMax); //upper right

	const PointType delta(simparam.xLength/simparam.iMax , simparam.yLength/simparam.jMax);

	RealType deltaT;
	RealType t = 0;
	int step = 0;

	// write first output data
	Reader.writeVTKFile(griddimension,u.GetGridFunction(),v.GetGridFunction(), p.GetGridFunction(), delta, step);

	// start time loop
	while (t <= simparam.tEnd){

		// compute deltaT
		deltaT = computeTimestep(u.MaxValueGridFunction(bb,ee),v.MaxValueGridFunction(bb,ee),delta,simparam.RE,simparam.tau);
		// set boundary
		setBoundaryU(u); //First implementation: only no-flow boundaries-> everything is zero!
		setBoundaryV(v);

	    // compute F / G


		// set right side of pressure equation


		// solver

			// set some values inside the grid
			IndexType a = step;
			MultiIndexType MIT1(a,a);
			u.SetGridFunction(MIT1,MIT1, 5);

		// update time
		t += deltaT;
		step++;

		// write files
		Reader.writeVTKFile(griddimension,u.GetGridFunction(),v.GetGridFunction(), p.GetGridFunction(), delta, step);
		std::cout<< step<<std::endl;
	}

	// ToDo Liste
	/*
	 * Stencil testen - kommen die richtigen Matrizen raus?
	 * Solver testen?
	 * Computation fertig machen
	 * Stencil andere Ableitungen
	 * grid wieder zum laufen bekommen
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
*/	std::cout<<"laeuft... immerhin ;)";
	return 0;
}

