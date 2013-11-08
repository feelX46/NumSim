/*
 * gridfunction.h
 *
 *  Created on: 08.11.2013
 *      Author: Markus
 */

#ifndef GRIDFUNCTION_H_
#define GRIDFUNCTION_H_

#include"../Misc/typedef.h"

class GridFunction {

public:
	/*! Construktor
	   * @param DimX Dimension in X-Direction
	   * @param DimY Dimension in Y-Direction
	   */
	GridFunction(int DimX, int DimY);

	GridFunction(const MultiIndexType griddimension_input);

	//! Destructor
	~GridFunction();

	//! get grid data (pointer to gridfunction)

	GridFunctionType GetGridFunction() const;

	//! get value of index
	RealType GetGridFunction(const MultiIndexType& index);

	//! get dimension
	MultiIndexType GetGridDimension();

	//! set a whole block to value
	void SetGridFunction (const MultiIndexType& begin, const MultiIndexType& end,RealType value);

	//! set a  value
	void SetGridFunction (const IndexType& i, const IndexType& j, RealType value);

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
