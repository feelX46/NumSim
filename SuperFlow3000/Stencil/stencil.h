/*
 * stencil.h
 *
 *  Created on: Nov 6, 2013
 *      Author: ischifx
 */

#ifndef STENCIL_H_
#define STENCIL_H_


#include"../Misc/typedef.h"

class Stencil {
public:

	Stencil(int stencilwidth_input, const PointType& h_input);
	//void ~Stencil();

	//StencilType stencil;
	int stencilwidth;
	//const PointType& h;
};



#endif /* STENCIL_H_ */
