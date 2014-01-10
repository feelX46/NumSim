/*
 * computation.cpp
 *
 *  Created on: 08.11.2013
 *      Author: felix
 */
#include<math.h>
#include"computation.h"
#include"../Stencil/stencil.h"
#include<iostream>
#include"../Grid/gridfunction.h"

Computation::Computation(Simparam param){
		this->param=param;
	}

RealType Computation::computeTimestep (RealType uMax, RealType vMax, const PointType& h){
    RealType value;
	RealType minimum = param.RE/(2*(1/(h[0]*h[0])+1/(h[1]*h[1])));
    value =  h[0]/abs(uMax);
    if (minimum > value) {minimum=value;}
    value = h[1]/abs(vMax);
    if (minimum > value) {minimum=value;}
    value = 0.5*param.RE*param.Pr/(1/(h[0]*h[0])+1/(h[1]*h[1]));
    if (minimum > value) {minimum=value;}
    return param.tau*minimum;
}


void Computation::computeNewVelocities(GridFunction* u, GridFunction* v,
                                GridFunction& f, GridFunction& g,
                                GridFunction& p, const PointType& h,
                                RealType deltaT,
                                GridFunction& geo){

	for (int i = u->beginwrite[0] ; i<= u->endwrite[0]; i++)
	{
		for (int j = u->beginwrite[1] ; j<= u->endwrite[1]; j++)
		{
			int indi/*cator*/ = geo.GetGridFunction()[i][j];
			RealType newU, newV;
			//compute u:
			if (indi >= 24 && indi <= 31)
			{
				newU = f.GetGridFunction(i,j)- deltaT / h[0] * (p.GetGridFunction(i+1,j) - p.GetGridFunction(i,j));
				u->SetGridFunction(i,j,newU);
			}
    		//compute v:
		    if (indi >= 16 && indi%2 == 1)
		    {
		    	newV = g.GetGridFunction(i,j)- deltaT / h[1] * (p.GetGridFunction(i,j+1) - p.GetGridFunction(i,j));
		    	v->SetGridFunction(i,j,newV);
			}
		}
	}
	//compute u
	/*MultiIndexType bb (u->beginwrite[0],u->beginwrite[1]);
	MultiIndexType ee(u->endwrite[0],u->endwrite[1]);
	u->SetGridFunction(bb,ee,1,f);
	RealType factor = -deltaT/h[0];
	MultiIndexType offset (1,0);
	u->AddToGridFunction (bb,ee, factor,p,offset);
	offset[0]=0;
	u->AddToGridFunction (bb,ee,-factor,p,offset);

	//compute v
	bb[0] = v->beginwrite[0]; bb[1] = v->beginwrite[1];
	ee[0] = v->endwrite[0]; ee[1] = v->endwrite[1];
	v->SetGridFunction(bb,ee,1,g);
	factor = -deltaT/h[1];
	offset[1]=1;
	v->AddToGridFunction (bb,ee, factor,p,offset);
	offset[1]=0;
	v->AddToGridFunction (bb,ee,-factor,p,offset);*/
}


