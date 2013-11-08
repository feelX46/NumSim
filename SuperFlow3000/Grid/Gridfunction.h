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

	//! get grid data (pointer to gridfunction)
	GridFunctionType getGridfunction();

	//! get grid data
	RealType getGridfunction(const MultiIndexType& index);

	//! get dimension
	MultiIndexType getGriddimension();

	//! set grid data by
	void SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,RealType value);

	//!
	void SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,RealType factor,
			GridFunctionType& sourcegridfunction);

	//!
	void SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end, RealType factor,
			GridFunctionType& sourcegridfunction, MultiIndexType& offset);

	//!
	void SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,RealType factor,
			GridFunctionType& sourcegridfunction, MultiIndexType& offset, RealType constant);

	//! scale grid
	void ScaleGridFunction (const MultiIndexType& begin, const MultiIndexType& end, RealType factor);

	//!
	void AddToGridFunction (const MultiIndexType& begin, const MultiIndexType& end,RealType factor,
			GridFunctionType& imagegridfunction);

	//!
	RealType MaxValueGridFunction (const MultiIndexType& begin, const MultiIndexType& end);

private:
	//! gridfunction Datamatrix
	GridFunctionType gridfunction;
	//! dimension of the grid
	MultiIndexType	griddimension;
};



#endif /* GRIDFUNCTION_H_ */
