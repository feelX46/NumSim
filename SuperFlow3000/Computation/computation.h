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
};



#endif /* COMPUTATION_H_ */
