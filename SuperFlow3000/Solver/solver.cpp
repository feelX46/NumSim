/*----------------------------------------------------------------------------------------------
 *  AUTHOR: Aaron Kr�mer, and others..?
 *  Version: 1
 *
 *  File: solver.cpp
 *----------------------------------------------------------------------------------------------
 * This is the class file to class Solver
 *
 * the Solver contains the function to compute the current residual and a function
 * in which the residual is variated, in way of SOR-cycling
 *
 */
#include "solver.h"
#include <math.h>

#include "../IO/IO.hpp"
#include "../Stencil/stencil.h"

RealType computeResidual(GridFunctionType& sourcegridfunction,
    				     GridFunctionType& rhs,
    					 const PointType& h){
//muss weg____________________________________________
    	char InputFileName[] = "../inputvals.bin";
    	char OutputFileName[] = "output.txt";
    	IO Reader(InputFileName,OutputFileName);
    	Simparam simparam = Reader.getSimparam();
//----------------------------------------------------
    RealType doubleSum = 0.0;

    MultiIndexType myBegin;
    myBegin[0]= 0;
    myBegin[1]= 0;
    const MultiIndexType begin = myBegin;
    MultiIndexType myEnd;
    myEnd[0]= simparam.iMax;
    myEnd[1]= simparam.jMax;
    const MultiIndexType end = myEnd;

    MultiIndexType dimensions; 				//ToDo Kann man const besser initialisieren?
    dimensions[0]= simparam.iMax;
    dimensions[1]= simparam.jMax;
    const MultiIndexType gridDimensions = dimensions;

    GridFunction Fxx(gridDimensions);
    GridFunction Fyy(gridDimensions);

    Stencil stencil(3, h); //?!!
    stencil.setFxxStencil();
    stencil.ApplyStencilOperator(begin, end, begin, end, sourcegridfunction, Fxx);
    stencil.setFyyStencil();
    stencil.ApplyStencilOperator(myBegin, end, myBegin, end, sourcegridfunction, Fyy);


    for (int i = 1; i <= simparam.iMax; i++)
    {
    	for (int j = 1; j <= simparam.jMax; j++)
    	{
    		RealType derivator = 0.0;

    		derivator = Fxx(i,j)+ Fyy(i,j) - rhs(i,j);//[i*simparam.jMax+j];
            doubleSum +=  derivator*derivator / simparam.iMax /simparam.jMax;
    	}
    }
    return sqrt(doubleSum);
}

    void SORCycle(GridFunction* gridfunction,
    	    	  GridFunctionType& rhs,
    	    	  const PointType& delta,
    	    	  RealType omega){
//muss weg____________________________________________
    	char InputFileName[] = "../inputvals.bin";
    	char OutputFileName[] = "output.txt";
    	IO Reader(InputFileName,OutputFileName);
    	Simparam simparam = Reader.getSimparam();
//----------------------------------------------------
    	//Allocate the GridFunction for the next step:
        GridFunctionType p_next;

        //Allocate the variables, needed to copy p_next to the actual gridfunction:
    	MultiIndexType myBegin;
    	myBegin[0]= 0;
    	myBegin[1]= 0;
    	MultiIndexType myEnd;
    	myEnd[0]= simparam.iMax;
    	myEnd[1]= simparam.jMax;

    	//Initialization of the residual. Just choose a value, that would be a bad failure.
        RealType res;
        res = 1000.0;
        int iterationCounter = 0;
        while (iterationCounter < simparam.iterMax && res < simparam.eps )
        {
             for (int i = 1; i <= simparam.iMax; i++)
            {
        	    for (int j = 1; j <= simparam.jMax; j++)
            	{
                    //SOR-cycling
        	    	RealType neighbours_x = (gridfunction->GetGridFunction()[i+1][j] + gridfunction->GetGridFunction()[i-1][j])
        	    			              / delta[0] / delta[0];
        	    	RealType neighbours_y = (gridfunction->GetGridFunction()[i][j+1] + gridfunction->GetGridFunction()[i][j-1])
                	    			              / delta[1] / delta[1];
                    p_next[i][j] = (1.0 - omega)*gridfunction->GetGridFunction()[i][j]
                                 + omega /(2.0*(1/delta[0]/delta[0]+1/delta[1]/delta[1]))
                                 * (neighbours_x + neighbours_y - rhs[i][j]);
        	    }
            }
    	    iterationCounter++;
    	    gridfunction->SetGridFunction(myBegin, myEnd, 1.0, p_next);
    	    res = computeResidual(p_next, rhs, delta);
        }

    }

