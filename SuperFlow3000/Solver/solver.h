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
/*! @class Class Solver runs the Successive-Over-Relaxation-Cycle
 *
 */
class Solver {
public:
	/*! @brief Function to compute the global residual
		   * @param sourcegridfunction ?The discretized solution
		   * @param rhs The right hand side of the discretized local PDE
		   * @param h ?what are these two RealTypes for?
		   */
    RealType computeResidual(GridFunction& sourcegridfunction,
    						 GridFunctionType& rhs,
    						 PointType& h);
    //-------------------------------------------------------------------------------

    /*! @brief Function to compute the global residual
    		   * @param gridfunction Pointer on the discretized solution
    		   * @param rhs The right hand side of the discretized local PDE
    		   * @param delta delta_i are the gridwidths in x- and y-direction
    		   * @param omega The relaxation-parameter of the SOR-cycle
    		   */
    void SORCycle(GridFunction* gridfunction,
    	    	  GridFunctionType& rhs,
    	    	  PointType& delta,
    	    	  const RealType omega,
    	    	  const RealType eps,
    	    	  const IndexType iterMax);
    //-------------------------------------------------------------------------------

    /*! @brief Constructor to made?
    * (does not have to do anything)
    */
    //Solver::Solver();
    //-------------------------------------------------------------------------------

    /*! @brief Destructor to be made?
     */
   // Solver::~Solver();

private:

};


#endif /* SOLVER_H_ */