void Computation::computeMomentumEquations(GridFunction* f,
		                        GridFunction* g,
                                GridFunction* u,
                                GridFunction* v,
                                GridFunction* T,
                                GridFunction& gx,
                                GridFunction& gy,
                                const PointType& h,
                                RealType& deltaT,
                                GridFunction& geo) {
	MultiIndexType thisCellX, thisCellY;
	MultiIndexType dim = f->griddimension;
    Stencil stenU(3,h);
    Stencil stenV(3,h);
    GridFunction derivativeUxx (dim,'d');
    derivativeUxx.SetGridFunction(u->beginread,u->endread,0);
    stenU.ApplyFxxStencilOperator(u->beginread,u->endread,u->beginwrite,u->endwrite,u->GetGridFunction(),derivativeUxx);

    GridFunction derivativeUyy (dim,'d');
    derivativeUyy.SetGridFunction(u->beginread,u->endread,0);
    stenU.ApplyFyyStencilOperator(u->beginread,u->endread,u->beginwrite,u->endwrite,u->GetGridFunction(),derivativeUyy);

    GridFunction derivativeUSquarex (dim,'d');
    derivativeUSquarex.SetGridFunction(u->beginread,u->endread,0);
    stenU.ApplyUSqxStencilOperator(u->beginread,u->endread,u->beginwrite,u->endwrite,u->GetGridFunction(),
    		                      derivativeUSquarex,param.alpha);

    GridFunction derivativeUVy (dim,'d');
    derivativeUVy.SetGridFunction(u->beginread,u->endread,0);
    stenU.ApplyUVyStencilOperator(u->beginread,u->endread,u->beginwrite,u->endwrite,u->GetGridFunction(),
    		                     v->GetGridFunction(),derivativeUVy,param.alpha);

    GridFunction boussinesq(dim,'d');
    boussinesq.SetGridFunction(f->beginread,f->endread,0);
    RealType factor = param.beta * deltaT * 0.5;
    boussinesq.AddProductToGridFunction(f->beginwrite,f->endwrite,factor,*T,gx);
    MultiIndexType offset;
    offset[0] = 1; offset[1] = 0;
    boussinesq.AddProductToGridFunction(f->beginwrite,f->endwrite,factor,*T,gx,offset);

    for (int i = f->beginwrite[0] ; i<= f->endwrite[0]; i++)
    	{
    			for (int j = f->beginwrite[1] ; j<= f->endwrite[1]; j++)
    		{
    			int indi = geo.GetGridFunction()[i][j];
    			RealType newF;
    	    	//compute u:
    			if (indi >= 24 && indi <= 31)
    			{
    			    newF = u->GetGridFunction(i,j)
    			    	 + deltaT * (1/ param.RE *(derivativeUxx.GetGridFunction(i,j) + derivativeUyy.GetGridFunction(i,j))
    			    			     - derivativeUSquarex.GetGridFunction(i,j) - derivativeUVy.GetGridFunction(i,j) + gx.GetGridFunction(i,j))
    			         - boussinesq.GetGridFunction(i,j);
    			    f->SetGridFunction(i,j,newF);
    			}
    		}
    	}

    GridFunction derivativeUVx (dim,'d');
    derivativeUVx.SetGridFunction(v->beginread,v->endread,0);
    stenU.ApplyUVxStencilOperator(v->beginread,v->endread,v->beginwrite,v->endwrite,u->GetGridFunction(),
      		                     v->GetGridFunction(),derivativeUVx,param.alpha);

    GridFunction derivativeVxx (dim,'d');
    derivativeVxx.SetGridFunction(v->beginread,v->endread,0);
    stenV.ApplyFxxStencilOperator(v->beginread,v->endread,v->beginwrite,v->endwrite,v->GetGridFunction(),derivativeVxx);

    GridFunction derivativeVyy (dim,'d');
    derivativeVyy.SetGridFunction(v->beginread,v->endread,0);
    stenV.ApplyFyyStencilOperator(v->beginread,v->endread,v->beginwrite,v->endwrite,v->GetGridFunction(),derivativeVyy);

    GridFunction derivativeVSquarey (dim,'d');
    derivativeVSquarey.SetGridFunction(v->beginread,v->endread,0);
    stenV.ApplyVSqyStencilOperator(v->beginread,v->endread,v->beginwrite,v->endwrite,v->GetGridFunction(),derivativeVSquarey,
    		                      param.alpha);

    boussinesq.SetGridFunction(g->beginread,g->endread,0);
    boussinesq.AddProductToGridFunction(g->beginwrite,g->endwrite,factor,*T,gy);
    offset[0] = 0; offset[1] = 1;
    boussinesq.AddProductToGridFunction(g->beginread, g->endwrite,factor,*T,gy,offset);

   	for (int i = g->beginwrite[0] ; i<= g->endwrite[0]; i++)
	{
			for (int j = g->beginwrite[1] ; j<= g->endwrite[1]; j++)
		{
			int indi = geo.GetGridFunction()[i][j];
			RealType newG;
			//compute v:
			if (indi >= 16 && indi%2 == 1)
			{
			   	newG = v->GetGridFunction(i,j)
		              + deltaT * (1/ param.RE *(derivativeVxx.GetGridFunction(i,j) + derivativeVyy.GetGridFunction(i,j))
		    			          - derivativeVSquarey.GetGridFunction(i,j) - derivativeUVx.GetGridFunction(i,j) + gy.GetGridFunction(i,j))
		    		  - boussinesq.GetGridFunction(i,j);
			   	g->SetGridFunction(i,j,newG);
			}
		}
	}

	/*GridFunction derivative (dim,'d'); //character egal... d als Platzhalter
	RealType factor;
	//  --  compute F  --
	MultiIndexType bread;
	bread = f-> beginread;
	MultiIndexType eread;
	eread = f-> endread;
	MultiIndexType bwrite;
	bwrite = f-> beginwrite;
	MultiIndexType ewrite;
	ewrite = f-> endwrite;

	derivative.SetGridFunction(bread,eread,0);  //set to zero
	//add u
	f->SetGridFunction(bwrite,ewrite,1,*u);
	//add derivatives:
	sten.ApplyFxxStencilOperator(bread,eread,bwrite,ewrite,u->GetGridFunction(),derivative);
	factor = deltaT/param.RE;
	f->AddToGridFunction(bwrite,ewrite,factor,derivative);

	sten.ApplyFyyStencilOperator(bread,eread,bwrite,ewrite,u->GetGridFunction(),derivative);
	f->AddToGridFunction(bwrite,ewrite,factor,derivative);

	factor=-deltaT;
	sten.ApplyUSqxStencilOperator(bread,eread,bwrite,ewrite,(u->GetGridFunction()),derivative,param.alpha);
	f->AddToGridFunction(bwrite,ewrite,factor,derivative);

	sten.ApplyUVyStencilOperator(bread,eread,bwrite,ewrite,(u->GetGridFunction()),(v->GetGridFunction()),derivative,param.alpha);
	f->AddToGridFunction(bwrite,ewrite,factor,derivative);

	f->AddToGridFunction(bwrite,ewrite,-factor,gx);

	// wie derivative
	GridFunction boussinesq(dim,'d');
	boussinesq.SetGridFunction(bread,eread,0);  //set to zero

	factor = param.beta * deltaT * 0.5;
	boussinesq.AddProductToGridFunction(bwrite,ewrite,factor,*T,gx);

	// hier wird gerade der offset auch fuer g angewendet... unklar ob das richtig... momentan ist aber g eh skalar!
	MultiIndexType offset;
	offset[0] = 1; offset[1] = 0;
	// Hier nocheinmal ganz genau ueberlegen, ob das mit dem offset passt - ich glaube fast, dass es falsch ist so - (markus: glaub das passt)
	boussinesq.AddProductToGridFunction(bwrite,ewrite,factor,*T,gx,offset);

	factor = -1.0;
	f->AddToGridFunction(bwrite,ewrite,factor,boussinesq);

	//  --  compute G  --
	bread = g->beginread;
	eread = g->endread;
	bwrite = g->beginwrite;
	ewrite = g->endwrite;

	derivative.SetGridFunction(bread,eread,0);  //set derivative to zero

	//add v
	g->SetGridFunction(bwrite,ewrite,1,*v);
	//add derivatives:
	sten.ApplyFxxStencilOperator(bread,eread,bwrite,ewrite,v->GetGridFunction(),derivative);
	factor = deltaT/param.RE;
	g->AddToGridFunction(bwrite,ewrite,factor,derivative);

	sten.ApplyFyyStencilOperator(bread,eread,bwrite,ewrite,v->GetGridFunction(),derivative);
	g->AddToGridFunction(bwrite,ewrite,factor,derivative);

	factor=-deltaT;
	sten.ApplyVSqyStencilOperator(bread,eread,bwrite,ewrite,v->GetGridFunction(),derivative,param.alpha);
	g->AddToGridFunction(bwrite,ewrite,factor,derivative);

	sten.ApplyUVxStencilOperator(bread,eread,bwrite,ewrite,u->GetGridFunction(),v->GetGridFunction(),derivative,param.alpha);
	g->AddToGridFunction(bwrite,ewrite,factor,derivative);

	// gy doppelt drin, genau wie gx
	// gleich wie oben... hab es wieder mit reingenommen
	g->AddToGridFunction(bwrite,ewrite,-factor,gy);

	boussinesq.SetGridFunction(bread,eread,0);  //set to zero

	factor = param.beta * deltaT * 0.5;
	boussinesq.AddProductToGridFunction(bwrite,ewrite,factor,*T,gy);
	offset[0] = 0; offset[1] = 1;
	boussinesq.AddProductToGridFunction(bwrite,ewrite,factor,*T,gy,offset);

	factor = -1.0;
	g->AddToGridFunction(bwrite,ewrite,factor,boussinesq);
*/

}
void Computation::setBoundaryU(GridFunction& u){
    MultiIndexType bb;
    MultiIndexType ee;
    MultiIndexType offset;

    // left
    if(u.globalboundary[3]){
		bb[0] = 1; bb[1] = 2;
		ee[0] = 1; ee[1] = u.griddimension[1]-2;
    	//Haft und Rutsch -> 0
    	if (param.WL ==1 || param.WL == 2){
    		u.SetGridFunction(bb,ee,0);
    	}
    	if (param.WL == 3){ //Ausström
    		offset[0]=1;
    		offset[1]=0;
			u.SetGridFunction(bb,ee,1,offset);
    	}
    	if (param.WL == 4){
    		u.SetGridFunction(bb,ee,param.Uflow);
    	}
    }
    //right
    if(u.globalboundary[1]){
    	//Haft und Rutsch  -> 0
    	bb[0]= u.griddimension[0]-2; bb[1] = 2;
    	ee[0]= u.griddimension[0]-2; ee[1] = u.griddimension[1]-2;
    	if (param.WR ==1 || param.WR == 2){
    		u.SetGridFunction(bb,ee,0);
    	}
    	if (param.WR == 3){
    		offset[0]=-1;
    		offset[1]=0;
			u.SetGridFunction(bb,ee,1,offset);
    	}
    	if (param.WR == 4){
    		u.SetGridFunction(bb,ee,param.Uflow);
    	}
    }
    //bottom
    if(u.globalboundary[0]){
    	bb[0]= 1; bb[1] = 1;
    	ee[0]= u.griddimension[0]-2; ee[1] = 1;
		offset[0] = 0;
		offset[1] = 1;
    	if (param.WU == 1){ //Haft
			u.SetGridFunction(bb,ee,-1,offset);
		}
    	if (param.WU == 2 || param.WU ==3){ //Rutsch und Ausström
			u.SetGridFunction(bb,ee,1,offset);
    	}
    	if (param.WU == 4){
    		u.SetGridFunction(bb,ee,param.Uflow);
    	}
    }


    //top
    if(u.globalboundary[2]) {
    	bb[0]= 1; bb[1] = u.griddimension[1]-1;
    	ee[0]= u.griddimension[0]-2; ee[1] = u.griddimension[1]-1;
   		offset[0] = 0;
    	offset[1] = -1;
    	if (param.WO == 1){ //Haft
    		u.SetGridFunction(bb,ee,-1,offset);
    	}
    	if (param.WO == 2 || param.WO == 3){ //Rutsch und Ausström
    		u.SetGridFunction(bb,ee,1,offset);
    	}
    	if (param.WO == 4){
    		u.SetGridFunction(bb,ee,param.Uflow);
    	}
    }
}


