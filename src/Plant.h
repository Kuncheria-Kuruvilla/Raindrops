/*
 *  Plant.h
 *  polygon_people
 *
 *  Created by Brian Eschrich on 05.08.10.
 *  Copyright 2010 All rights reserved.
 *
 */
#ifndef _PLANT_H
#define _PLANT_H

//#pragma once
#include "ofxBox2d.h"
#include "ofxVectorMath.h"
#include "ColorSchemes.h"


class Flower : public ofxBox2dCircle {
public:
	bool isflour;
	
	int nCircles;
	int cScheme;
	int nLeafes;
	int rLeafes;
	int leafColor[3];
	int leafColor2[3];
	int growedLeafes;
	ofSoundPlayer           mySound;
	
	//flouring attributes
	bool isflouring;
	float actposani; //0-1
	float growtime;
	float tSysInit;
	
	void draw();
	void setInits(int numberOfCircles,
				  int colorScheme,
				  int numberLeafes,
				  int leafRadius);

 };

 class Plant : public ofxBox2dBaseShape {

public:	
	void setup(b2World * world, int screenWidth ,int pos_x, int pos_y, int numberOfBranches=3,float maxHeight=400, float maxWidth= 300, int colorSchemeFlower = 1, int colorSchemeBranch = 0);
	void draw();
	void calculateMiddlePoint(int pos);
	void updateBranches();
	void getFlowers(vector<ofxBox2dCircle*> &ofxBox2dCircle);
	vector<Flower> getFlowers2();
	vector <Flower> flowers;
	
	int cSchemeBranch, cSchemeFlower;
	vector <ofxBox2dCircle> ballJoints;
	vector <ofxBox2dJoint> joints;
	int nBranches;
	
private:
	
	ofxVec2f position;
	float maxH, maxW;
	//branch attributes
	
	vector <ofxVec2f> branchesEndpoint;	
	vector <ofxVec2f> branchesMiddlepoint;
	vector <ofxVec2f> offset;	//sets the maximal offset
	vector <ofxVec3f> offsetMultiplier; //random setted multiplier for linestroke
	vector <float> influences;
	vector <float> radiusCircles;
	vector <float> angles;
	
	
	
};

#endif