/*----------------------------------------------------------------------------------------------
 *  AUTHOR: Aaron Kraemer, and others..?
 *  Version: 1
 *
 *  File: solver.cpp
 *----------------------------------------------------------------------------------------------
 * This is the class file to class Solver
 *
 * the Solver contains the function to compute the current residual and a function
 * in which the residual is variated, in the way of SOR
 *
 */
#include "solver.h"
#include <math.h>

#include "../IO/IO.hpp"
#include "../Stencil/stencil.h"

RealType computeResidual(GridFunctionType& sourcegridfunction,
    				     GridFunctionType& rhs,
    					 const PointType& h){
//ToDo delete when use Solver in main:>>>>>>>>>>>>>>>>>>
    	char InputFileName[] = "../inputvals.bin";
    	char OutputFileName[] = "output.txt";
    	IO Reader(InputFileName,OutputFileName);
    	Simparam simparam = Reader.getSimparam();
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // The pre-value to be returned (return sqrt(doubleSum)):
    RealType doubleSum = 0.0;

    /* We need to compute the derivatives p_xx and p_yy, therefore the stencil has to be applied.
     *
     *
     * The following lines >> << are ugly, but have to be, because the aPPLYsTENCILoPERATOR needs
     * some special input.. It would be nice, if we could write it in a more space-saving way.
     */
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    MultiIndexType myBegin;							//
    myBegin[0]= 0;									//
    myBegin[1]= 0;									//
    const MultiIndexType begin = myBegin;			//
    MultiIndexType myEnd;							//
    myEnd[0]= simparam.iMax;						//
    myEnd[1]= simparam.jMax;						//
    const MultiIndexType end = myEnd;				//
													//
    MultiIndexType dimensions; 						// ToDo Kann man const besser initialisieren?
    dimensions[0]= simparam.iMax;					//
    dimensions[1]= simparam.jMax;					//
    const MultiIndexType gridDimensions = dimensions;//
													//
    PointType helper;								//
    helper = h; 									// ToDo Kann man non-const besser initialisieren?
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    //Compute the needed derivations for the whole (inner?) area
    Stencil stencil(3, helper); 					// bzw. Kann man einfach const weitergeben? /Wie?
    //Get the values for derivative in x-direction:
    stencil.setFxxStencil();
    GridFunction Fxx(gridDimensions);
    stencil.ApplyStencilOperator(begin, end, begin, end, sourcegridfunction, Fxx);
    //Get the values for derivative in y-direction:
    stencil.setFyyStencil();
    GridFunction Fyy(gridDimensions);
    stencil.ApplyStencilOperator(myBegin, end, myBegin, end, sourcegridfunction, Fyy);

    // Compute the residual: res = sqrt(Sum_i^I(Sum_j^J((p_xx+p_yy-rightHandSide)²/(I*J))))
    for (int i = 1; i <= simparam.iMax; i++)
    {
    	for (int j = 1; j <= simparam.jMax; j++)
    	{
    		RealType derivator = Fxx.GetGridFunction()[i][j]+ Fyy.GetGridFunction()[i][j] - rhs[i][j];

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

    	//Initialization of the residual. Just choose a value, that should be a bad error.
        RealType res;
        res = 10e20;
        int iterationCounter = 0;
        // SOR-cycling until error is small enough, or the number of iterations gets to high:
        while (iterationCounter < simparam.iterMax && res < simparam.eps )
        {
             for (int i = 1; i <= simparam.iMax; i++)
            {
        	    for (int j = 1; j <= simparam.jMax; j++)
            	{
                    //help-values "neighbours_x" and "neighbours_y" for better overview
        	    	RealType neighbours_x = (gridfunction->GetGridFunction()[i+1][j] + gridfunction->GetGridFunction()[i-1][j])
        	    			              / delta[0] / delta[0];
        	    	RealType neighbours_y = (gridfunction->GetGridFunction()[i][j+1] + gridfunction->GetGridFunction()[i][j-1])
                	    			              / delta[1] / delta[1];
        	    	//SOR-iteration
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

