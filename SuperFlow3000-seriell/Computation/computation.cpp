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
                                RealType deltaT){
	//compute u
	MultiIndexType bb (u->beginwrite[0],u->beginwrite[1]);
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
	v->AddToGridFunction (bb,ee,-factor,p,offset);
}




void Computation::computeMomentumEquations(GridFunction* f, GridFunction* g,
                                GridFunction* u, GridFunction* v,
                                GridFunction* T,
                                GridFunction& gx, GridFunction& gy,
                                const PointType& h, RealType& deltaT) {
	MultiIndexType dim = f->griddimension;

	Stencil sten(3,h);
	GridFunction derivative (dim,'d'); //character egal... d als Platzhalter
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

	//ToDo: Laut Gleichungen auf Arbeitsblaettern (Gl. 12 und Gl. 46) muesste die Gravitation doppelt in die Gleichung eingehen - eher unwahrscheinlich!
	//ToDo: (markus:) muss glaube ich schon rein... siehe gleichung 35... das ist die 1 in der klammer vor dem g
	f->AddToGridFunction(bwrite,ewrite,-factor,gx);

	// wie derivative
	GridFunction boussinesq(dim,'d');
	boussinesq.SetGridFunction(bread,eread,0);  //set to zero

	factor = param.beta * deltaT * 0.5;
	boussinesq.AddProductToGridFunction(bwrite,ewrite,factor,*T,gx);

	//ToDo: hier wird gerade der offset auch fuer g angewendet... unklar ob das richtig... momentan ist aber g eh skalar!
	MultiIndexType offset;
	offset[0] = 1; offset[1] = 0;
	// ToDo Hier nocheinmal ganz genau ueberlegen, ob das mit dem offset passt - ich glaube fast, dass es falsch ist so - (markus: glaub das passt)
	boussinesq.AddProductToGridFunction(bwrite,ewrite,factor,*T,gy,offset);

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

	// ToDo gy doppelt drin, genau wie gx
	// gleich wie oben... hab es wieder mit reingenommen
	g->AddToGridFunction(bwrite,ewrite,-factor,gy);

	boussinesq.SetGridFunction(bread,eread,0);  //set to zero

	factor = param.beta * deltaT * 0.5;
	boussinesq.AddProductToGridFunction(bwrite,ewrite,factor,*T,gy);
	// ToDo Hier nocheinmal ganz genau ueberlegen, ob das mit dem offset passt - ich glaube fast, dass es falsch ist so
	offset[0] = 0; offset[1] = 1;
	boussinesq.AddProductToGridFunction(bwrite,ewrite,factor,*T,gy,offset);

	factor = -1.0;
	g->AddToGridFunction(bwrite,ewrite,factor,boussinesq);


}
void Computation::setBoundaryU(GridFunction& u){
    RealType value = 0;
    MultiIndexType bb;
    MultiIndexType ee;
    MultiIndexType offset;

    if(u.globalboundary[3]){
    // left -> 0
    	bb[0] = 1; bb[1] = 2;
    	ee[0] = 1; ee[1] = u.griddimension[1]-2;
    	u.SetGridFunction(bb,ee,value);
    }
    //right -> 0
    if(u.globalboundary[1]){
    	bb[0]= u.griddimension[0]-2; bb[1] = 2;
    	ee[0]= u.griddimension[0]-2; ee[1] = u.griddimension[1]-2;
    	u.SetGridFunction(bb,ee,value);
    }

    //bottom
    if(u.globalboundary[0]){
    	bb[0]= 1; bb[1] = 1;
    	ee[0]= u.griddimension[0]-2; ee[1] = 1;
    	offset[0] = 0;
    	offset[1] = 1;
    	u.SetGridFunction(bb,ee,-1,offset);
    }

    //top
    if(u.globalboundary[2]) {
    	bb[0]= 1; bb[1] = u.griddimension[1]-1;
    	ee[0]= u.griddimension[0]-2; ee[1] = u.griddimension[1]-1;
    	offset[0] = 0;
    	offset[1] = -1;
    	u.SetGridFunction(bb,ee,-1,offset);
    }
}


