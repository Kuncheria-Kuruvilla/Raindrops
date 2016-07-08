

//#include "ofMain.h"
//#include "testApp.h"
#include "gameScene.h"
#include "ofAppGlutWindow.h"





//========================================================================
int main( ){

	//init window for app and debug (just 1 big window stretched on both monitors
	int height = DEBUG_WINDOW_HEIGHT;


	if (1024 > DEBUG_WINDOW_HEIGHT);
		height = 1024;
		cout << "höhe  " <<height;
	int width = 1400 + DEBUG_WINDOW_WIDTH;

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, width,height, OF_WINDOW);			// <-------- setup the GL context

	//testApp* app = new testApp;
	gameScene* app = new gameScene;

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( app);

};
