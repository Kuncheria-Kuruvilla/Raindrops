/*
 *  gameScene.h
 *  polygon_people
 *
 *  Created by Brian Eschrich on 10.08.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _GAMESCENE
#define _GAMESCENE

//monitor resolution
#define DEBUG_WINDOW_WIDTH 1280
#define DEBUG_WINDOW_HEIGHT 1024



#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"
#include "ofxContourAnalysis.h"
#include "ofxTriangle.h"
#include "ColorSchemes.h"
#include "WaterParticles.h"
#include "Plant.h"
#include "costumSoftBody.h"
#include "PolygonMan.h"


#include "Drop.h"

#include "RaindropLine.h"
#include "Background.h"
#include "LevelLoader.h"

#include "contactListener.h"
 struct b2ContactPoint  
{  
	b2Body* shape1;		///< the first shape  
	b2Body* shape2;		///< the second shape  
	b2Vec2 position;		///< position in world coordinates  
	b2Vec2 velocity;		///< velocity of point on body2 relative to point on body1 (pre-solver)  
	b2Vec2 normal;			///< points from shape1 to shape2  
	float32 separation;		///< the separation is negative when shapes are touching  
	float32 friction;		///< the combined friction coefficient  
	float32 restitution;	///< the combined restitution coefficient  
	b2ContactID id;			///< the contact id identifies the features in contact  
}; 

class MyContactListener : public b2ContactListener{
public:
	void Add(const b2ContactPoint* point);
	void Remove(const b2ContactPoint* point);
};

class gameScene : public ofBaseApp{
public:


	bool isDebugMode;
	bool isFullScreen;
	bool first_level;
	bool polygonmanDeleted;

	void setup();
	 void update();
	 void draw();
	void loadLevel(bool success);
	void cleanLevel();
	void updateLevel();

	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	void box2dContactEventHandler ( const b2ContactPoint* point );

	//gamestats
	int width;
	int height;
	int dropsDown;
	int animation_time;
	int animation_time_max;
	float maxRecVolume;
	float minVolStartWind;
	//box2dz
	ofxBox2d						box2d;			  //	the box2d world
	vector		<costumSoftBody>*	rainDrops;
	vector		<Plant>             plants;
	MyContactListener               contact;
	RaindropLine                    line;
	vector<WaterParticles>          waterParticles;

	ofxBox2dRect                    rect_bottom;
	ofxBox2dRect                    rect_top;
	ofxBox2dRect                    rect_left;
	ofxBox2dRect                    rect_right;

	//tracking
	PolygonMan                      polygonMan;

	Background                      background;

	LevelLoader                     ll;

	//soundinput
	void audioReceived 	(float * input, int bufferSize, int nChannels);

	float * leftBuffer;
	float * rightBuffer;
	int 	bufferCounter;
	int 	drawCounter;
};






#endif
