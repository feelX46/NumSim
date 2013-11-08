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
