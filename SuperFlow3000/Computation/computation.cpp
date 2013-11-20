/*
 * computation.cpp
 *
 *  Created on: 08.11.2013
 *      Author: felix
 */
#include<math.h>
#include"computation.h"
#include<iostream>
#include"../Grid/gridfunction.h"
RealType computeTimestep(RealType uMax, RealType vMax, const PointType&h,
        RealType Re, RealType tau){
    RealType minimum = tau*Re/2/(1/h[0]/h[0]+1/h[1]/h[1]);
    if (minimum > h[0]/abs(uMax)) {minimum = h[0]/abs(uMax);}
    if (minimum > h[1]/abs(vMax)) {minimum = h[1]/abs(vMax);}
    return minimum;
}
void computeNewVelocities(GridFunction* u, GridFunction* v,
                                GridFunctionType& f, GridFunctionType& g,
                                GridFunctionType& p, const PointType& delta,
                                RealType deltaT){
    // u und v nach 10 und 11
    std::cout << "Hier muss unbedingt noch iMax, jMax angepasst werden!!!";
    GridFunctionType temp[128][128];
    for (int i=1; i++; i<=128){
        for (IndexType j=1; j++; j<=128) {
            p[i][j] = 1;
        }
    }
    MultiIndexType begin;
    begin[0]=1;
    begin[1]=1;
    MultiIndexType end;
    end[0]=128;
    end[1]=128;
    //u->SetGridFunction(&begin,&end, 1.0, f-deltaT/delta[0]*temp); TODO

}

void computeMomentumEquations(GridFunction* f, GridFunctionType* g,
                                GridFunctionType* u, GridFunctionType* v,
                                GridFunctionType& gx, GridFunctionType& gy,
                                const PointType& h, RealType deltaT) {
    // compute F and G with 12 and 13
    int Rey = 1000;
    std::cout << "Hier muss noch die Reynoldszahl aus simparam genommen werden!!!!";
    RealType ux2;
    RealType uy2;
    RealType u2x;
    RealType uvy;
    //f = u + deltaT(1/Rey(ux2+uy2)-u2x-uvy+gx);
    /*ApplyStencilOperator(const MultiIndexType& gridreadbegin,
                const MultiIndexType& gridreadend,
                const MultiIndexType& gridwritebegin,
                const MultiIndexType& gridwriteend,
                const GridFunction sourcegridfunction,
                GridFunction imagegridfunction);*/
}
void setBoundaryU(GridFunction u){
    RealType value = 0;
    // left -> 0
    MultiIndexType bb(0,0);
    MultiIndexType ee(0,u.griddimension[1]-1);
    u.SetGridFunction(bb,ee,value);
    //right -> 0
    bb[0]= u.griddimension[0]-1; bb[1] = 0;
    ee[0]= u.griddimension[0]-1; ee[1] = u.griddimension[1]-1;
    u.SetGridFunction(bb,ee,value);

    //bottom
    bb[0]= 0; bb[1] = 0;
    ee[0]= u.griddimension[0]-1; ee[1] = 0;
    MultiIndexType offset(0,1);
    u.SetGridFunction(bb,ee,-1,offset);

    //top
    bb[0]= 0; bb[1] = u.griddimension[1]-1;
    ee[0]= u.griddimension[0]-1; ee[1] = u.griddimension[1]-1;
    offset[1]=-1;
    u.SetGridFunction(bb,ee,-1,offset);
}
void setBoundaryV(GridFunction v){
    // left
    MultiIndexType bb (0,0);
    MultiIndexType ee (0,v.griddimension[1]-1);
    MultiIndexType offset(1,0);
    v.SetGridFunction(bb,ee,-1,offset);
    //right
    bb[0] = v.griddimension[0]-1; bb[1] = 0;
    ee[0] = v.griddimension[0]-1; ee[1] = v.griddimension[1]-1;
    offset[0]=-1;
    v.SetGridFunction(bb,ee,-1,offset);

    RealType value = 0;
    //bottom ->0
    bb[0] = 0; bb[1] = 0;
    ee[0] = v.griddimension[0]-1; ee[1] = 0;
    v.SetGridFunction(bb,ee,value);
    //top ->0
    bb[0] = 0; bb[1] = v.griddimension[1]-1;
    ee[0] = v.griddimension[0]-1; ee[1] = v.griddimension[1]-1;
    v.SetGridFunction(bb,ee,value);
}

