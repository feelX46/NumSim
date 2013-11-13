/*
 * computation.h
 *
 *  Created on: 08.11.2013
 *      Author: felix
 */

#ifndef COMPUTATION_H_
#define COMPUTATION_H_

#include"../Misc/typedef.h"
#include"../Grid/gridfunction.h"

class Computation {
public:
	RealType computeTimestep(RealType uMax, RealType vMax, const PointType&h,
								RealType Re, RealType tau);

	void computeNewVelocities(GridFunction* u, GridFunction* v,
								GridFunctionType& f, GridFunctionType& g,
								GridFunctionType& p, const PointType& delta,
								RealType deltaT);

	void computeMomentumEquations(GridFunction* f, GridFunctionType* g,
								GridFunctionType* u, GridFunctionType* v,
								GridFunctionType& gx, GridFunctionType& gy,
								const PointType& h, RealType deltaT);

	/*! @brief Function to set the boundary values for u, the velocities in x-direction.
	 * First implementation: only no-flow boundaries.
	 *  @param velocity_x is a GridFunction-Object, containing all discretization points.
	 */
	void setBoundaryU(GridFunction velocity_x);

	/*! @brief Function to set the boundary values for v, the velocities in y-direction.
	 * First implementation: only no-flow boundaries.
	 *  @param velocity_y is a GridFunction-Object, containing all discretization points.
	 */
	void setBoundaryV(GridFunction velocity_y);

	/*! @brief Function to set the boundary values for the pressure p.
	 *  @param pressure is a GridFunction-Object, containing all discretization points.
	 */
	void setBoundaryP(GridFunction pressure);

	/*! @brief Function to set the boundary values for F (12).
	 *  @param f is a GridFunction-Object, containing F for all discretization points.
	 */
	void setBoundaryF(GridFunction f);

	/*! @brief Function to set the boundary values for G.
	 *  @param g is a GridFunction-Object, containing G for all discretization points.
	 */
	void setBoundaryG(GridFunction g);


};


#endif /* COMPUTATION_H_ */
