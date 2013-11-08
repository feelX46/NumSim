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
	/*! Function to compute the global residual
		   * @param sourcegridfunction ?The discretized solution
		   * @param rhs The right hand side of the discretized local PDE
		   * @param h ?what are these two RealTypes for?
		   */
    RealType computeResidual(GridFunctionType& sourcegridfunction,
    						 GridFunctionType& rhs,
    						 const PointType& h);
    //-------------------------------------------------------------------------------

    /*! Function to compute the global residual
    		   * @param gridfunction Pointer on the discretized solution
    		   * @param rhs The right hand side of the discretized local PDE
    		   * @param delta \delta_i are the gridwidths in x- and y-direction
    		   * @param omega The \omega -parameter of the SOR-cycle
    		   */
    void SORCycle(Gridfunction* gridfunction,
    	    	  GridFunctionType& rhs,
    	    	  const PointType& delta,
    	    	  RealType omega);
    //-------------------------------------------------------------------------------

    /*! Constructor
    * (does not have to do anything)
    */
    Solver::Solver(void);
    //-------------------------------------------------------------------------------

    /*! Destructor
     */
    Solver::~Solver();

private:

};

IO::IO Reader;

#endif /* SOLVER_H_ */
