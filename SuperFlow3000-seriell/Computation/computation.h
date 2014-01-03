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
#include "../IO/IO.hpp"

class Computation {
public:

	Computation(Simparam param);

	RealType computeTimestep(RealType uMax, RealType vMax, const PointType& h);

	void computeNewVelocities(GridFunction* u, GridFunction* v,
								GridFunction& f, GridFunction& g,
								GridFunction& p, const PointType& h,
								RealType deltaT);

	void computeMomentumEquations(GridFunction* f, GridFunction* g,
								GridFunction* u, GridFunction* v,
								GridFunction* T,
								GridFunction& gx, GridFunction& gy,
							    const PointType& h, RealType& deltaT);

	/*! @brief Function to set the boundary values for u, the velocities in x-direction.
	 * First implementation: only no-flow boundaries.
	 *  @param velocity_x is a GridFunction-Object, containing all discretization points.
	 */
	void setBoundaryU(GridFunction& velocity_x);


	/*! @brief Function to set the boundary values for v, the velocities in y-direction.
	 * First implementation: only no-flow boundaries.
	 *  @param velocity_y is a GridFunction-Object, containing all discretization points.
	 */
	void setBoundaryV(GridFunction& velocity_y);

	void setBoundaryV(GridFunction& velocity_y, int rank);

	/*! @brief Function to set the boundary values for the pressure p.
	 *  @param pressure is a GridFunction-Object, containing all discretization points.
	 */
	void setBoundaryP(GridFunction& pressure);

	/*! @brief Function to set the boundary values for F (12).
	 *  @param f is a GridFunction-Object, containing F for all discretization points.
	 */
	void setBoundaryF(GridFunction& f, GridFunction& u);

	/*! @brief Function to set the boundary values for G (13).
	 *  @param g is a GridFunction-Object, containing G for all discretization points.
	 */
	void setBoundaryG(GridFunction& g, GridFunction& v);

	void setBoundaryTD(GridFunction& T);

	/*! @brief Function to compute the righthand like (14).
		 *  @param rhs is a pointer on the GridFunction-object, that is to be computed.
		 *  @param f is the reference to a GridFunctionType, containing F.
		 *  @param g is the reference to a GridFunctionType, containing G.
		 *  @param delta contains the gridwidths in x- and y-direction.
		 *  @param deltaT contains the time step size for the next iteration step.
		 */
    void computeRighthandSide(GridFunction* rhs,
    		GridFunction& f,
    		GridFunction& g,
    		const PointType& delta,
    		RealType deltaT);

    void computeTemperature(GridFunction& T,
    						GridFunction& u,
    						GridFunction& v,
    						GridFunction& q,
    						const PointType& h,
    						RealType& deltaT);

    Simparam param;
};


#endif /* COMPUTATION_H_ */