void Computation::setBoundaryV(GridFunction& v){
	MultiIndexType bb;
	MultiIndexType ee;
	MultiIndexType offset;

	// left
	if(v.globalboundary[3]) {
		bb[0] = 1; bb[1] = 1;
		ee[0] = 1; ee[1] = v.griddimension[1]-2;
		offset[0] = 1;
		offset[1] = 0;
		if (param.WL==1){//Haft
			v.SetGridFunction(bb,ee,-1,offset);
		}
		if (param.WL ==2 || param.WL == 3){//Rutsch
			v.SetGridFunction(bb,ee,1,offset);
		}
    	if (param.WL == 4){
    		v.SetGridFunction(bb,ee,param.Vflow);
    	}
	}

    //right
    if(v.globalboundary[1]) {
    	bb[0] = v.griddimension[0]-1; bb[1] = 1;
    	ee[0] = v.griddimension[0]-1; ee[1] = v.griddimension[1]-2;
		offset[0]=-1;
		offset[1] = 0;
    	if (param.WR == 1){ //Haft
    		v.SetGridFunction(bb,ee,-1,offset);
    	}
    	if (param.WR == 2 || param.WR == 3){ //Rutsch
    		v.SetGridFunction(bb,ee,1,offset);
    	}
    	if (param.WR == 4){
        	v.SetGridFunction(bb,ee,param.Vflow);
    	}
    }

    //bottom
    if(v.globalboundary[0]) {
    	bb[0] = 2; bb[1] = 1;
    	ee[0] = v.griddimension[0]-2; ee[1] = 1;
    	if (param.WU == 1 || param.WU ==2){     	//Haft und Rutsch  -> 0
    		v.SetGridFunction(bb,ee,0);
    	}
    	if (param.WU == 3){
    		offset[0]=0;
    		offset[1]=1;
    		v.SetGridFunction(bb,ee,1,offset);
    	}
    	if (param.WU == 4){
    		v.SetGridFunction(bb,ee,param.Vflow);
    	}
    }
    //top
    if(v.globalboundary[2]) {
    	bb[0] = 2; bb[1] = v.griddimension[1]-2;
    	ee[0] = v.griddimension[0]-2; ee[1] = v.griddimension[1]-2;

    	if (param.WO == 1 || param.WO ==2){     	//Haft und Rutsch  -> 0
    		v.SetGridFunction(bb,ee,0);
    	}
    	if (param.WO == 3){
    		offset[0]=0;
    		offset[1]=-1;
    		v.SetGridFunction(bb,ee,1,offset);
    	}
    	if (param.WO == 4){
    	 	v.SetGridFunction(bb,ee,param.Vflow);
    	}
    }


}

