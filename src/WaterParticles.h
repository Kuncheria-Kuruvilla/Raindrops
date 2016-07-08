/*
 *  waterParticles.h
 *  polygon_people
 *
 *  Created by Brian Eschrich on 12.08.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _WATERPARTICLES_H
#define _WATERPARTICLES_H
#pragma once
#include "ofxBox2d.h"

class WaterParticles : public ofxBox2dBaseShape {
public:
	vector<ofxBox2dCircle> particles;
	vector<float> lifetime;
	ofxVec3f pColor;
	float tLastStep;
	b2World* w;
	int pX,pY;
	bool isCreating;
	int number;
	bool isCreated;
	int mywidth;
	int myheight;

	void createParticles (b2World * world, int nPart, int posX, int posY, ofxVec3f color, int width = 1024, int height=768)
	{
		isCreating = true;
		w=world;
		pX=posX;
		pY=posY;
		number = nPart + ofRandom(nPart*(-0.8), nPart*0.2);
		pColor = color;
		tLastStep = ofGetSystemTime();
		isCreated = false;
		mywidth = width;
		myheight = height;
	}

	void doParticles(){
		for (int i=0; i<number; ++i) {
			ofxBox2dCircle c;
			c.setPhysics(2, 0.3, 0.3);
			c.setup(w,pX + ofRandom(-5, 5), pY+ ofRandom(-5, 5), 4 + ofRandom(-3, 2));
			ofxVec2f f;
			f.x = ofRandom(-50, 50);
			f.y = ofRandom(20, 70);
			ofxVec2f p;
			p.x = pX;
			p.y = pY;
			//c.addForce(f,p); 	
			c.addAttractionPoint(f);
			c.addAttractionPoint(p);
			float lt = ofRandom(500, 1500);
			particles.push_back(c);
			lifetime.push_back(lt);
			//cout <<"pos c " << c.getPosition().x <<endl;
			//cout <<"pos " << particles[0].getPosition().x <<endl;
		}
	}



	void draw()
	{

		//--yes, creation in drawing method, thanks to box2d useless random bug
		if (isCreating)
		{

		    //cout << "WaterParticles::draw::createparticles" << endl;

			doParticles();
			isCreating = false;
			isCreated = true;

		}

		//cout << "WaterParticles::draw::clean_up" << endl;

        for (int i=0; i< particles.size(); ++i)
		{
			lifetime[i] -= (ofGetSystemTime() - tLastStep);
			if (lifetime[i]<=0)
			{
				particles.erase(particles.begin() + i);
				lifetime.erase(lifetime.begin() + i);
				//--i;
			}

		}

		//cout << "WaterParticles::draw::particles" << endl;

		ofFill();
		ofSetColor(pColor.x,pColor.y,pColor.z);

		for (int i=0; i< particles.size(); ++i)
		{
		    //cout << "WaterParticles::draw::particle" << i << endl;

		    if( particles[i].body != NULL &&
			   particles[i].getPosition().x < mywidth && particles[i].getPosition().x > 5 &&
			   particles[i].getPosition().y < myheight && particles[i].getPosition().y > 5) {
                ofCircle(particles[i].getPosition().x, particles[i].getPosition().y, particles[i].getRadius());

		    }

		}
		tLastStep = ofGetSystemTime();

		//cout << "WaterParticles::draw::end" << endl;
	}
};
#endif
