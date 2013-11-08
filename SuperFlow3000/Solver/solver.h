/*
 * solver.h
 *
 *  Created on: 08.11.2013
 *      Author: Aaron
 */

#ifndef SOLVER_H_
#define SOLVER_H_


#include"../Misc/typedef.h"
#include"../Grid/gridfunction.h"

class Solver {
public:
    RealType computeResidual(GridFunctionType& sourcegridfunction,
    						 GridFunctionType& rhs,
    						 const PointType& h);

    void SORCycle(GridFunction* gridfunction,
    	    	  GridFunctionType& rhs,
    	    	  const PointType& delta,
    	    	  RealType omega);

    Solver::Solver(void);

    Solver::~Solver();

private:

};

IO::IO Reader;

#endif /* SOLVER_H_ */