void Computation::setBoundaryP(GridFunction& p){
	MultiIndexType bb;
	MultiIndexType ee;
	MultiIndexType offset;

	// left
	if(p.globalboundary[3]) {
		bb[0] = p.beginread[0]; bb[1] = p.beginread[1];
		ee[0] = p.beginread[0]; ee[1] = p.endread[1];
		offset[0] = 1;
		offset[1] = 0;
		p.SetGridFunction(bb,ee,1,offset);
	}
    //right
	if(p.globalboundary[1]) {
		bb[0] = p.endread[0]; bb[1] = p.beginread[1];
		ee[0] = p.endread[0]; ee[1] = p.endread[1];
		offset[0]=-1;
		offset[1] = 0;
		p.SetGridFunction(bb,ee,1,offset);
	}
    //bottom
	if(p.globalboundary[0]) {
		bb[0] = p.beginread[0]; bb[1] = p.beginread[1];
		ee[0] = p.endread[0]; ee[1] = p.beginread[1];
		offset[0]=0; offset[1]=1;
		p.SetGridFunction(bb,ee,1,offset);
	}
    //top
	if(p.globalboundary[2]) {
		bb[0] = p.beginread[0]; bb[1] = p.endread[1];
		ee[0] = p.endread[0]; ee[1] = p.endread[1];
		offset[0]=0; offset[1]=-1;
		p.SetGridFunction(bb,ee,1,offset);
	}
}

