/*
 * stencil.h
 *
 *  Created on: Nov 6, 2013
 *      Author: ischifx
 */

#ifndef STENCIL_H_
#define STENCIL_H_


#include"../Misc/typedef.h"
#include"../Grid/Gridfunction.h"

class Stencil {
public:

	Stencil(int stencilwidth_input, PointType& h_input);
	//void ~Stencil();

	StencilType stencil;
	int stencilwidth;
	PointType& h;

	void ApplyStencilOperator(const MultiIndexType& gridreadbegin,
			const MultiIndexType& gridreadend,
			const MultiIndexType& gridwritebegin,
			const MultiIndexType& gridwriteend,
			const GridFunction sourcegridfunction,
			GridFunction imagegridfunction);

	void setFxxStencil();
	void setFyyStencil();
};

#endif /* STENCIL_H_ */
