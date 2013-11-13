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

	int tEnd;		//Endzeit
	RealType tau;   //Sicherheitsfaktor
	int deltaVec;   //Zeitabstand f�r die Ausgabe

	int iterMax;     // Maximale Anzahl an Iterationen
	RealType eps;	//Toleranz f�r Druckiteration
	RealType omg;	//Relaxationsfaktor
	RealType alpha;	//Upwind-Differencing-Faktor
	int RE;			//�u�ere Kr�fte g_x und g_y
	int GX;
	int GY;
	int UI;			//Initailwerte f�r u,v undp
	int UV;
	int PI;
};


#endif /* SIMPARAM_H_ */
