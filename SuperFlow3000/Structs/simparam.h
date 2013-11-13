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
	RealType xLength;  //Gebietslänge in x-Richtung
	RealType yLength;  //Gebietslänge in y.Richtung
	int iMax;          //Anzahl der inneren Zellin in x-Richtung
	int jMax;          //Anzahl der inneren Zellen in y-Richtung

	int tEnd;		//Endzeit
	RealType tau;   //Sicherheitsfaktor
	int deltaVec;   //Zeitabstand für die Ausgabe

	int iterMax;     // Maximale Anzahl an Iterationen
	RealType eps;	//Toleranz für Druckiteration
	RealType omg;	//Relaxationsfaktor
	RealType alpha;	//Upwind-Differencing-Faktor
	int RE;			//Äußere Kräfte g_x und g_y
	int GX;
	int GY;
	int UI;			//Initailwerte für u,v undp
	int UV;
	int PI;
};


#endif /* SIMPARAM_H_ */
