/*
 * gridfunction.cpp
 *
 *  Created on: 06.11.2013
 *      Author: Markus
 */

#include "gridfunction.h"

GridFunction::GridFunction(int DimX, int DimY){
	 gridfunction= new RealType*[DimX];
	 for (int i = 0; i < DimX; i++){
		 gridfunction[i] = new RealType [DimY];
	 }
	 griddimension[1] = DimX;
	 griddimension[2] = DimY;
}

GridFunction::GridFunction(const MultiIndexType griddimension_input) : griddimension(griddimension_input){
	 gridfunction= new RealType*[griddimension[1]];
	 for (int i = 0; i < griddimension[1]; i++){
		 gridfunction[i] = new RealType [griddimension[2]];
	 }
}

GridFunction::~GridFunction(){
	for(IndexType delCol = 0; delCol < griddimension[1]; delCol++)
		{
			delete [] gridfunction[griddimension[2]];
		}
		delete [] gridfunction;
	}


GridFunctionType GridFunction::GetGridFunction() const{
	return gridfunction;
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


void GridFunction::SetGridFunction (const IndexType& i, const IndexType& j,
		RealType value){
		gridfunction[i][j] = value;
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
