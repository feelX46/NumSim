/*
 * Gridfunction.h
 *
 *  Created on: 06.11.2013
 *      Author: Markus
 */

#ifndef GRIDFUNCTION_H_
#define GRIDFUNCTION_H_

#include"../Misc/typedef.h"

class Gridfunction {

public:
	/*! Construktor
	   * @param DimX Dimension in X-Direction
	   * @param DimY Dimension in Y-Direction
	   */
	Gridfunction(int DimX, int DimY);

	Gridfunction(const MultiIndexType griddimension);

	//! Destructor
	~Gridfunction();

private:
	//! grid Data on grid
	GridFunctionType gridfunction;
	//! some explanation
	MultiIndexType	griddimension;
};



#endif /* GRIDFUNCTION_H_ */
