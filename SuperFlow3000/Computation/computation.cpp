/*
 * computation.cpp

 *
 *  Created on: 08.11.2013
 *      Author: felix
 */
#include<math.h>
#include"computation.h"
#include<iostream>
#include"../Grid/gridfunction.h"

RealType computeTimestep(RealType uMax, RealType vMax, const PointType&h,
		RealType Re, RealType tau){

	RealType minimum = tau*Re/2/(1/h[0]/h[0]+1/h[1]/h[1]);
	if (minimum > h[0]/abs(uMax)) {minimum = h[0]/abs(uMax);}
	if (minimum > h[1]/abs(vMax)) {minimum = h[1]/abs(vMax);}
	return minimum;

}

void computeNewVelocities(GridFunction* u, GridFunction* v,
								GridFunctionType& f, GridFunctionType& g,
								GridFunctionType& p, const PointType& delta,
								RealType deltaT){

	std::cout << "Hier muss unbedingt noch iMax, jMax angepasst werden!!!";

	GridFunctionType temp[128][128];
	for (int i=1; i++; i<=128){
		for (IndexType j=1; j++; j<=128) {
			p[i][j] = 1;
		}
	}
	MultiIndexType begin;
	begin[0]=1;
	begin[1]=1;
	MultiIndexType end;
	end[0]=128;
	end[1]=128;
	//u->SetGridFunction(&begin,&end, 1.0, f-deltaT/delta[0]*temp); TODO


}


void computeMomentumEquations(GridFunction* f, GridFunctionType* g,
								GridFunctionType* u, GridFunctionType* v,
								GridFunctionType& gx, GridFunctionType& gy,
								const PointType& h, RealType deltaT) {
	/*int Rey = 1000;
	std::cout << "Hier muss noch die Reynoldszahl aus simparam genommen werden!!!!";

	f = u + deltaT(1/Rey(...+...)-...-...+gx);

	ApplyStencilOperator(const MultiIndexType& gridreadbegin,
				const MultiIndexType& gridreadend,
				const MultiIndexType& gridwritebegin,
				const MultiIndexType& gridwriteend,
				const GridFunction sourcegridfunction,
				GridFunction imagegridfunction);*/
}