void Computation::setBoundaryV(GridFunction& v){
	MultiIndexType bb;
	MultiIndexType ee;
	MultiIndexType offset;
	RealType value = 0;

	// left
	if(v.globalboundary[3]) {
		bb[0] = 1; bb[1] = 1;
		ee[0] = 1; ee[1] = v.griddimension[1]-2;
		offset[0] = 1;
		offset[1] = 0;
		v.SetGridFunction(bb,ee,-1,offset);
	}

    //bottom ->0
    if(v.globalboundary[0]) {
    	bb[0] = 2; bb[1] = 1;
    	ee[0] = v.griddimension[0]-2; ee[1] = 1;
    	v.SetGridFunction(bb,ee,value);
    }
    //top ->0
    if(v.globalboundary[2]) {
    	bb[0] = 2; bb[1] = v.griddimension[1]-2;
    	ee[0] = v.griddimension[0]-2; ee[1] = v.griddimension[1]-2;
    	v.SetGridFunction(bb,ee,value);
    }

    //right
    if(v.globalboundary[1]) {
    	bb[0] = v.griddimension[0]-1; bb[1] = 1;
    	ee[0] = v.griddimension[0]-1; ee[1] = v.griddimension[1]-2;
    	offset[0]=-1;
    	offset[1] = 0;
    	v.SetGridFunction(bb,ee,-1,offset);
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
		offset[0] = 1;  offset[1] = 0;
		T.SetGridFunction(bb,ee,2*param.TU);
		T.AddToGridFunction (bb,ee, -1, T, offset);
	}

	//top
	if(T.globalboundary[2] && param.boun[2]==0) {
		bb[0] = T.beginread[0]; bb[1] = T.endread[1];
		ee[0] = T.endread[1]; ee[1] = T.endread[1];
		offset[0] = -1;  offset[1] = 0;
		T.SetGridFunction(bb,ee,2*param.TO);
		T.AddToGridFunction (bb,ee, -1, T, offset);
	}

	//right
	if(T.globalboundary[1] && param.boun[1]==0) {
		bb[0] = T.endread[0]; bb[1] = T.beginread[1];
		ee[0] = T.endread[0]; ee[1] = T.endread[1];
		offset[0] = 0;  offset[1] = -1;
		T.SetGridFunction(bb,ee,2*param.TR);
		T.AddToGridFunction (bb,ee, -1, T, offset);
	}

	//left
	if(T.globalboundary[3] && param.boun[3]==0) {
		bb[0] = T.beginread[0]; bb[1] = T.beginread[1];
		ee[0] = T.beginread[0]; ee[1] = T.endread[1];
		offset[0] = 0;  offset[1] = 1;
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
		offset[0] = 1;  offset[1] = 0;
		T.SetGridFunction(bb,ee,h[1]*param.QU);
		T.AddToGridFunction (bb,ee, 1, T, offset);
	}

	//top
	if(T.globalboundary[2] && param.boun[2]==1) {
		bb[0] = T.beginread[0]; bb[1] = T.endread[1];
		ee[0] = T.endread[1]; ee[1] = T.endread[1];
		offset[0] = -1;  offset[1] = 0;
		T.SetGridFunction(bb,ee,h[1]*param.QO);
		T.AddToGridFunction (bb,ee, 1, T, offset);
	}

	//right
	if(T.globalboundary[1] && param.boun[1]==1) {
		bb[0] = T.endread[0]; bb[1] = T.beginread[1];
		ee[0] = T.endread[0]; ee[1] = T.endread[1];
		offset[0] = 0;  offset[1] = -1;
		T.SetGridFunction(bb,ee,h[0]*param.QR);
		T.AddToGridFunction (bb,ee, 1, T, offset);
	}

	//left
	if(T.globalboundary[3] && param.boun[3]==1) {
		bb[0] = T.beginread[0]; bb[1] = T.beginread[1];
		ee[0] = T.beginread[0]; ee[1] = T.endread[1];
		offset[0] = 0;  offset[1] = 1;
		T.SetGridFunction(bb,ee,h[0]*param.QL);
		T.AddToGridFunction (bb,ee, 1, T, offset);
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