void Computation::setBoundaryF(GridFunction& f, GridFunction& u){
	MultiIndexType bb;
	MultiIndexType ee;

	// left
	if(f.globalboundary[2]) {
		bb[0] = f.beginread[0]; bb[1] = f.beginread[1];
		ee[0] = f.beginread[0]; ee[1] = f.endread[1];
		f.SetGridFunction(bb,ee,1,u);
	}
    //right
	if(f.globalboundary[1]) {
		bb[0] = f.endread[0]; bb[1] = f.beginread[1];
		ee[0] = f.endread[0]; ee[1] = f.endread[1];
		f.SetGridFunction(bb,ee,1,u);
	}
}

void Computation::setBoundaryG(GridFunction& g, GridFunction& v){
	MultiIndexType bb;
	MultiIndexType ee;

	//bottom
	if(g.globalboundary[0]) {
		bb[0] = g.beginread[0]; bb[1] = g.beginread[1];
		ee[0] = g.endread[0]; ee[1] = g.beginread[1];
		g.SetGridFunction(bb,ee,1,v);
	}
    //top
	if(g.globalboundary[2]) {
		bb[0] = g.beginread[0]; bb[1] = g.endread[1];
		ee[0] = g.endread[1]; ee[1] = g.endread[1];
		g.SetGridFunction(bb,ee,1,v);
	}
}

void Computation::setBoundaryTD(GridFunction& T) {
	MultiIndexType bb;
	MultiIndexType ee;
	MultiIndexType offset;
	// ToDo Hier nochmal nachschauen - Gleichung (42) ist viel komplizierter aufgeschrieben
	// ToDo (markus: hab hier einiges angepasst! - jetzt gleich wie (42))

	//bottom
	if(T.globalboundary[0] && param.boun[0]==0) {
		bb[0] = T.beginread[0]; bb[1] = T.beginread[1];
		ee[0] = T.endread[0]; ee[1] = T.beginread[1];
		offset[0] = 0;  offset[1] = 1;
		T.SetGridFunction(bb,ee,2*param.TU); // wohin geht das (i-0.5) aus Gleichung (42)?
		T.AddToGridFunction (bb,ee, -1, T, offset);
	}

	//top
	if(T.globalboundary[2] && param.boun[2]==0) {
		bb[0] = T.beginread[0]; bb[1] = T.endread[1];
		ee[0] = T.endread[1]; ee[1] = T.endread[1];
		offset[0] = 0;  offset[1] = -1;
		T.SetGridFunction(bb,ee,2*param.TO);
		T.AddToGridFunction (bb,ee, -1, T, offset);
	}

	//right
	if(T.globalboundary[1] && param.boun[1]==0) {
		bb[0] = T.endread[0]; bb[1] = T.beginread[1];
		ee[0] = T.endread[0]; ee[1] = T.endread[1];
		offset[0] = -1;  offset[1] = 0;
		T.SetGridFunction(bb,ee,2*param.TR);
		T.AddToGridFunction (bb,ee, -1, T, offset);
	}

	//left
	if(T.globalboundary[3] && param.boun[3]==0) {
		bb[0] = T.beginread[0]; bb[1] = T.beginread[1];
		ee[0] = T.beginread[0]; ee[1] = T.endread[1];
		offset[0] = 1;  offset[1] = 0;
		T.SetGridFunction(bb,ee,2*param.TL);
		T.AddToGridFunction (bb,ee, -1, T, offset);
	}

}

void Computation::setBoundaryTN(GridFunction& T,const PointType& h) {
	MultiIndexType bb;
	MultiIndexType ee;
	MultiIndexType offset;

	//bottom
	if(T.globalboundary[0] && param.boun[0]==1) {
		bb[0] = T.beginread[0]; bb[1] = T.beginread[1];
		ee[0] = T.endread[0]; ee[1] = T.beginread[1];
		offset[0] = 0;  offset[1] = 1;
		T.SetGridFunction(bb,ee,h[1]*param.QU);
		T.AddToGridFunction (bb,ee, 1, T, offset);
	}

	//top
	if(T.globalboundary[2] && param.boun[2]==1) {
		bb[0] = T.beginread[0]; bb[1] = T.endread[1];
		ee[0] = T.endread[1]; ee[1] = T.endread[1];
		offset[0] = 0;  offset[1] = -1;
		T.SetGridFunction(bb,ee,h[1]*param.QO);
		T.AddToGridFunction (bb,ee, 1, T, offset);
	}

	//right
	if(T.globalboundary[1] && param.boun[1]==1) {
		bb[0] = T.endread[0]; bb[1] = T.beginread[1];
		ee[0] = T.endread[0]; ee[1] = T.endread[1];
		offset[0] = -1;  offset[1] = 0;
		T.SetGridFunction(bb,ee,h[0]*param.QR);
		T.AddToGridFunction (bb,ee, 1, T, offset);
	}

	//left
	if(T.globalboundary[3] && param.boun[3]==1) {
		bb[0] = T.beginread[0]; bb[1] = T.beginread[1];
		ee[0] = T.beginread[0]; ee[1] = T.endread[1];
		offset[0] = 1;  offset[1] = 0;
		T.SetGridFunction(bb,ee,h[0]*param.QL);
		T.AddToGridFunction (bb,ee, 1, T, offset);
	}

}

