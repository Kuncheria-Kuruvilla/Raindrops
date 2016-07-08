// #include "testApp.h"


// //--------------------------------------------------------------
// void testApp::setup(){

//     const char* xml_dir = "data/level.xml";
//     ll.load(xml_dir);

// 	isDebugMode = true;
// 	isFullScreen = false;


// 	imgWidth=320;
// 	imgHeight=240;
// 	workImage=new unsigned char[imgWidth*imgHeight*3];


// 	bReversePoints	 = true;
// 	bDrawGray		 = false;
// 	simpleAmount     = 1.0;


// #ifdef _USE_LIVE_VIDEO
// 	vidGrabber.setVerbose(true);
// 	vidGrabber.initGrabber(320,240);
// #else
// 	vidPlayer.loadMovie("toll1.mov");
// 	vidPlayer.play();
// #endif

//     colorImg.allocate(320,240);
// 	grayImage.allocate(320,240);
// 	grayBg.allocate(320,240);
// 	grayDiff.allocate(320,240);

// 	bLearnBakground = true;
// 	threshold = 24;
// 	contourAnalysis.setSize(320, 240);


// 	//init physics
// 	box2d.init();
// 	box2d.setGravity(0, 10);
// 	//box2d.createBounds(-10, -10, APP_WINDOW_WIDTH + 20, APP_WINDOW_HEIGHT + 20);
// 	box2d.createFloor();
// 	box2d.checkBounds(true);
// 	box2d.setFPS(30.0);

// 	sb.setWorld(box2d.getWorld());
// 	sb.setPhysics(3, 0.1, 0.1);
// 	sb.setup(box2d.getWorld(), 400, 900);
// 	sb.color.r = 128;
// 	sb.color.g = 128;
// 	sb.color.b = 128;
// 	plant.setup(box2d.getWorld(), ofRandom(APP_WINDOW_WIDTH/3,APP_WINDOW_WIDTH*2/3), APP_WINDOW_HEIGHT, 5, APP_WINDOW_HEIGHT, 100, 1, 1);
// 	vector<int> colorsets;
// 	colorsets.push_back(1);
// 	colorsets.push_back(2);
// 	colorsets.push_back(3);
// 	colorsets.push_back(4);
// 	colorsets.push_back(5);
// 	line.setup(box2d.getWorld(), APP_WINDOW_WIDTH, colorsets);
// 	movingObject.setup(box2d.getWorld(),APP_WINDOW_WIDTH/2,APP_WINDOW_HEIGHT/2, 30,30);
// }

// //--------------------------------------------------------------
// void testApp::update(){
// 	ofBackground(255,255,255);

//     bool bNewFrame = false;

// 	//update physics
// 	box2d.update();
// 	plant.updateBranches();

//     if(objectMoves) {
// 		float strength = 13.0f;
// 		float damping  = 0.8f;
// 		for(int i=0; i<line.vertex_count; i++) {
// 			line.vertices[i].circle.addAttractionPoint(mouseX, mouseY, strength);
// 			line.vertices[i].circle.addDamping(damping, damping);
// 		}
// 	}

// 	for(int i = 0; i<line.softdrops.size();i++) {
// 	    if(line.softdrops[i].ring[0]->getPosition().x>0)
//         if( (line.softdrops[i].ring[0]->getPosition().y>APP_WINDOW_WIDTH) || (line.softdrops[i].ring[0]->getPosition().y>APP_WINDOW_HEIGHT) ) {
//                 mySound.loadSound("sound/cell_death.wav");
//                 mySound.play();
//                 line.softdrops[i].dead = true;
//                 line.softdrops[i].destroyShape();
//                 line.softdrops.erase(line.softdrops.begin()+i);
//                 cout << "cell death" << endl;
//            }
// 	}

// 	ofSoundUpdate();

// #ifdef _USE_LIVE_VIDEO
// 	vidGrabber.grabFrame();
// 	bNewFrame = vidGrabber.isFrameNew();
// #else
// 	vidPlayer.idleMovie();
// 	bNewFrame = vidPlayer.isFrameNew();
// #endif

// 	if (bNewFrame){

// #ifdef _USE_LIVE_VIDEO
// 		colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
// #else
// 		colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
// #endif

//         grayImage = colorImg;
// 		if (bLearnBakground == true){
// 			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
// 			bLearnBakground = false;
// 		}

// 		// take the abs value of the difference between background and incoming and then threshold:
// 		grayDiff.absDiff(grayBg, grayImage);
// 		grayDiff.blur(3);
// 		grayDiff.threshold(threshold);
// 		//grayDiff.invert();


// 		//grayImage.threshold(threshold, true);

// 		contour.findContours(grayDiff, 30, 320*240, 20, true);

// 		triangle.clear();
// 		int i;

// 		for (i=polys.size()-1; i>=0; i--) {
// 			box2d.world->DestroyBody(polys[i].body);
// 		}

// 		polys.clear();
// 		// for testing lets just grab the largest blob
// 		if(contour.nBlobs > 0) {

// 			contourAnalysis.simplify(contour.blobs[0].pts, simpleCountour, simpleAmount);

// 			triangle.triangulate(contour.blobs[0], max(3.0f, (float)contour.blobs[0].pts.size()/5));



// 			ofxTriangleData* tData;
//             for (i=triangle.triangles.size()-1; i>=0; i--) {

// 				tData = &triangle.triangles[i];

// 				ofxBox2dPolygon poly;

// 				poly.addVertex((tData->a.x)*3.2, (tData->a.y)*3.2);
// 				poly.addVertex((tData->b.x)*3.2, (tData->b.y)*3.2);
// 				poly.addVertex((tData->c.x)*3.2, (tData->c.y)*3.2);

