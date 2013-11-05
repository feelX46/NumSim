/*
 * simparam.h
 *
 *  Created on: 05.11.2013
 *      Author: felix
 */

#ifndef SIMPARAM_H_
#define SIMPARAM_H_

#include "template.h"
struct Simparam {
	RealType xLength;
	RealType yLength;
	int iMax;
	int jMax;
	int tEnd;
	RealType tau;
	int deltaVec;
	int iterMax;
	RealType eps;
	RealType omg;
	RealType alpha;
	int RE;
	int GX;
	int GY;
	int UI;
	int UV;
	int PI;
};


#endif /* SIMPARAM_H_ */