void Computation::setBarrierBoundaryU(GridFunction& velocity_x, GridFunction& geo)
{
	int indicator;

	for (int i = velocity_x.beginwrite[0]; i <= velocity_x.endwrite[0]; i++)
	{
		for  (int j = velocity_x.beginwrite[1]; j <= velocity_x.endwrite[1]; j++ )
		{
			indicator = geo.GetGridFunction()[i][j];
			if(indicator < 16) // generell Behandlung nur von Hinderniszellen
			{
				switch(indicator)
				{
				    //Fehlerfälle:
				    case 3:
				    case 7:
				    case 11:
				    case 12:
				    case 13:
				    case 14:
				    case 15:
				    	std::cout << "Unzulässige Geometrie!"; break;


				    //nothing to do:
				    case 0:
				    	break;

				    //1: North
				    case 1:
				    	velocity_x.SetGridFunction(i,j,-velocity_x.GetGridFunction()[i][j+1]);
				    	velocity_x.SetGridFunction(i-1,j,-velocity_x.GetGridFunction()[i-1][j+1]); //ToDo: wirklich nötig, dass doppelt behandelt?
                        break;

				    //South:
				    case 2:
				    	velocity_x.SetGridFunction(i,j,-velocity_x.GetGridFunction()[i][j-1]);
				    	velocity_x.SetGridFunction(i-1,j,-velocity_x.GetGridFunction()[i-1][j-1]);
                        break;

				    //West
				    case 4:
				    	velocity_x.SetGridFunction(i-1,j,0);
                        break;

				    //East:
				    case 8:
				    	velocity_x.SetGridFunction(i,j,0);
                        break;

				    //North-west:
				    case 5:
				    	velocity_x.SetGridFunction(i-1,j,0);
				    	velocity_x.SetGridFunction(i,j,-velocity_x.GetGridFunction()[i][j+1]);
                        break;

				    //South-west:
				    case 6:
				    	velocity_x.SetGridFunction(i-1,j,0);
				    	velocity_x.SetGridFunction(i,j,-velocity_x.GetGridFunction()[i][j-1]);
                        break;

				    //North-east:
				    case 9:
				    	velocity_x.SetGridFunction(i,j,0);
				    	velocity_x.SetGridFunction(i-1,j,-velocity_x.GetGridFunction()[i-1][j+1]);
                        break;

				    //South-east:
				    case 10:
				    	velocity_x.SetGridFunction(i,j,0);
				    	velocity_x.SetGridFunction(i-1,j,-velocity_x.GetGridFunction()[i-1][j-1]);
                        break;
				}
			}
		}
	}
}