// 				if(poly.validateShape()) {

// 					poly.init(box2d.world, 0.0f, 0.0f);
// 					polys.push_back(poly);
//                 }
//             }


// 			ofPoint p;
// 			lineStrip.setWorld(box2d.getWorld());
// 			lineStrip.clear();

// 			// we need to loop backward to make the top
// 			// of the contour the active side else go the
// 			// other way.
// 			if(bReversePoints) {
// 				for(int i=simpleCountour.size()-1; i>0; i--) {
// 					p.x = simpleCountour[i].x;
// 					p.y = simpleCountour[i].y;
// 					lineStrip.addPoint(p.x*3.2, p.y*3.2);
// 				}
// 			}
// 			else {
// 				for(int i=0; i<simpleCountour.size(); i++) {
// 					p.x = simpleCountour[i].x;
// 					p.y = simpleCountour[i].y;
// 					lineStrip.addPoint(p.x*3.2, p.y*3.2);
// 				}
// 			}

// 			// good to go :)
// 			lineStrip.createShape();
// 		}

// 	}

// }

// //--------------------------------------------------------------
// void testApp::draw(){

// 	if (isDebugMode)
// 	{
// 		// draw the incoming, the grayscale, the bg and the thresholded difference
// 		ofSetColor(0xffffff);
// 		colorImg.draw(APP_WINDOW_WIDTH + 20,20);
// 		grayImage.draw(APP_WINDOW_WIDTH + 360,20);
// 		grayBg.draw(APP_WINDOW_WIDTH + 20,280);
// 		grayDiff.draw(APP_WINDOW_WIDTH + 360,280);


// 		ofNoFill();
// 		//ofSetColor(255, 0, 0);
// 		contourAnalysis.draw(simpleCountour, APP_WINDOW_WIDTH + 360,280, vw, vh);

//         //triangle.draw(APP_WINDOW_WIDTH + 360, 0);
// 		//textoutput

// 		string info = "";
// 		info += "number of blobs: " + ofToString(contourFinder.nBlobs,1) + "\n";
// 		info += "lineele: " + ofToString(contour.nBlobs, 1) + "\n";
// 		info += "threshold: " + ofToString(threshold, 1) + "\n";
// 		info += "fps: " + ofToString(ofGetFrameRate(), 1);
// 		if (contourFinder.nBlobs >0)
// 			info += "posx " + ofToString(contourFinder.blobs[0].pts[0].x) + "  posy " +  ofToString(contourFinder.blobs[0].pts[0].y);
// 		ofSetColor(0, 255, 255);
// 		ofDrawBitmapString(info,APP_WINDOW_WIDTH + 30, 800);

//         int i;

//      	for(i=polys.size()-1; i>=0; i--) {
// 			polys[i].draw();
//         }

// 		// finally, a report:

// 		ofSetColor(0xffffff);
// 		char reportStr[1024];
// 		sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/-)\nnum blobs found %i, fps: %f", threshold, contourFinder.nBlobs, ofGetFrameRate());
// 		ofDrawBitmapString(reportStr,APP_WINDOW_WIDTH + 20, 600);
// 	}


// 	sb.draw();

// 	//draw physics
// 	for(int i=0; i<customParticles.size(); i++) {
// 		customParticles[i].draw();
// 	}
// 	box2d.draw();
// 	lineStrip.draw();


// 	plant.draw();
// 	line.draw();
// 	movingObject.draw();



// }


// //--------------------------------------------------------------
// void testApp::keyPressed  (int key){

// 	switch (key){
// 		case ' ':
// 			bLearnBakground = true;
// 			break;
// 		case '+':
// 			threshold ++;
// 			if (threshold > 255) threshold = 255;
// 			break;
// 		case '-':
// 			threshold --;
// 			if (threshold < 0) threshold = 0;
// 			break;
// 		case 'd':
// 			isDebugMode = !isDebugMode;
// 			break;
// 		case 'f':
// 			isFullScreen = !isFullScreen;

// 			if(!isFullScreen){
// 				ofSetWindowShape(800,600);
// 				ofSetFullscreen(false);
// 				// figure out how to put the window in the center:
// 				int screenW = ofGetScreenWidth();
// 				int screenH = ofGetScreenHeight();
// 				ofSetWindowPosition(screenW/2-800/2, screenH/2-600/2);
// 			} else if(isFullScreen == 1){
// 				ofSetFullscreen(true);
// 			}
// 			break;
// 		case 'z':

// 			float r = 30;		// a random radius 4px - 20px
// 			costumSoftBody p;
// 			p.setup(box2d.getWorld(), mouseX, mouseY, r,true);
// 			p.color.r = ofRandom(20, 100);
// 			p.color.g = 0;
// 			p.color.b = ofRandom(150, 255);
// 			customParticles.push_back(p);
// 			break;

// 	}
// }

// void testApp::mouseDragged(int x, int y, int button) {
// 	if(objectMoves)
//         movingObject.setPosition(x,y);
// }

// //--------------------------------------------------------------
// void testApp::mousePressed(int x, int y, int button) {

// 	if(x>movingObject.getPosition().x-movingObject.circle.radius && x<movingObject.getPosition().x+movingObject.circle.radius) {
// 	    if(y>movingObject.getPosition().y-movingObject.circle.radius && y<movingObject.getPosition().y+movingObject.circle.radius) {
//             objectMoves = true;
// 	    }
// 	}

// }

// void testApp::mouseMoved(int x, int y) {
// }

// //--------------------------------------------------------------
// void testApp::mouseReleased(int x, int y, int button){
// 	objectMoves = true;
// }
// //--------------------------------------------------------------
// void testApp::windowResized(int w, int h){

// }






