/*
 * gridfunction.cpp
 *
 *  Created on: 06.11.2013
 *      Author: Markus
 */

#include "gridfunction.h"
#include <iostream>
//1
GridFunction::GridFunction(int DimX, int DimY){
	 gridfunction= new RealType*[DimX];
	 for (int i = 0; i < DimX; i++){
		 gridfunction[i] = new RealType [DimY];
	 }
	 griddimension[1] = DimX;
	 griddimension[2] = DimY;
}

//2
GridFunction::GridFunction(const MultiIndexType griddimension_input) : griddimension(griddimension_input){
	 gridfunction= new RealType*[griddimension[1]];
	 for (int i = 0; i < griddimension[1]; i++){
		 gridfunction[i] = new RealType [griddimension[2]];
	 }
}

//3
GridFunction::~GridFunction(){
	for(IndexType delCol = 0; delCol < griddimension[1]; delCol++)
		{
			delete [] gridfunction[griddimension[2]];
		}
		delete [] gridfunction;
	}

//4
GridFunctionType GridFunction::GetGridFunction() const{
	return gridfunction;
}
//new
RealType GridFunction::GetGridFunction(const MultiIndexType& index){
	return gridfunction[index[1]][index[2]];
}

//new
MultiIndexType GridFunction::GetGridDimension(){
	return griddimension;
}

//5
void GridFunction::SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType value){
	for (IndexType i = begin[2];i<end[2]; i++){
		for (IndexType j = begin[1]; j<end[1]; j++){
			gridfunction[i][j] = value;
		}
	}
}

//new
void GridFunction::SetGridFunction (const IndexType& i, const IndexType& j,
		RealType value){
		gridfunction[i][j] = value;
}

//6
void GridFunction::SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,RealType factor,
			MultiIndexType& offset){
	for (IndexType i = begin[2];i<end[2]; i++){
		for (IndexType j = begin[1]; j<end[1]; j++){
			gridfunction[i][j] = factor * gridfunction[i+offset[0]][j+offset[1]];
		}
	}
}

//8
void GridFunction::SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType factor, GridFunctionType& sourcegridfunction){
	for (IndexType i = begin[2];i<end[2]; i++){
		for (IndexType j = begin[1]; j<end[1]; j++){
			gridfunction[i][j] = factor * sourcegridfunction[i][j];
		}
	}
}

//9
void GridFunction::SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType factor, GridFunctionType& sourcegridfunction, MultiIndexType& offset){
	for (IndexType i = begin[2];i<end[2]; i++){
		for (IndexType j = begin[1]; j<end[1]; j++){
			gridfunction[i][j] = factor * sourcegridfunction[i+offset[0]][j+offset[1]];
		}
	}
}

//~10
void GridFunction::SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType factor,GridFunctionType& sourcegridfunction, RealType constant){
	for (IndexType i = begin[2];i<end[2]; i++){
		for (IndexType j = begin[1]; j<end[1]; j++){
			gridfunction[i][j] = constant + factor * sourcegridfunction[i][j];
		}
	}
}

//~10
void GridFunction::SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType factor,GridFunctionType& sourcegridfunction, MultiIndexType& offset,
		RealType constant){
	for (IndexType i = begin[2];i<end[2]; i++){
		for (IndexType j = begin[1]; j<end[1]; j++){
			gridfunction[i][j] = constant + factor * sourcegridfunction[i+offset[0]][j+offset[1]];
		}
	}
}

//7
void GridFunction::ScaleGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType factor){
	for (IndexType i = begin[2];i<end[2]; i++){
		for (IndexType j = begin[1]; j<end[1]; j++){
			gridfunction[i][j] = factor * gridfunction[i][j];
		}
	}

}

//11
void GridFunction::AddToGridFunction (const MultiIndexType& begin, const MultiIndexType& end,
		RealType factor, GridFunctionType& sourcegridfunction){
	for (IndexType i = begin[2];i<end[2]; i++){
		for (IndexType j = begin[1]; j<end[1]; j++){
			gridfunction[i][j] = gridfunction[i][j] + factor * sourcegridfunction[i][j];
		}
	}
}

//12
RealType GridFunction::MaxValueGridFunction (const MultiIndexType& begin,
		const MultiIndexType& end){
	IndexType max = 0;
	for (IndexType i = begin[2];i<end[2]; i++){
		for (IndexType j = begin[1]; j<end[1]; j++){
			if (gridfunction[i][j] > max)
				max = gridfunction[i][j];
		}
	}

	return max;
}
