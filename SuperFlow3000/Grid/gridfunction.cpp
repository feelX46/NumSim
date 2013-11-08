/*
 * gridfunction.cpp
 *
 *  Created on: 06.11.2013
 *      Author: Markus
 */

#include "gridfunction.h"

GridFunction::GridFunction(int DimX, int DimY){
	 RealType *grid_data[DimY];
	 for (int i=0; i < DimY; i++){
		 RealType grid_datax[DimX];
		 grid_data[i]=grid_datax;
	 }
	 gridfunction = grid_data;


	  /*int **matrix ; // Adresse eines Pointers
	    int i,k;

	    matrix=(int**)malloc(z*sizeof(void*));
	    for (i=0;i<z;i++) {
	        matrix[i]=(int*)malloc(s*sizeof(int));
	    }
	    */
}

GridFunction::GridFunction(const MultiIndexType griddimension){
	/*
	 RealType *gridfunction = new (RealType*)[griddimension[1]];
	 for (int i = 0; i < griddimension[1]; i++){
		 gridfunction[i] = new (RealType)[griddime'nsion[2]];
	 }
*/
/*
	 int*** array2d = new (int**)[rows];
	 for (int i = 0; i < rows; ++i) {
	   array2d[i] = new (int*)[cols];
	 }
	 */
}

GridFunction::~GridFunction(){
}


GridFunctionType GridFunction::GetGridFunction() const{
	return 0;
}

RealType GridFunction::GetGridFunction(const MultiIndexType& index){
	return gridfunction[index[1]][index[2]];
}


MultiIndexType GridFunction::GetGridDimension(){
	return griddimension;
}


void GridFunction::SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType value){
	for (int i = begin[2]; i++; i<end[2]){
		for (int j = begin[1]; j++; j<end[1]){
			gridfunction[i][j] = value;
		}
	}
}

void GridFunction::SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType factor, GridFunctionType& sourcegridfunction){
}


void GridFunction::SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType factor, GridFunctionType& sourcegridfunction, MultiIndexType& offset){

}


void GridFunction::SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType factor,GridFunctionType& sourcegridfunction, MultiIndexType& offset,
		RealType constant){
}


void GridFunction::ScaleGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType factor){
}


void GridFunction::AddToGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType factor, GridFunctionType& imagegridfunction){

}


RealType GridFunction::MaxValueGridFunction (const MultiIndexType& begin,
		const MultiIndexType& end){
	return 0;
}