void Computation::setBarrierBoundaryV(GridFunction& velocity_y, GridFunction& geo){

	int indicator;

	for (int i = velocity_y.beginwrite[0]; i <= velocity_y.endwrite[0]; i++)
	{
		for  (int j = velocity_y.beginwrite[1]; j <= velocity_y.endwrite[1]; j++ )
		{
			indicator = geo.GetGridFunction()[i][j];
			if(indicator < 16) // generell Behandlung nur von Hinderniszellen
			{
				switch(indicator)
				{
				    //Fehlerfälle:
				    case 3:
				    case 7:
				    case 11:
				    case 12:
				    case 13:
				    case 14:
				    case 15:
				    	std::cout << "Unzulässige Geometrie!"; break;


				    //nothing to do:
				    case 0:
				    	break;

				    //1: North
				    case 1:
				    	velocity_y.SetGridFunction(i,j,0);
                        break;

				    //South:
				    case 2:
				    	velocity_y.SetGridFunction(i-1,j,0);
                        break;

				    //West
				    case 4:
				    	velocity_y.SetGridFunction(i,j,-velocity_y.GetGridFunction()[i-1][j]);
				    	velocity_y.SetGridFunction(i,j-1,-velocity_y.GetGridFunction()[i-1][j-1]);
                        break;

				    //East:
				    case 8:
				    	velocity_y.SetGridFunction(i,j,-velocity_y.GetGridFunction()[i+1][j]);
				    	velocity_y.SetGridFunction(i,j-1,-velocity_y.GetGridFunction()[i+1][j-1]);
                        break;

				    //North-west:
				    case 5:
				    	velocity_y.SetGridFunction(i,j,0);
				    	velocity_y.SetGridFunction(i,j-1,-velocity_y.GetGridFunction()[i-1][j-1]); //ToDo: ist der wirklich nötig?
                        break;

				    //South-west:
				    case 6:
				    	velocity_y.SetGridFunction(i,j,-velocity_y.GetGridFunction()[i-1][j]);
				    	velocity_y.SetGridFunction(i,j-1,0);
                        break;

				    //North-east:
				    case 9:
				    	velocity_y.SetGridFunction(i,j,0);
				    	velocity_y.SetGridFunction(i,j-1,-velocity_y.GetGridFunction()[i+1][j-1]); //ToDo: ist der wirklich nötig?
                        break;

				    //South-east:
				    case 10:
				    	velocity_y.SetGridFunction(i,j-1,0);
				    	velocity_y.SetGridFunction(i,j,-velocity_y.GetGridFunction()[i+1][j]);
                        break;
				}
			}
		}
	}

}
void Computation::setBarrierBoundaryP(GridFunction& pressure, GridFunction& geo, const PointType& h)
{
	int indicator;

	for (int i = pressure.beginwrite[0]; i <= pressure.endwrite[0]; i++)
	{
		for  (int j = pressure.beginwrite[1]; j <= pressure.endwrite[1]; j++ )
		{
			indicator = geo.GetGridFunction()[i][j];
			if(indicator < 16) // generell Behandlung nur von Hinderniszellen
			{
				switch(indicator)
				{
				    //Fehlerfälle:
				    case 3:
				    case 7:
				    case 11:
				    case 12:
				    case 13:
				    case 14:
				    case 15:
				    	std::cout << "Unzulässige Geometrie!"; break;


				    //nothing to do:
				    case 0:
				    	break;

				    //1: North
				    case 1:
				    	pressure.SetGridFunction(i,j,pressure.GetGridFunction()[i][j+1]);
				    	break;

				    //South:
				    case 2:
				    	pressure.SetGridFunction(i,j,pressure.GetGridFunction()[i][j+1]);
                        break;

				    //West
				    case 4:
				    	pressure.SetGridFunction(i,j,pressure.GetGridFunction()[i-1][j]);
                        break;

				    //East:
				    case 8:
				    	pressure.SetGridFunction(i,j,pressure.GetGridFunction()[i+1][j]);
                        break;

				    //North-west:
				    case 5:
				    	pressure.SetGridFunction(i,j,1/(h[0]*h[0]+h[1]*h[1])*
				    	(h[0]*h[0]*pressure.GetGridFunction()[i][j+1])+ h[1]*h[1]*pressure.GetGridFunction()[i-1][j]);
                        break;

				    //South-west:
				    case 6:
				    	pressure.SetGridFunction(i,j,1/(h[0]*h[0]+h[1]*h[1])*
				        (h[0]*h[0]*pressure.GetGridFunction()[i][j-1])+ h[1]*h[1]*pressure.GetGridFunction()[i-1][j]);
                        break;

				    //North-east:
				    case 9:
				    	pressure.SetGridFunction(i,j,1/(h[0]*h[0]+h[1]*h[1])*
				    	(h[0]*h[0]*pressure.GetGridFunction()[i][j+1])+ h[1]*h[1]*pressure.GetGridFunction()[i+1][j]);
                        break;

				    //South-east:
				    case 10:
				    	pressure.SetGridFunction(i,j,1/(h[0]*h[0]+h[1]*h[1])*
				    	(h[0]*h[0]*pressure.GetGridFunction()[i][j-1])+ h[1]*h[1]*pressure.GetGridFunction()[i+1][j]);
                        break;
				}
			}
		}
	}

}
void Computation::setBarrierBoundaryF(GridFunction& f, GridFunction& u, GridFunction& geo)
{
	int indicator;

		for (int i = f.beginwrite[0]; i <= f.endwrite[0]; i++)
		{
			for  (int j = f.beginwrite[1]; j <= f.endwrite[1]; j++ )
			{
				indicator = geo.GetGridFunction()[i][j];
				if(indicator < 16) // generell Behandlung nur von Hinderniszellen
				{
					switch(indicator)
					{
					    //Fehlerfälle:
					    case 3:
					    case 7:
					    case 11:
					    case 12:
					    case 13:
					    case 14:
					    case 15:
					    	std::cout << "Unzulässige Geometrie!"; break;


					    //nothing to do:
					    case 0:
					    case 1:
					    case 2:
					    	break;

					    //West
					    case 4:
					    	f.SetGridFunction(i-1,j,u.GetGridFunction()[i-1][j]);
	                        break;

					    //East:
					    case 8:
					    	f.SetGridFunction(i,j,u.GetGridFunction()[i][j]);
	                        break;

					    //North-west:
					    case 5:
					    	f.SetGridFunction(i-1,j,u.GetGridFunction()[i-1][j]);
					    	break;

					    //South-west:
					    case 6:
					    	f.SetGridFunction(i-1,j,u.GetGridFunction()[i-1][j]);
					    	break;

					    //North-east:
					    case 9:
					    	f.SetGridFunction(i,j,u.GetGridFunction()[i][j]);
					    	break;

					    //South-east:
					    case 10:
					    	f.SetGridFunction(i,j,u.GetGridFunction()[i][j]);
					    	break;
					}
				}
			}
		}
}

