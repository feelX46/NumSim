/*
 * gridfunction.cpp
 *
 *  Created on: 06.11.2013
 *      Author: Markus
 */

#include "Gridfunction.h"

//bliblbablub
Gridfunction::Gridfunction(int DimX, int DimY){
	 RealType *grid_data[DimY];
	 for (int i=0; i < DimY; i++){
		 RealType grid_datax[DimX];
		 grid_data[i]=grid_datax;
	 }
	 gridfunction = grid_data;
}

Gridfunction::Gridfunction(const MultiIndexType griddimension){
	Gridfunction(griddimension[1],griddimension[2]);
	gridfunction = 0;
}

Gridfunction::~Gridfunction(){

}
