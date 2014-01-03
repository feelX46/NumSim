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
	RealType xLength;  //Gebietsl�nge in x-Richtung
	RealType yLength;  //Gebietsl�nge in y.Richtung
	int iMax;          //Anzahl der inneren Zellin in x-Richtung
	int jMax;          //Anzahl der inneren Zellen in y-Richtung

	RealType tEnd;		//Endzeit
	RealType deltaT;
	RealType tau;   //Sicherheitsfaktor
	RealType deltaVec;   //Zeitabstand f�r die Ausgabe

	IndexType iterMax;     // Maximale Anzahl an Iterationen
	RealType eps;	//Toleranz f�r Druckiteration
	RealType omg;	//Relaxationsfaktor
	RealType alpha;	//Upwind-Differencing-Faktor
	RealType RE;			//Reynoldszahl
	RealType GX;			//�u�ere Kr�fte g_x und g_y
	RealType GY;
	RealType VI;			//Initailwerte f�r u,v undp
	RealType UI;
	RealType PI;

	RealType Pr; //Prandtl-Zahl
	RealType beta; //Volumenexpansionskoeffizient
	RealType TI; //initiale Temperatur
	RealType TO; //Temperatur-Randbedingung
	RealType TU; //Temperatur-Randbedingung
	RealType TL; //Temperatur-Randbedingung
	RealType TR; //Temperatur-Randbedingung

};


#endif /* SIMPARAM_H_ */
