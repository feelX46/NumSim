/*
 * stencil.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: ischifx
 */

#include "stencil.h"
#include <iostream>


Stencil::Stencil(int stencilwidth_input, PointType& h_input) : stencilwidth(stencilwidth_input), h(h_input){
	stencil = new RealType*[stencilwidth_input];
}

void Stencil::ApplyStencilOperator(const MultiIndexType& gridreadbegin,
						const MultiIndexType& gridreadend,
						const MultiIndexType& gridwritebegin,
						const MultiIndexType& gridwriteend,
						const GridFunction sourcegridfunction,
						GridFunction imagegridfunction) {

	// iteriere ueber alle punkte (zeilen und spalten)
	for (IndexType i=gridreadbegin[0]; i <=gridreadend[0]; i++){
    	for (IndexType j=gridreadbegin[1]; j<=gridreadend[1];  j++){
    		// fuer jede Ableitung wird die 3x3 Stencil Matrix angewendet
    				for(IndexType k=0; k<3;k++){
    					for(IndexType l=0;l<3;l++){
    						imagegridfunction.SetGridFunction(i,j,
    								sourcegridfunction.GetGridFunction()[i-k-1][j-l-1]*stencil[k][l]);
    					}
    				}
    	}
    }
}

void Stencil::setFxxStencil() {
	stencil[-1][-1] = 0;
	stencil[0][-1] = 0;
	stencil[1][-1] = 0;
	stencil[-1][0] = 1/(h[0]*h[0]);
	stencil[0][0] = -2/(h[0]*h[0]);
	stencil[1][0] = 1/(h[0]*h[0]);
	stencil[-1][1] = 0;
	stencil[0][1] = 0;
	stencil[1][1] = 0;
}

void Stencil::setFyyStencil() {
	stencil[-1][-1] = 0;
	stencil[0][-1] = 1/(h[0]*h[0]);
	stencil[1][-1] = 0;
	stencil[-1][0] = 0;
	stencil[0][0] = -2/(h[0]*h[0]);
	stencil[1][0] = 0;
	stencil[-1][1] = 0;
	stencil[0][1] = 1/(h[0]*h[0]);
	stencil[1][1] = 0;
}
