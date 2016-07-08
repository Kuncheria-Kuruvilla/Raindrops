/*
 *  ColorSchemes.h
 *  polygon_people
 *
 *  Created by Brian Eschrich on 06.08.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _COLORSCHEMES_H
#define _COLORSCHEMES_H


	const int nFlowerColors = 6*12; //size of schemes
	const int flowerSchemes[nFlowerColors] =   //color values
	{
		/** brown **/
		98,50,1,
		150,99,44,
		203,149,51,
		255,205,0,
		/** green **/
		44,55,46,
		70,95,63,
		0,157,87,
		189,223,179,
		/** red **/
        130,13,13,
		191,19,4,
		217,72,17,
		242,129,29,
		/** blue **/
		41,41,65,
		0,81,127,
		0,134,204,
		109,166,225,
        /** grey **/
		48,48,47,
		90,89,92,
		120,114,103,
		203,227,206,
        /** colored **/
		255,78,33,
		39,42,43,
		232,12,122,
		33,214,189
	};

	inline void cSgetColor(int nScheme, int nColor, int* arrayToSave){
 		arrayToSave[0] = flowerSchemes[(nScheme*12)+(nColor*3)];
		arrayToSave[1] = flowerSchemes[(nScheme*12)+(nColor*3)+1];
		arrayToSave[2] = flowerSchemes[(nScheme*12)+(nColor*3)+2];

	};

#endif
