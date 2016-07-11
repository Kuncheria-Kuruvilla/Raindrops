/*
 *  Plant.cpp
 *  polygon_people
 *
 *  Created by Brian Eschrich on 05.08.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Plant.h"


void Plant::setup(b2World * world , int screenWidth,int pos_x, int pos_y, int numberOfBranches, float maxHeight, float maxWidth, int colorSchemeFlower, int colorSchemeBranch) {
	cout<<"here--";
	nBranches = numberOfBranches;
	position.x = pos_x;
	position.y = pos_y;
	maxW = maxWidth;
	maxH = maxHeight;
	cSchemeBranch = colorSchemeBranch;
	cSchemeFlower = colorSchemeFlower;


	//create branches
	for (int i= 0; i<nBranches;++i)
	{
		//set endpoint
		ofxVec2f p;
		float xMin = position.x - (maxW/2);
		if (xMin < 20) xMin = 20;
		float xMax = position.x + (maxW/2);
		if (xMax > screenWidth-20) xMax = screenWidth -20;
		p.x = ofRandom(xMin,xMax);
		p.y = ofRandom(position.y - 80, position.y - maxH);
		branchesEndpoint.push_back(p);

		//set middlepoint
		ofxVec2f v0 = p-position;										//vector between root and leaf
		float influenceRoot = ofRandom(0.2, 0.8);						//sets sets distance from root of middle point
		influences.push_back (influenceRoot);

		ofxVec2f posCircle = position + (v0*influenceRoot);				//centerPoint of MiddleCreationCircle
		float radiusCircle =  ofRandom(20, v0.length()*influenceRoot);	//radius ccircle
		radiusCircles.push_back(radiusCircle);

		float angle = ofRandom(0, 2*PI);
		angles.push_back(angle);

		ofxVec2f middlePoint;											//point of creation
		middlePoint.x = posCircle.x + radiusCircle*cos(angle);
		middlePoint.y = posCircle.y + radiusCircle*sin(angle);
		branchesMiddlepoint.push_back(middlePoint);

		//set offset
		ofxVec2f o;
		o.x = -1*v0.y;
		o.y = v0.x;
		o.normalize();
		offset.push_back(o);

		//set offset multiplier
		ofxVec3f v;
		v.x = ofRandom(3,7);
		v.y = ofRandom(3,7);
		v.z = ofRandom(3,7);
		offsetMultiplier.push_back(v);

		//set Flower
		Flower c;
		c.setPhysics(3.0, 0.53, 0.1);
		c.setup(world, p.x, p.y, ofRandom(15, 40));
		int nColors = c.getRadius()/(5*ofRandom(1, 2));
		if (nColors > 4) {
			nColors = 4;
		}
		int nCircles = ofRandom(5, 10);
		float nRadius = ofRandom(c.getRadius()/4, c.getRadius()/2);

		c.setInits(nColors, cSchemeFlower, nCircles,nRadius);
		flowers.push_back(c);


		//add the joints
		ofxBox2dCircle jc;
		p.y=-10;
		jc.setup(world, (float)p.x,(float)-10.0, (float)3.0);
		ballJoints.push_back(jc);
		ofxBox2dJoint j;
		j.setWorld(world);
		j.setup(world,jc.body, c.body, 0.3, 0.1);
		joints.push_back(j);

	}
};


void Plant::draw(){

	int cTemp[3];
	for (int i=0; i<nBranches; ++i) {

		//draw branches
		float multC;
		if (nBranches <= 3)
			multC = 2;
		else if (nBranches <=8)
		{
			if (i < 3)
				multC = 1;
			else {
				multC = 2;
			}
		}
		else {
			if (i <= nBranches /3)
				multC = 0;
			else if (i <= nBranches/3*2)
				multC = 1;
			else {
				multC = 2;
			}
		}

		cSgetColor(cSchemeBranch, multC, cTemp);
		ofSetColor(cTemp[0],cTemp[1],cTemp[2]);

		ofFill();
		ofBeginShape();
			ofVertex(position.x, position.y);
			ofCurveVertex(position.x, position.y);
			ofCurveVertex(position.x+(offset[i].x*offsetMultiplier[i].x), position.y+(offset[i].y*offsetMultiplier[i].x));
			ofCurveVertex(branchesMiddlepoint[i].x+(offset[i].x*offsetMultiplier[i].y), branchesMiddlepoint[i].y+(offset[i].y*offsetMultiplier[i].y));
			ofCurveVertex(branchesEndpoint[i].x+(offset[i].x*offsetMultiplier[i].z), branchesEndpoint[i].y+(offset[i].y*offsetMultiplier[i].z));
			ofCurveVertex(branchesEndpoint[i].x+(offset[i].x*offsetMultiplier[i].z), branchesEndpoint[i].y+(offset[i].y*offsetMultiplier[i].z));
			ofCurveVertex(branchesEndpoint[i].x, branchesEndpoint[i].y);
			ofCurveVertex(branchesMiddlepoint[i].x, branchesMiddlepoint[i].y);
			ofCurveVertex(position.x, position.y);
			ofCurveVertex(position.x, position.y);
			ofVertex(position.x, position.y);
		ofEndShape();

		//draw flowers
		flowers[i].draw();

		//debug
		int cTemp2[3];
		cSgetColor(0, 1, cTemp2);

	}




};

void Plant::updateBranches()
{
	for (int i=0; i<nBranches; ++i) {
		branchesEndpoint[i].x = flowers[i].getPosition().x;
		branchesEndpoint[i].y = flowers[i].getPosition().y;
		calculateMiddlePoint(i);
	}
}

void Plant::calculateMiddlePoint(int pos){
	//set middlepoint
	ofxVec2f v0 = branchesEndpoint[pos]-position;						//vector between root and leaf
	ofxVec2f posCircle = position + (v0*influences[pos]);				//centerPoint of MiddleCreationCircle
	ofxVec2f middlePoint;												//point of creation
	middlePoint.x = posCircle.x + radiusCircles[pos]*cos(angles[pos]);
	middlePoint.y = posCircle.y + radiusCircles[pos]*sin(angles[pos]);
	branchesMiddlepoint[pos]=middlePoint;
	//branchesMiddlepoint[pos]=branchesEndpoint[pos]/2;
	/*
	//set offset
	ofxVec2f o;
	o.x = -1*v0.y;
	o.y = v0.x;
	o.normalize();
	offset[pos]=0;*/

}

