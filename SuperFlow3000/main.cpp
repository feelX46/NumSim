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
#include "Solver/solver.h"


int main(int argc, char *argv[]){
	std::cout << "#### SuperFlow3000 ####\n";
	// Initialize MPI
	//MPI_Init(&argc, &argv);

	char InputFileName[] = "inputvals.bin";
	char OutputFolderName[] = "output";  // output folder! -> be careful, if folder is not there, no data are saved
	// load simparam
	IO Reader(InputFileName,OutputFolderName);
	Simparam simparam = Reader.getSimparam();
	Computation pc (simparam);
	Solver sol (simparam);
	// initialize grids
    MultiIndexType griddimension (simparam.iMax+2,simparam.jMax+2);
    GridFunction p(griddimension,simparam.PI);
    GridFunction rhs(griddimension);
    GridFunction v(griddimension,simparam.VI);
    GridFunction g(griddimension);
    GridFunction u(griddimension,simparam.UI);
    GridFunction f(griddimension);

    MultiIndexType bb(1,1); //lower left
    MultiIndexType ee(simparam.iMax,simparam.jMax); //upper right

	const PointType h(simparam.xLength/simparam.iMax , simparam.yLength/simparam.jMax);

	RealType deltaT = simparam.deltaT;
	RealType t = 0;
	int step = 0;
	GridFunction gx(griddimension,simparam.GX);
	GridFunction gy(griddimension,simparam.GY);

	//---- for Boundary condition ----
	//for driven cavity
	MultiIndexType upperleft (1,                 griddimension[1]-1);
	MultiIndexType upperright(griddimension[0]-2,griddimension[1]-1);
	MultiIndexType offset (0,-1);

	MultiIndexType linksunten (0,1);
	MultiIndexType linksoben  (0,griddimension[1]-2);
	MultiIndexType rechtsunten (griddimension[0]-2,1);
	MultiIndexType rechtsoben  (griddimension[0]-2,griddimension[1]-2);

	// write first output data
//	Reader.writeVTKFile(griddimension,u.GetGridFunction(),v.GetGridFunction(), p.GetGridFunction(), h, step);


	// parallel writing routine
	//ToDo: einkommentieren!
	int ibegin = 2;//p.beginwrite[0];
	int iend   = 126;//p.endwrite[0];
	int jbegin = 2;//p.beginwrite[1];
	int jend   = 126;//p.endwrite[1];
	int localgriddimensionX = iend-ibegin+1;
	int localgriddimensionY = jend-jbegin+1;
	int mpiSizeH = 1;
	int mpiSizeV = 1;
	int rank = 0;
	if (rank == 0)
		 Reader.writeVTKMasterfile(mpiSizeH, mpiSizeV, step, localgriddimensionX, localgriddimensionY);
	Reader.writeVTKSlavefile(u, v,  p, h, mpiSizeH, mpiSizeV, step,rank);

	// start time loop
	while (t <= simparam.tEnd){
		step++;
		// compute deltaT
		deltaT =  pc.computeTimestep(u.MaxValueGridFunction(bb,ee),v.MaxValueGridFunction(bb,ee),h);
		//std::cout<<"deltat = "<<deltaT<<std::endl;

		// set boundary
		pc.setBoundaryU(u); //First implementation: only no-flow boundaries-> everything is zero!
		pc.setBoundaryV(v);
		// driven cavity:
		 u.SetGridFunction(upperleft,upperright,-1.0,offset,2.0);
		//einfach durchfliesen
		//u.SetGridFunction(linksunten,linksoben,1);
		//u.SetGridFunction(rechtsunten,rechtsoben,1);

		//u.PlotGrid();
		 if (0 == (step % 10)) {
		 	 //Reader.writeVTKFile(griddimension,u.GetGridFunction(),v.GetGridFunction(), p.GetGridFunction(), h, step);
		 	 if (rank == 0)
		 		 Reader.writeVTKMasterfile(mpiSizeH, mpiSizeV, step, localgriddimensionX, localgriddimensionY);
		 	 Reader.writeVTKSlavefile(u, v,  p, h, mpiSizeH, mpiSizeV, step,rank);
		 }
		 /*std::cout << "pressure: " <<std::endl;
		 p.PlotGrid();
		 std::cout << std::endl;

		 std::cout << "u/v: " <<std::endl;
		 u.PlotGrid();
         std::cout << std::endl;
		 v.PlotGrid();
         std::cout << std::endl;
*/

	    // compute f / g
		GridFunctionType blgx = gx.GetGridFunction(); //ToDo: schoener machen!
		GridFunctionType blgy = gy.GetGridFunction();
		GridFunctionType blu  = u.GetGridFunction();
		GridFunctionType blv  = v.GetGridFunction();
		pc.computeMomentumEquations(&f,&g,&blu,&blv,blgx,blgy,h,deltaT);

		pc.setBoundaryF(f,blu);
		pc.setBoundaryG(g,blv);
		// set right side of pressure equation
		GridFunctionType blf = f.GetGridFunction();
		GridFunctionType blg = g.GetGridFunction();
		pc.computeRighthandSide(&rhs, blf, blg,h,deltaT);


		// solver
		//ToDo enventuell muss die iterationschleife hier rein!
		GridFunctionType blrhs = rhs.GetGridFunction();
		sol.SORCycle(&p,blrhs,h);
		//Update velocity
		GridFunctionType blp = p.GetGridFunction();
		pc.computeNewVelocities(&u, &v,blf,blg,blp,h,deltaT);
	//	u.PlotGrid();
		//std::cout<<std::endl<<"------------------------"<<std::endl;

		// update time
		t += deltaT;



		// write files
		std::cout<< step<<"  -  "<<t<<" / " <<simparam.tEnd<<std::endl;
	}

	GridFunction TestGridU(7,7);
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			TestGridU.SetGridFunction(i,j,i);
		}
	}

	GridFunction TestGridV(7,7);
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 7; j++) {
				TestGridV.SetGridFunction(i,j,j);
			}
		}


	GridFunction DerivGrid(7,7);
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			DerivGrid.SetGridFunction(i,j,0);
		}
	}

	Stencil stenci(3,h);
	MultiIndexType beginread(0,0);
	MultiIndexType endread(6,6);
	MultiIndexType beginwrite(1,1);
	MultiIndexType endwrite(5,5);
	stenci.ApplyUVyStencilOperator(beginread,endread,beginwrite,endwrite, TestGridU.GetGridFunction(), TestGridV.GetGridFunction(),DerivGrid, simparam.alpha);
	//DerivGrid.PlotGrid();

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


/*	GridFunction testgrid(griddimension,simparam.UI);
	for (int i = 0; i < griddimension[0]; i++) {
		for (int j=0; j < griddimension[1]; j++) {
			testgrid.SetGridFunction(i,j,i);
		}
	}

	testgrid.PlotGrid();

	GridFunction derivative(griddimension,0);
	Stencil sten(3,h);
	sten.ApplyPxStencilOperator(bb,ee,bb,ee,testgrid.GetGridFunction(),derivative);

	derivative.PlotGrid();
//bla
*/
	return 0;
}

