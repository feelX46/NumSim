/*
 * stencil.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: ischifx
 */

#include "stencil.h"
#include <iostream>


Stencil::Stencil(int stencilwidth_input, PointType& h_input) : stencilwidth(stencilwidth_input), h(h_input){
	 stencil = new RealType*[stencilwidth];
	 for (int i = 0; i < stencilwidth; i++){
		 stencil[i] = new RealType [stencilwidth];
	 }
}


void Stencil::ApplyStencilOperator(const MultiIndexType& gridreadbegin,
		const MultiIndexType& gridreadend,
		const MultiIndexType& gridwritebegin,
		const MultiIndexType& gridwriteend,
		const GridFunctionType sourcegridfunction,
		GridFunction imagegridfunction){

	for (IndexType i = gridwritebegin[0]; i < gridwriteend[0]+1;i++){
		imagegridfunction.SetGridFunction(0,i,0.0);
		imagegridfunction.SetGridFunction(gridreadend[0],i,0.0);
	}
	for (IndexType j = gridwritebegin[1]; j < gridwriteend[1]+1; j++) {
		imagegridfunction.SetGridFunction(j,0,0.0);
		imagegridfunction.SetGridFunction(j,gridreadend[0],0.0);
	}

	// Berechne die Ableitungen
	// (0,0) ist bei allen allen drei Matrtzen (Stencil, sourcegrid, imagegrid) oben links
	RealType tmp;
	for (IndexType i=gridwritebegin[0]+1; i<gridwriteend[0]; i++){
		for (IndexType j=gridwritebegin[1]+1; j<gridwriteend[1]; j++){
			tmp = 0.0;
			for(IndexType k=0; k<3; k++){
				for(IndexType l=0; l<3; l++){
					tmp = tmp + sourcegridfunction[i+k-1][j+l-1]*stencil[l][k];
				}
			}
			imagegridfunction.SetGridFunction(i,j,tmp);
	 	}
	}

	// Da die Groesse der Ableitungsmatrix so gross ist, wie die eigentliche Matrix,
	// werden die Ableitungsraender auf 0 gesetzt



}

void Stencil::setFxxStencil() {
	stencil[0][0] = 0;
	stencil[1][0] = 0;
	stencil[2][0] = 0;
	stencil[0][1] = 1/(h[0]*h[0]);
	stencil[1][1] = -2/(h[0]*h[0]);
	stencil[2][1] = 1/(h[0]*h[0]);
	stencil[0][2] = 0;
	stencil[1][2] = 0;
	stencil[2][2] = 0;
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