void Plant::getFlowers(vector<ofxBox2dCircle*> &vFlower){
	vFlower.clear();
	for (int i=0; i<flowers.size(); ++i) {
		vFlower.push_back(&(flowers[i]));
	}
};

vector<Flower> Plant::getFlowers2(){
	return flowers;
}



void Flower::draw(){
	int cTemp[3];
	int offset = 4-nCircles;
	for (int j=0; j<nCircles; ++j) {
		int rC = getRadius() * (nCircles-j)/nCircles;
		cSgetColor(cScheme, j+offset, cTemp);
		ofSetColor(cTemp[0], cTemp[1], cTemp[2]);
		ofCircle(getPosition().x, getPosition().y, rC);
	}

	//draw leafes
	float distance = getRadius() + (1.2 * rLeafes);
	float distance2 = getRadius() + (1.1 * rLeafes);
	if (isflour)
	{
		for (int i=0; i<nLeafes; ++i) {
			float angle = 2*PI * i/nLeafes;
			int px = getPosition().x + distance * cos(angle);
			int py = getPosition().y + distance * sin(angle);
			ofSetColor(leafColor[0], leafColor[1], leafColor[2]);
			ofCircle(px, py, rLeafes);

			if (nCircles >=3)
			{
				ofSetColor(leafColor2[0], leafColor2[1], leafColor2[2]);
				ofCircle(px, py, rLeafes);
				px = getPosition().x + distance2 * cos(angle);
				py = getPosition().y + distance2 * sin(angle);
				ofSetColor(leafColor[0], leafColor[1], leafColor[2]);
				ofCircle(px, py, rLeafes/3*2);
			}
		}
	}

	//grow leafes
	if (isflouring){
		if (tSysInit == -1){
			tSysInit = ofGetSystemTime();
		}
		else {
			float tAct = ofGetSystemTime() - tSysInit;
			float tStep = growtime/nLeafes;
			int stepsAct = tAct/tStep;
			if (stepsAct > growedLeafes){
				growedLeafes =stepsAct;
				// mySound.loadSound("sound/maximize.wav");
				// mySound.play();
			}
			if (stepsAct < nLeafes) {
				for (int i=0; i<stepsAct; ++i) {
					float angle = 2*PI * i/nLeafes;
					int px = getPosition().x + (distance * cos(angle));
					int py = getPosition().y + (distance * sin(angle));
					ofSetColor(leafColor[0], leafColor[1], leafColor[2]);
					ofCircle(px, py, rLeafes);
					if (nCircles >=3)
					{
						ofSetColor(leafColor2[0], leafColor2[1], leafColor2[2]);
						ofCircle(px, py, rLeafes);
						px = getPosition().x + distance2 * cos(angle);
						py = getPosition().y + distance2 * sin(angle);
						ofSetColor(leafColor[0], leafColor[1], leafColor[2]);
						ofCircle(px, py, rLeafes/3*2);
					}
				}
			}
			else {
				isflour = true;
				isflouring = false;
			}

		}

	}

}

void Flower::setInits(int numberOfCircles,
					  int colorScheme,
					  int numberLeafes,
					  int leafRadius){

	nCircles = numberOfCircles;
	cScheme = colorScheme;
	nLeafes = numberLeafes;
	rLeafes = leafRadius;

	//mySound.setMultiPlay(true);
	int cS = 4 - (ofRandom(0, 2));
	cSgetColor(colorScheme, cS, leafColor);
	cSgetColor(colorScheme, cS-1, leafColor2);
	isflouring = false;
	tSysInit = -1;
	actposani = 0;
	growtime = 2000;
	growedLeafes = 0;
	isflour = false;
}



