/*
 * simparam.h
 *
 *  Created on: 05.11.2013
 *      Author: felix
 */

#ifndef SIMPARAM_H_
#define SIMPARAM_H_

#include "../Misc/template.h"
struct Simparam {
	RealType xLength;  //Gebietslï¿½nge in x-Richtung
	RealType yLength;  //Gebietslï¿½nge in y.Richtung
	int iMax;          //Anzahl der inneren Zellin in x-Richtung
	int jMax;          //Anzahl der inneren Zellen in y-Richtung

	RealType tEnd;		//Endzeit
	RealType deltaT;
	RealType tau;   //Sicherheitsfaktor
	RealType deltaVec;   //Zeitabstand fï¿½r die Ausgabe

	IndexType iterMax;     // Maximale Anzahl an Iterationen
	RealType eps;	//Toleranz fï¿½r Druckiteration
	RealType omg;	//Relaxationsfaktor
	RealType alpha;	//Upwind-Differencing-Faktor
	RealType RE;			//Reynoldszahl
	RealType GX;			//ï¿½uï¿½ere Krï¿½fte g_x und g_y
	RealType GY;
	RealType VI;			//Initailwerte fï¿½r u,v undp
	RealType UI;
	RealType PI;

	RealType Pr; //Prandtl-Zahl
	RealType beta; //Volumenexpansionskoeffizient
	RealType TI; //initiale Temperatur

	//Temperatur-Randbedingung
	RealType *boun=new RealType[4];
	//Dirichlet
	RealType TO;
	RealType TU;
	RealType TL;
	RealType TR;
	//Neumann
	RealType QO;
	RealType QU;
	RealType QL;
	RealType QR;

	//AB6: Geometrie: 1:Haft; 2: Rutsch; 3: Ausström; 4: Einstroem (Uflow; Vflow)
	RealType WL;
	RealType WR;
	RealType WO;
	RealType WU;
	RealType Uflow;
	RealType Vflow;

};


#endif /* SIMPARAM_H_ */
