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
//#include "mpi.h"
//#include "Communication/communication.h"

int main(int argc, char *argv[]){
	std::cout << "#### SuperFlow3000 ####\n";

	char InputFileName[] = "inputvals.bin";
	char OutputFolderName[] = "output";  // output folder! -> be careful, if folder is not there, no data are saved
	char GeometryInputFileName[] = "stufe_geometrie.csv";

	//IO GeometryReader();
	//char *adress = &GeometryInputFileName;
	//GeometryReader.readCSVfile(adress);
	// load simparam

	Simparam simparam;
	IO Reader(InputFileName,OutputFolderName);
	simparam = Reader.getSimparam();

	/*
	MPI_Init(&argc, &argv);
	int mpiRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
	int mpiSize;
	MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

	// willkuerliche gewaehlt - vertikal immer nur zwei gebiete
	IndexType mpiSizeV = 2;
	IndexType mpiSizeH = 2;
	*/

	IndexType mpiSizeH = 1;
	IndexType mpiSizeV = 1;
	int mpiRank = 0;
	Computation pc (simparam);
	Solver sol (simparam);

	//Grids sollen gleiche groesse haben!!
	IndexType il=2;
	IndexType ir=il+(simparam.iMax)/mpiSizeH-1;
	IndexType jb=2;
	IndexType jt=jb+(simparam.jMax)/mpiSizeV-1;

	MultiIndexType griddimension ((ir-il+4),(jt-jb+4));

	//ToDo: glaube hier wird doppelt die InitializeGlobalBoundaryPosition aufgerufen... notwendig???
	GridFunction p(griddimension,simparam.PI,'p');
    p.InitializeGlobalBoundaryPosition(mpiRank,mpiSizeH,mpiSizeV,'p');

    GridFunction rhs(griddimension,'r');
    rhs.InitializeGlobalBoundaryPosition(mpiRank,mpiSizeH,mpiSizeV,'r');

    GridFunction v(griddimension,simparam.VI,'v');
    v.InitializeGlobalBoundaryPosition(mpiRank,mpiSizeH,mpiSizeV,'v');

    GridFunction g(griddimension,'g');
    g.InitializeGlobalBoundaryPosition(mpiRank,mpiSizeH,mpiSizeV,'g');

    GridFunction u(griddimension,simparam.UI,'u');
    u.InitializeGlobalBoundaryPosition(mpiRank,mpiSizeH,mpiSizeV,'u');

    GridFunction f(griddimension,'f');
    f.InitializeGlobalBoundaryPosition(mpiRank,mpiSizeH,mpiSizeV,'f');

    GridFunction T(griddimension,simparam.TI,'T');
    T.InitializeGlobalBoundaryPosition(mpiRank,mpiSizeH,mpiSizeV,'T');

    // Gridfunction q fuer Waermequellen
    GridFunction q(griddimension,0,'q');
    q.InitializeGlobalBoundaryPosition(mpiRank,mpiSizeH,mpiSizeV,'q');

    // geometry:
    //ToDo: fuer MPI so nicht richtig!!!
	//GridFunction geo(griddimension,0,'s');// = Reader.readCSVfile(GeometryInputFileName,griddimension);
	GridFunction geo = Reader.readCSVfile(GeometryInputFileName,griddimension);
     const int aof = Reader.getAmountOfFluidcells(geo); // ToDo: eleganter l�sen? (bspw. nicht �ber IO/Reader)

     std::cout<<aof<<std::endl;
     //int aof=3;
	const PointType h(simparam.xLength/simparam.iMax , simparam.yLength/simparam.jMax);
	RealType deltaT = simparam.deltaT;
	RealType local_deltaT = simparam.deltaT;
	RealType t = 0;
	int step = 0;

	int ibegin = p.beginwrite[0];
	int iend   = p.endwrite[0];
	int jbegin = p.beginwrite[1];
	int jend   = p.endwrite[1];
	int localgriddimensionX = iend-ibegin+1;
	int localgriddimensionY = jend-jbegin+1;

	// so gross wie u
    geo.PlotGrid();
	GridFunction gx(griddimension,simparam.GX,'u');
    //geo.PlotGrid();
    //exit(0);
	// so gross wie v
	GridFunction gy(griddimension,simparam.GY,'v');

	//---- for Boundary condition ----
	//for driven cavity
	MultiIndexType offset (0,-1);
	//evtl. zum testen noetig (einfach durchfliessen)
	MultiIndexType linksunten (0,1);
	MultiIndexType rechtsoben  (griddimension[0]-2,griddimension[1]-2);
	// write first output data
	// Parallele Visualisierung

	std::cout<<"Schritt 1"<<std::endl;
	if (mpiRank == 0) {
		Reader.writeVTKMasterfile(mpiSizeH, mpiSizeV, step, localgriddimensionX, localgriddimensionY);
	}

	Reader.writeVTKSlavefile(u, v,  p, T, geo, h, mpiSizeH, mpiSizeV, step,mpiRank);
	// start time loop
//	Communication communicator(mpiRank, mpiSizeH, mpiSizeV, p.globalboundary); //(MPI)
	while (t <= simparam.tEnd){
		// set boundary
		pc.setBoundaryU(u); //First implementation: only no-flow boundaries-> everything is zero!
		pc.setBoundaryV(v);
		pc.setBoundaryTD(T);
		pc.setBoundaryTN(T,h);
		pc.setBarrierBoundaryU(u, geo);
		pc.setBarrierBoundaryV(v, geo);
		// driven cavity:
		/*if (u.globalboundary[2]){
			//MultiIndexType UpperLeft(1,u.griddimension[1]-1);
			MultiIndexType UpperLeft(0,u.griddimension[1]-1);
			MultiIndexType UpperRight(u.griddimension[0]-2, u.griddimension[1]-1);
			u.SetGridFunction(UpperLeft,UpperRight,-1.0,offset,2.0);
		}*/

		// compute deltaT
		//deltaT = 0.005;
		local_deltaT = pc.computeTimestep(u.MaxValueGridFunction(u.beginread,u.endread),v.MaxValueGridFunction(v.beginread,v.endread),h);
		//MPI_Allreduce ( &local_deltaT, &deltaT, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
		deltaT = local_deltaT;

		if (0 == (step % 20)) {  // parallel visualisierung
			 if (mpiRank == 0) {
				 Reader.writeVTKMasterfile(mpiSizeH, mpiSizeV, step, localgriddimensionX, localgriddimensionY);
			 }
			 Reader.writeVTKSlavefile(u, v,  p, T, geo, h, mpiSizeH, mpiSizeV, step,mpiRank);
		}

		// Berechne neues T um damit die Momentum-Equations zu berechnen
		pc.computeTemperature(T, u, v, q, h, deltaT);
	    // compute f / g
		pc.computeMomentumEquations(&f,&g,&u,&v,&T,gx,gy,h,deltaT);
		pc.setBoundaryF(f,u);
		pc.setBoundaryG(g,v);
		pc.setBarrierBoundaryF(f, u, geo);
		pc.setBarrierBoundaryG(g, v, geo);
		std::cout<<std::endl;
        geo.PlotGrid();
		// set right side of pressure equation
		pc.computeRighthandSide(&rhs, f, g,h,deltaT);

		// solver
		sol.SORCycle(&p,rhs,h, aof, geo);// ,&communicator); (MPI)

		//Update velocity
		pc.computeNewVelocities(&u, &v,f,g,p,h,deltaT, geo);

		//MPI-Functions:
		//communicator.ExchangePValues(u);
		//communicator.ExchangePValues(v);

		// update time
		t += deltaT;
		step++;

		// write files
		std::cout<< step<<"  -  "<<t<<" / " <<simparam.tEnd<<std::endl;

	}
	//MPI_Finalize();
	std::cout<<"fedisch";
	return 0;
}

