 /*
//  *  costumSoftBody.h
//  *  polygon_people
//  *
//  *  Created by Brian Eschrich on 04.08.10.
//  *  Copyright 2010 __MyCompanyName__. All rights reserved.
//  *
//  */

 #ifndef _COSTUMSOFTBODY_H
 #define _COSTUMSOFTBODY_H

#include "ofxBox2d.h"
#include "ofxBox2dSoftBody.h"
#include "contactListener.h"

typedef struct {

 	float 	x;
  	float 	y;

	}	vertexArray;



// ------------------------------------------------- a simple extended box2d softBody
class costumSoftBody : public ofxBox2dSoftBody {

public:
	ofColor color;
	int nCurveVertexes;
	vertexArray *curveVertices;
	int cSchemeDrop;

	bool destroyMe;
	bool useTexture;

	void getRingList(vector <ofxBox2dCircle*> &ringList)
	{
		ringList = ring;
	}
	/*
	void getShapes(vector<b2Shape*>& shapesToSave){
		shapesToSave.clear();
		for (int i=0; i<nCurveVertexes; ++i) {
			shapesToSave.push_back(ring[i]->body->GetShapeList());
		}

	};*/
	//inline b2Shape* b2Body::GetShapeList()

	void setup(b2World * world, float px, float py, float ringRadius=100, bool showGrid = true, int colorSchemeDrop = 0)
	{
		ofxBox2dSoftBody::setup(world,  px,  py);
		useTexture = showGrid;
		destroyMe = false;
		nCurveVertexes = 2*ringRadius/5/1.5;

		cSchemeDrop = colorSchemeDrop;
	};



	/*Lotto =
	delete[] Lotto;      // Freigabe des Speichers
	Lotto = 0;           // Zeiger sichern
	*/

	void draw() {
		ofxVec2f va,vb;

		//cout << "ringsize " <<ring.size() <<endl;
		for (int i=0; i<nCurveVertexes; ++i) {
			//cout << "pos " <<i <<endl;
				if (i==0)
				{
					va.x = ring[i]->getPosition().x;
					va.y = ring[i]->getPosition().y;
				}
				else {
					va = vb;
				}
				vb.x = ring[i+1]->getPosition().x;
				vb.y = ring[i+1]->getPosition().y;

				if (va.distance(vb) > 30) {
					destroyMe = true;
				}
		}

		/*
		 for (int i=0; i<nCurveVertexes; ++i)
		 {
		 //b2Vec2 p1 = m_joints[i]->GetAnchor1();
		 //p1 *= OFX_BOX2D_SCALE;
		 float p1 = ring[i]->getPosition().x;
		 float p2 = ring[i]->getPosition().y;
		 curveVertices[i].x = ring[i]->getPosition().x;
		 curveVertices[i].y = ring[i]->getPosition().y;
		 }*/




		string info = "";
		info += "px py: " + ofToString(ring[4]->getPosition().x,1) + "\n"  + ofToString(nCurveVertexes,1);
		ofSetColor(0, 255, 255);
		ofDrawBitmapString(info,30, 800);


			ofSetColor(color.r, color.g, color.b);
			ofFill();
			ofBeginShape();

			for (int i = 0; i < nCurveVertexes; i++){
				// sorry about all the if/states here, but to do catmull rom curves
				// we need to duplicate the start and end points so the curve acutally
				// goes through them.

				// for i == 0, we just call the vertex twice
				// for i == nCurveVertexes-1 (last point) we call vertex 0 twice
				// otherwise just normal ofCurveVertex call

				if (i == 0){
					ofCurveVertex(ring[0]->getPosition().x, ring[0]->getPosition().y); // we need to duplicate 0 for the curve to start at point 0
					ofCurveVertex(ring[0]->getPosition().x, ring[0]->getPosition().y); // we need to duplicate 0 for the curve to start at point 0
				} else if (i == nCurveVertexes-1){
					ofCurveVertex(ring[i]->getPosition().x, ring[i]->getPosition().y);
					ofCurveVertex(ring[0]->getPosition().x, ring[0]->getPosition().y);	// to draw a curve from pt 6 to pt 0
					ofCurveVertex(ring[0]->getPosition().x, ring[0]->getPosition().y);	// we duplicate the first point twice
				} else {
					ofCurveVertex(ring[i]->getPosition().x, ring[i]->getPosition().y);
				}
			}

			ofEndShape();

	if (useTexture)
	{
			ofEnableAlphaBlending();
			ofSetColor(0, 233, 255, 100);
			for(int i=0; i<ring.size(); i++) {
				ring[i]->draw();
			}

			ofSetColor(20, 233, 255, 100);

			for(int i=0; i<m_joints.size(); i++) {
				b2Vec2 p1 = m_joints[i]->GetAnchorA();
				b2Vec2 p2 = m_joints[i]->GetAnchorB();
				p1 *= OFX_BOX2D_SCALE;
				p2 *= OFX_BOX2D_SCALE;
				ofLine(p1.x, p1.y, p2.x, p2.y);
			}

			for(int i=0; i<center_joints.size(); i++) {
				b2Vec2 p1 = center_joints[i]->GetAnchorA();
				b2Vec2 p2 = center_joints[i]->GetAnchorA();
				p1 *= OFX_BOX2D_SCALE;
				p2 *= OFX_BOX2D_SCALE;
				ofLine(p1.x, p1.y, p2.x, p2.y);
			}
			ofDisableAlphaBlending();

			center.draw();
		}

	}
 };

 #endif
