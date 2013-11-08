/*
 * stencil.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: ischifx
 */

#include "stencil.h"
#include <iostream>
#include "../Grid/gridfunction.h"

Stencil::Stencil(int stencilwidth_input, PointType& h_input) : stencilwidth(stencilwidth_input), h(h_input){
	stencil = new RealType*[stencilwidth_input];
}

void Stencil::ApplyStencilOperator(const MultiIndexType& gridreadbegin,
						const MultiIndexType& gridreadend,
						const MultiIndexType& gridwritebegin,
						const MultiIndexType& gridwriteend,
						const GridFunction sourcegridfunction,
						GridFunction imagegridfunction) {

	for (int i=gridreadbegin[0]; i++; i <=gridreadend[0]){
    	for (int j=gridreadbegin[1]; j++; j<=gridreadend[1]){
    		imagegridfunction.SetGridFunction(gridwritebegin,gridwriteend,sourcegridfunction.GetGridFunction()[i][j]*stencil[gridreadend[0]-i][gridreadend[1]-j]);
    	}
    }
}

void Stencil::setFxxStencil() {
	stencil[-1][-1] = 0;
	stencil[0][-1] = 0;
	stencil[1][-1] = 0;
	stencil[-1][0] = 1/(h[0]*h[0]);
	stencil[0][0] = 2/(h[0]*h[0]);
	stencil[1][0] = 1/(h[0]*h[0]);
	stencil[-1][1] = 0;
	stencil[0][1] = 0;
	stencil[1][1] = 0;

}
