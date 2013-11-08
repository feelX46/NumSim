/*
 * gridfunction.cpp
 *
 *  Created on: 06.11.2013
 *      Author: Markus
 */

#include "Gridfunction.h"

Gridfunction::Gridfunction(int DimX, int DimY){
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

Gridfunction::Gridfunction(const MultiIndexType griddimension){
	Gridfunction(griddimension[1],griddimension[2]);
	gridfunction = 0;
}

Gridfunction::~Gridfunction(){

}

//! get grid data
GridFunctionType Gridfunction::getGridfunction(){
	return 0;
}

//! get grid data
RealType Gridfunction::getGridfunction(const MultiIndexType& index){
	return gridfunction[index[1]][index[2]];
}


//! get dimension
MultiIndexType Gridfunction::getGriddimension(){
	return griddimension;
}

//! set grid data by
void Gridfunction::SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,RealType value){
}

//!
void Gridfunction::SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType factor, GridFunctionType& sourcegridfunction){
}

//!
void Gridfunction::SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType factor, GridFunctionType& sourcegridfunction, MultiIndexType& offset){
}

//!
void Gridfunction::SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType factor,GridFunctionType& sourcegridfunction, MultiIndexType& offset,
		RealType constant){
}

//! scale grid
void Gridfunction::ScaleGridFunction (const MultiIndexType& begin, const MultiIndexType& end, RealType factor){
}

//!
void Gridfunction::AddToGridFunction (const MultiIndexType& begin, const MultiIndexType& end,RealType factor,
		GridFunctionType& imagegridfunction){

}

//!
RealType Gridfunction::MaxValueGridFunction (const MultiIndexType& begin, const MultiIndexType& end){
	return 0;
}
