// #ifndef _TEST_APP
// #define _TEST_APP
// #pragma once

// #include "ofMain.h"
// #include "ofxBox2d.h"
// #include "ofxOpenCv.h"
// #include "ofxVectorMath.h"
// #include "ofxContourAnalysis.h"
// #include "costumSoftBody.h"
// #include "ofxTriangle.h"
// #include "ColorSchemes.h"


// #define _USE_LIVE_VIDEO		// uncomment this to use a live camera
// // otherwise, we'll use a movie file

// //monitor resulution
// #define APP_WINDOW_WIDTH 1024
// #define APP_WINDOW_HEIGHT 500
// #define DEBUG_WINDOW_WIDTH 1400
// #define DEBUG_WINDOW_HEIGHT 500

// #include "Plant.h"
// #include "RaindropLine.h"
// #include "LevelLoader.h"

// // ------------------------------------------------- a simple extended box2d circle
// class CustomParticle : public ofxBox2dCircle {

// public:
// 	CustomParticle() {
// 	}
// 	ofColor color;
// 	void draw() {
// 		float radius = getRadius();

// 		glPushMatrix();
// 		glTranslatef(getPosition().x, getPosition().y, 0);

// 		ofSetColor(color.r, color.g, color.b);
// 		ofFill();
// 		ofCircle(0, 0, radius);

// 		glPopMatrix();

// 	}
// };

// class testApp : public ofBaseApp{

// public:


// 	bool isDebugMode;
// 	bool isFullScreen;

// 	void setup();
// 	void update();
// 	void draw();

// 	void keyPressed  (int key);
// 	void mouseMoved(int x, int y );
// 	void mouseDragged(int x, int y, int button);
// 	void mousePressed(int x, int y, int button);
// 	void mouseReleased(int x, int y, int button);
// 	void windowResized(int w, int h);

// #ifdef _USE_LIVE_VIDEO
// 	ofVideoGrabber 		vidGrabber;
// #else
// 	ofVideoPlayer 		vidPlayer;
// #endif

//     ofSoundPlayer       mySound;

// 	unsigned char * workImage;
// 	ofxCvColorImage		colorImg;
// 	ofxCvColorImage		colorNew;

// 	ofxCvGrayscaleImage 	grayImage;
// 	ofxCvGrayscaleImage 	grayBg;
// 	ofxCvGrayscaleImage 	grayDiff;

// 	ofxCvContourFinder 	contourFinder;

// 	int 				threshold;
// 	bool				bLearnBakground;


// 	int imgWidth;
// 	int imgHeight;

// 	// OpenCV
// 	bool					bReversePoints;
// 	bool					bDrawGray;
// 	float					simpleAmount;
// 	int						vw, vh;

// 	// contour work
// 	ofxCvContourFinder		contour;
// 	ofxContourAnalysis		contourAnalysis;
// 	vector <ofPoint>		simpleCountour;

// 	//Triangle contour fill
// 	ofxTriangle              triangle;
//     vector <ofxBox2dPolygon> polys;

// 	//box2d
// 	ofxBox2d						box2d;			  //	the box2d world
// 	ofxBox2dLine				lineStrip;		  //	a linestrip for contours
// 	vector		<costumSoftBody>	customParticles;  //	this is a custom particle the extends a cirlce
// 	ofxBox2dCircle					innerBallJoint;
// 	ofxBox2dCircle					ballJoints[5];	  //	ball joints
// 	ofxBox2dCircle					outerBallJoints[5];
// 	ofxBox2dJoint					joints[5];			  //	box2d joints
// 	ofxBox2dJoint					outerJoints[5];
// 	ofxBox2dJoint					innerJoints[5];
// 	costumSoftBody					sb;

// 	Plant                           plant;
// 	RaindropLine                            line;
// 	LevelLoader                     ll;

// 	ofxBox2dCircle                  movingObject;
// 	bool                            objectMoves;

// };
// // #endif

// #endif