void Computation::setBarrierBoundaryG(GridFunction& g, GridFunction& v, GridFunction& geo)
{
	int indicator;

		for (int i = g.beginwrite[0]; i <= g.endwrite[0]; i++)
		{
			for  (int j = g.beginwrite[1]; j <= g.endwrite[1]; j++ )
			{
				indicator = geo.GetGridFunction()[i][j];
				if(indicator < 16) // generell Behandlung nur von Hinderniszellen
				{
					switch(indicator)
					{
					    //Fehlerfälle:
					    case 3:
					    case 7:
					    case 11:
					    case 12:
					    case 13:
					    case 14:
					    case 15:
					    	std::cout << "Unzulässige Geometrie!"; break;


					    //nothing to do:
					    case 0:
					    case 4:
					    case 8:
					    	break;

					    //North:
					    case 1:
					    	g.SetGridFunction(i,j,v.GetGridFunction()[i][j]);
	                        break;

					    //South:
					    case 2:
					    	g.SetGridFunction(i,j-1,v.GetGridFunction()[i][j-1]);
	                        break;

					    //North-west:
					    case 5:
					    	g.SetGridFunction(i,j,v.GetGridFunction()[i][j]);
					    	break;

					    //South-west:
					    case 6:
					    	g.SetGridFunction(i,j-1,v.GetGridFunction()[i][j-1]);
					    	break;

					    //North-east:
					    case 9:
					    	g.SetGridFunction(i,j,v.GetGridFunction()[i][j]);
					    	break;

					    //South-east:
					    case 10:
					    	g.SetGridFunction(i,j-1,v.GetGridFunction()[i][j-1]);
					    	break;
					}
				}
			}
		}
}

void Computation::computeRighthandSide(GridFunction* rhs,
    		GridFunction& f,
    		GridFunction& g,
    		const PointType& delta,
    		RealType deltaT){

	// alle Werte mit 0 initialisieren
	MultiIndexType bwrite (0,0);
	MultiIndexType ewrite (rhs->griddimension[0]-1,rhs->griddimension[1]-1);
	rhs->SetGridFunction(bwrite,ewrite,0);

	bwrite = rhs->beginwrite;
	ewrite = rhs->endwrite;

	RealType factor = 1/(deltaT*delta[0]);
	MultiIndexType offset (0,0);
	rhs->AddToGridFunction(bwrite,ewrite,factor,f,offset);
	offset[0]=-1;
	rhs->AddToGridFunction(bwrite,ewrite,-factor,f,offset);
	factor = (1/(deltaT*delta[1]));
	offset[0]=0;
	rhs->AddToGridFunction(bwrite,ewrite,factor,g,offset);
	offset[1]=-1;
	rhs->AddToGridFunction(bwrite,ewrite,-factor,g,offset);
}

void Computation::computeTemperature(GridFunction& T,
									 GridFunction& u,
									 GridFunction& v,
									 GridFunction& q,
									 const PointType& h,
									 RealType& deltaT) {
	//Voraus-Initialisierungen
	MultiIndexType dim = T.griddimension;

	Stencil sten(3,h);

	MultiIndexType bread (T.beginread[0],T.beginread[1]);
	MultiIndexType eread(T.endread[0],T.endread[1]);

	MultiIndexType bwrite (T.beginwrite[0],T.beginwrite[1]);
	MultiIndexType ewrite(T.endwrite[0],T.endwrite[1]);

	GridFunction derivative (dim,'d'); //Temperatur wie Druck im Zellmittelpunkt
	derivative.SetGridFunction(bread,eread,0);  //set to zero

	RealType factor = 1.0;

	// Setze die Temperaturen gemaess Gl. 40 und 41 auf Blatt 5
	//Setze alte Temperaturwerte
	T.SetGridFunction(bwrite,ewrite,factor,T);
	// Addiere Waermequelle q
	factor = deltaT;
	T.AddToGridFunction(bwrite, ewrite, factor, q);

	//Addiere Term: 1/Re 1/Pr (Txx + Tyy)
	factor = deltaT/param.RE /param.Pr;

	sten.ApplyFxxStencilOperator(bread,eread,bwrite,ewrite,T.GetGridFunction(),derivative);
	T.AddToGridFunction(bwrite,ewrite,factor,derivative);

	sten.ApplyFyyStencilOperator(bread,eread,bwrite,ewrite,T.GetGridFunction(),derivative);
	T.AddToGridFunction(bwrite,ewrite,factor,derivative);

	//Addiere Term -(uT)x
	factor = -1 * deltaT;
	//ToDo in Gleichung 40/41 steht statt alpha ein gamma - aehnliche Bedeutung, aber vielleicht doch nicht die gleiche zahl?? (Markus: glaub das passt so)
	sten.ApplyUTxStencilOperator(bread,eread,bwrite,ewrite,u.GetGridFunction(),T.GetGridFunction(),derivative,param.alpha);
	T.AddToGridFunction(bwrite,ewrite,factor,derivative);

	//Addiere Term -(vT)y
	factor = -1 * deltaT;
	// ToDo alpha / gamma Problem wie eben
	sten.ApplyVTyStencilOperator(bread,eread,bwrite,ewrite,v.GetGridFunction(),T.GetGridFunction(),derivative,param.alpha);
	T.AddToGridFunction(bwrite,ewrite,factor,derivative);
}

