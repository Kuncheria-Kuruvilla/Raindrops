 #ifndef _BACKGROUND_H
 #define _BACKGROUND_H

 #include "ofxBox2d.h"
 #include "ofxPerlin.h"

 class Background : public ofxBox2dBaseShape{


private:
    ofImage bg1;
	b2World* pWorld;
	ofxPerlin *noise;
	int *dropFallDown;

	//wind variables
	vector<ofxVec2f> windSource;
	vector<ofxVec2f> windForce;
	vector<int> aniPos;
	vector<int> aniEnd;
	vector<float>aniSpeed;

	ofColor bg_color;
	ofColor water_color;
	float lastButterflyCreated;
	ofColor colorButterfly;

	int mywidth;
	int myheight;

public:

    bool level_loading;


	void setup(b2World * world, int *dropsdown, int width, int height, ofColor color1, ofColor color2,ofColor colorWind, string img_src="") {
        pWorld = world;
		colorButterfly = colorWind;
		dropFallDown =dropsdown;
		lastButterflyCreated = 0;
		water_color = color1;
        bg_color = color2;
		if(img_src!=""){
			cout<<img_src.c_str()<<endl;
            bg1.loadImage(img_src);
        }
        else
            bg1.clear();

		noise = new ofxPerlin();

		level_loading = false;

		mywidth = width;
		myheight = height;
    }

	void draw() {
	    if(level_loading)
            drawLevelLoader();
        else {
            ofSetColor(bg_color.r,bg_color.g,bg_color.b);

            ofRect(0, 0, mywidth, myheight);
            ofSetColor(255, 255, 255);
			bg1.draw(0,0);
            doWaves();
			doWind();
        }
	}

	void doWaves(){
		//animated waves
		float div = 400;
		//float div = 200;
		ofEnableAlphaBlending();
		ofSetColor(water_color.r, water_color.g, water_color.b,75);
		ofFill();
		ofBeginShape();
		ofVertex(0, myheight);
		for(int i=0; i<mywidth; i+=2){
			float posY = noise->noise((float)i/div, (float)2, (float)ofGetFrameNum()/div);
			posY *= 100;
			posY += myheight-100 - ((*dropFallDown)*50);
			ofVertex(i, posY);
		}
		ofVertex(mywidth, myheight);
		ofEndShape();
	}

	void setupWind (b2Vec2 source, b2Vec2 force)
	{
		if (ofGetSystemTime() - lastButterflyCreated  > 300) {
			ofxVec2f s;
			s.x = source.x;
			s.y = source.y;
			ofxVec2f f;
			f.x = force.x;
			f.y = force.y;

			windSource.push_back(s);
			windForce.push_back(f);
			aniPos.push_back(0);
			aniEnd.push_back(ofRandom(20, 60));
			aniSpeed.push_back(ofRandom(3, 7));
			lastButterflyCreated = ofGetSystemTime();
		}
	}
	void doWind(){
		//cout << "paint" <<endl;;
		int l = 1000;
		int maxH = 100;

		for(int i=0; i<windSource.size(); ++i){



			ofxVec2f dir = windForce[i].normalized();
			float force = windForce[i].length();
			ofxVec2f h;
			h.x = -dir.y;
			h.y = dir.x;

			ofxVec2f windPos;
			windPos = windSource[i] + (dir*aniPos[i]*aniSpeed[i]);

			windPos += h *  sin(aniPos[i]) * force/10 ;

			ofEnableAlphaBlending();
			ofSetColor(colorButterfly.r, colorButterfly.g, colorButterfly.b, aniPos[i]*255/aniEnd[i]);
			if (aniPos[i]%2 == 0) {
				ofCircle(windPos.x+(h.x*10), windPos.y+(h.y*10), force/10);
			}
			else
				ofCircle(windPos.x+(h.x*-10), windPos.y+(h.y*-10), force/10);
			ofDisableAlphaBlending();

			if (aniPos[i] >= aniEnd[i])
			{
				windSource.erase(windSource.begin() + i);
				windForce.erase(windForce.begin() + i);
				aniPos.erase(aniPos.begin() + i);
				aniEnd.erase(aniEnd.begin() + i);
			}
			else
				++(aniPos[i]);
		}
	}

	void drawLevelLoader() {

        ofSetColor(0,0,0);
        ofRect(0, 0, mywidth, myheight);

	}

};
 #endif



