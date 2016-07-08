/*
 *  gameScene.cpp
 *  polygon_people
 *
 *  Created by Brian Eschrich on 10.08.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "gameScene.h"
bool new_level;


//--------------------------------------------------------------
void gameScene::setup(){

    cout << "gamescene::setup::start" << endl;

	isDebugMode = true;
	isFullScreen = false;
	first_level = true;
	new_level = false;

	//init gameVars
	dropsDown = 0;
	animation_time = 0;

	//init physics
	box2d.init();
	box2d.setGravity(0, 10);

	box2d.createGround();
	box2d.checkBounds(true);
	box2d.setFPS(30.0);

    box2d.getWorld() -> SetContactListener(&contact);

	// //sound-setup
	// ofSoundStreamSetup(0,2,this, 44100, 256, 4);
	// leftBuffer = new float[256];
	// rightBuffer = new float[256];
 //    maxRecVolume = 0;
	// minVolStartWind = .85;
	// bufferCounter = 0;
	// drawCounter = 0;

    loadLevel(true);
	rainDrops = &(line.softdrops);

	//cout << "gamescene::setup::end" << endl;

}

//--------------------------------------------------------------
void gameScene::update(){

    cout << "gamescene::update::start" << endl;

    float strength = 13.0f;
    float damping  = 0.8f;
    for(int i=0; i<line.vertex_count; i++) {
        line.vertices[i].circle.addAttractionPoint(polygonMan.highestPoint.X, polygonMan.highestPoint.Y, strength);
        //cmt--line.vertices[i].circle.addDamping(damping, damping);
    }

	ofBackground(255,255,255);

  	//update physics
	box2d.update();
	for (int i=0;i<plants.size();++i)
		plants[i].updateBranches();
	for (int i=0;i<waterParticles.size();++i)
	{
		if (waterParticles[i].particles.size() == 0 && waterParticles[i].isCreated)
		{
			waterParticles.erase(waterParticles.begin()+i);
			//cout << "size part " << waterParticles.size() <<endl;
			break;
		}
	}

	polygonMan.update(box2d.getWorld(),&polygonmanDeleted);

	//cout << "gamescene::update::end" << endl;
}

void gameScene::loadLevel(bool success) {

    cout << "gamescene::loadlevel::start" << endl;

    new_level = false;
    dropsDown = 0;
    animation_time = 0;
    animation_time_max = 150;

    polygonmanDeleted = true;

    if(first_level) {
        first_level = false;
        cout << "firstlevel";
        ll.setup(box2d.getWorld(), &width, &height);
    }

    //clean up
    b2World *world = box2d.getWorld();
    b2Body* node = world->GetBodyList();
    while (node) {
        b2Body* b = node;
        node = node->GetNext();
        world->DestroyBody(b);
    }
	if (isFullScreen){}
		else {
			int complete_width = width + DEBUG_WINDOW_WIDTH;
    ofSetWindowShape(complete_width,max(DEBUG_WINDOW_HEIGHT,height));
    ofSetFullscreen(false);
    // figure out how to put the window in the center:
    int screenW = ofGetScreenWidth();
    int screenH = ofGetScreenHeight();
    ofSetWindowPosition(width/2, height/2);
		}



    cout << "gamescene::loadlevel::nextlevel" << endl;

    ll.nextLevel(&plants, &line, &background, &dropsDown, success);

    cout << "gamescene::loadlevel::setup_rectangles" << endl;


	rect_bottom.setPhysics(0,0,0);
	rect_top.setPhysics(0,0,0);
	rect_left.setPhysics(0,0,0);
	rect_right.setPhysics(0,0,0);
	int weight = 100;
	rect_bottom.setup(box2d.getWorld(),-weight,height,width+weight*2, weight);
	rect_top.setup(box2d.getWorld(),-weight,-weight,width+weight*2, weight);
	rect_left.setup(box2d.getWorld(),-weight,0,weight, height);
	rect_right.setup(box2d.getWorld(),width,0,weight, height);


	cout << "gamescene::loadlevel::setup_polygonman" << endl;

	/*
	int temp_threshold;
	ofxCvColorImage		temp_colorBG;
	if(!first_level){
		temp_threshold = polygonMan.threshold;
		temp_colorBG = polygonMan.colorBG;
	}
	 */

    polygonMan.setup(width, height);
	/*
	if(!first_level) {
		polygonMan.threshold = temp_threshold;
		polygonMan.colorBG = temp_colorBG;
	}
	*/

    cout << "gamescene::loadlevel::end" << endl;
}


void gameScene::cleanLevel() {

    line.please_clean_up = false;

    vector<b2Body*> body_living;

    body_living.push_back(rect_bottom.body);
    body_living.push_back(rect_top.body);
    body_living.push_back(rect_left.body);
    body_living.push_back(rect_right.body);

    for (int plant_id = 0; plant_id < plants.size(); plant_id++) {
        for (int flower_id = 0; flower_id < plants[plant_id].flowers.size(); flower_id++) {
            body_living.push_back(plants[plant_id].flowers[flower_id].body);
        }
        for (int balljoint_id = 0; balljoint_id < plants[plant_id].ballJoints.size(); balljoint_id++) {
            body_living.push_back(plants[plant_id].ballJoints[balljoint_id].body);
        }
        //body_living.push_back(plants[plant_id].body);
    }

    for (int softdrop_id = 0; softdrop_id < line.softdrops.size(); softdrop_id++) {
        //body_living.push_back(line.softdrops[softdrop_id].body);
        body_living.push_back(line.softdrops[softdrop_id].center.body);
        for(int ring_id = 0; ring_id < line.softdrops[softdrop_id].ring.size(); ring_id++) {
            body_living.push_back(line.softdrops[softdrop_id].ring[ring_id]->body);
        }
    }

    for(int vertex_id = 0; vertex_id < line.vertices.size(); vertex_id++) {
        body_living.push_back(line.vertices[vertex_id].circle.body);
        body_living.push_back(line.vertices[vertex_id].circle_top.body);
    }

    for(int water_id = 0; water_id < waterParticles.size(); water_id++) {
        //body_living.push_back(waterParticles[water_id].body);
        for(int particles_id = 0; particles_id < waterParticles[water_id].particles.size(); particles_id++) {
            body_living.push_back(waterParticles[water_id].particles[particles_id].body);
        }
    }
	/*
	for(int polyman_id = 0; polyman_id < polygonMan.polys.size(); polyman_id++) {
        body_living.push_back(polygonMan.polys[polyman_id].body);
    }*/
	//cout << "polyman size 2  " << polygonMan.polys.size() <<endl;


    //cout << "body_living count: " << body_living.size() << endl;

    b2World *world = box2d.getWorld();
    b2Body* node = world->GetBodyList();
    int body_count = 0;

    while (node) {

        b2Body* b = node;
        node = node->GetNext();

        bool temp_bla = false;

        for (int i = 0; i < body_living.size(); i++) {

            if(body_living[i] == node) {
                body_living.erase(body_living.begin()+i);
                temp_bla = true;
                body_count++;
                break;
            }
        }

        if(!temp_bla) {
            world->DestroyBody(b);
        }

    }
	polygonmanDeleted = true;
    //cout << "body_count: " << body_count << endl;

}

void gameScene::updateLevel() {

    //cout << "gamescene::updatelevel::start" << endl;

	int posY = height-100 - ((dropsDown)*50);

    if(line.please_clean_up)
        cleanLevel();

	if(!new_level && posY>150)
		animation_time=0;

    if(new_level) {

        //cout << "gamescene::updatelevel::new_level" << endl;

        if(animation_time == 0) {

            line.stop = true;

            // line.mySound.loadSound("sound/StartUp.wav");
            // line.mySound.play();

            animation_time_max = 0;
            for (int plant_id = 0; plant_id < plants.size(); plant_id++) {
                for (int flower_id = 0; flower_id < plants[plant_id].flowers.size(); flower_id++) {
                    animation_time_max++;
                }
            }
        }

        bool animation_done = true;
        for (int plant_id = 0; plant_id < plants.size(); plant_id++) {

            for (int flower_id = 0; flower_id < plants[plant_id].flowers.size(); flower_id++) {
                if(plants[plant_id].flowers[flower_id].isflouring == true) {
                    animation_done = false;
                    break;
                }
            }
            if(!animation_done)
                break;

        }


        int counter = 0;
        for (int plant_id = 0; plant_id < plants.size(); plant_id++) {

            for (int flower_id = 0; flower_id < plants[plant_id].flowers.size(); flower_id++) {
                if(counter <= animation_time/10) {
                    int x = plants[plant_id].flowers[flower_id].getPosition().x;
                    int y = plants[plant_id].flowers[flower_id].getPosition().y;
                    ofFill();
                    ofSetColor(255,255,0);
                    ofCircle(x,y,15);
                    ofSetColor(0,0,0);
                    ofCircle(x-5,y,2);
                    ofCircle(x+5,y,2);
                    ofNoFill();
                    ofBeginShape();
                    ofCurveVertex(x-10, y-5);
                    ofCurveVertex(x-10, y+5);
                    ofCurveVertex(x, y+10);
                    ofCurveVertex(x+10, y+5);
                    ofCurveVertex(x+10, y+5);
                    ofEndShape();
                }
                counter ++;
            }
        }

        if (animation_done && animation_time >= animation_time_max*10+10 && animation_time >= 200) {
            loadLevel(true);
        }

        animation_time++;
    }

    if(posY<150) {

        //cout << "gamescene::updatelevel::restart_level" << endl;

        if(animation_time == 0) {

            line.stop = true;

            // line.mySound.loadSound("sound/WAWAWAWA.wav");
            // line.mySound.play();

            animation_time_max = 0;
            for (int plant_id = 0; plant_id < plants.size(); plant_id++) {

                for (int joint_id = 0; joint_id < plants[plant_id].joints.size(); joint_id++) {
                    animation_time_max++;
                }
            }
        }

        if(animation_time%10 == 0) {
            int counter = 0;
            for (int plant_id = 0; plant_id < plants.size(); plant_id++) {

                for (int joint_id = 0; joint_id < plants[plant_id].joints.size(); joint_id++) {
                    if(counter == animation_time/10)
                        plants[plant_id].joints[joint_id].destroy();
                    counter ++;
                }
            }
        }

        if (animation_time >= animation_time_max*10+10 && animation_time >= 200) {
            loadLevel(false);
        }

        animation_time++;
    }

    //cout << "gamescene::updatelevel::end" << endl;


}

//--------------------------------------------------------------
void gameScene::draw(){

    cout << "gamescene::draw::start" << endl;

	if (isDebugMode)
	{
	}

    //cout << "gamescene::draw::box2d" << endl;
	box2d.draw();

    //cout << "gamescene::draw::background" << endl;
	background.draw();

	//cout << "gamescene::draw::polygonman" << endl;
	polygonMan.draw();

	//cout << "gamescene::draw::plants" << endl;
	for (int i=0;i<plants.size();++i)
		plants[i].draw();



    //cout << "gamescene::draw::line" << endl;
	line.draw();
	for (int i=0;i<waterParticles.size();++i)
		waterParticles[i].draw();

	updateLevel();

	//cout << "gamescene::draw::end" << endl;

}

//--------------------------------------------------------------
void gameScene::keyPressed  (int key){

	switch (key){
	    case 's':
            polygonMan.useFullCollorDifference=!polygonMan.useFullCollorDifference;
            break;
			//Treshold
        case '+':
        polygonMan.tLastUpdated = ofGetSystemTime();
            if(polygonMan.currentChangingValue=="offsetposition"&&polygonMan.offsetY<500)
				polygonMan.offsetY+=30;
             if(polygonMan.currentChangingValue=="Intensity_Treshold")
				polygonMan.threshold+=3;
            if(polygonMan.currentChangingValue=="Color_Treshold")
				polygonMan.c_threshold+=3;
			if(polygonMan.currentChangingValue=="Triangle_Smoothing"&&polygonMan.smoothAmount<0.95)
				polygonMan.smoothAmount+=0.05;
            if(polygonMan.currentChangingValue=="Triangle_Simplicity")
				polygonMan.simpleAmount+=0.3;
            if(polygonMan.currentChangingValue=="ImageBlur")
				polygonMan.Imageblur+=1;
            if(polygonMan.currentChangingValue=="colorDifferenceMethod"&&polygonMan.colorDifferenceMethod<2)
				polygonMan.colorDifferenceMethod+=1;
            if(polygonMan.currentChangingValue=="erode")
				polygonMan.erode+=1;
            if(polygonMan.currentChangingValue=="diletate")
				polygonMan.diletate+=1;
            if(polygonMan.currentChangingValue=="soundInputThreshold")
				minVolStartWind+=3;
			break;
		case '-':
		polygonMan.tLastUpdated = ofGetSystemTime();
            if(polygonMan.currentChangingValue=="offsetposition"&&polygonMan.offsetY>30)
				polygonMan.offsetY-=30;
            if(polygonMan.currentChangingValue=="soundInputThreshold")
				minVolStartWind-=3;
            if(polygonMan.currentChangingValue=="Intensity_Treshold")
				polygonMan.threshold-=3;
            if(polygonMan.currentChangingValue=="Color_Treshold")
				polygonMan.c_threshold-=3;
			if(polygonMan.currentChangingValue=="Triangle_Smoothing"&&polygonMan.smoothAmount>0.05)
				polygonMan.smoothAmount-=0.05;
            if(polygonMan.currentChangingValue=="Triangle_Simplicity"&&polygonMan.simpleAmount>0.3)
				polygonMan.simpleAmount-=0.3;
            if(polygonMan.currentChangingValue=="ImageBlur"&&polygonMan.Imageblur>0)
				polygonMan.Imageblur-=1;
            if(polygonMan.currentChangingValue=="colorDifferenceMethod"&&polygonMan.colorDifferenceMethod>0)
				polygonMan.colorDifferenceMethod-=1;
            if(polygonMan.currentChangingValue=="erode")
				polygonMan.erode-=1;
            if(polygonMan.currentChangingValue=="diletate")
				polygonMan.diletate-=1;

			break;
			break;
        case '1':
            polygonMan.currentChangingValue="Intensity_Treshold";
            polygonMan.tLastUpdated = ofGetSystemTime();
            break;
        case '2':
            polygonMan.currentChangingValue="Color_Treshold";
            polygonMan.tLastUpdated = ofGetSystemTime();
            break;
        case '3':
            polygonMan.currentChangingValue="Triangle_Simplicity";
            polygonMan.tLastUpdated = ofGetSystemTime();
            break;
        case '4':
            polygonMan.currentChangingValue="Triangle_Smoothing";
            polygonMan.tLastUpdated = ofGetSystemTime();
            break;
        case '5':
            polygonMan.currentChangingValue="ImageBlur";
            polygonMan.tLastUpdated = ofGetSystemTime();
            break;
        case '6':
            polygonMan.currentChangingValue="colorDifferenceMethod";
            polygonMan.tLastUpdated = ofGetSystemTime();
            break;
        case '7':
            polygonMan.currentChangingValue="erode";
            polygonMan.tLastUpdated = ofGetSystemTime();
            break;
        case '8':
            polygonMan.currentChangingValue="diletate";
            polygonMan.tLastUpdated = ofGetSystemTime();
            break;
        case '9':
            polygonMan.currentChangingValue="soundInputThreshold";
            break;
        case '0':
            polygonMan.currentChangingValue="offsetposition";
            break;

	    case ' ':
			polygonMan.bLearnBakground = true;
			polygonMan.tLastUpdated = ofGetSystemTime();
			break;

		case 'd':
			isDebugMode = !isDebugMode;
			break;
		case 'f':
			isFullScreen = !isFullScreen;
			if(!isFullScreen){
				ofSetWindowShape(width,height);
				ofSetFullscreen(false);
				// figure out how to put the window in the center:
				int screenW = ofGetScreenWidth();
				int screenH = ofGetScreenHeight();
				ofSetWindowPosition(width/2, height/2);
			} else if(isFullScreen == 1){
				ofSetFullscreen(true);
			}
			break;

        case 'l': {
            loadLevel(false);
            break;
        }
        case 'o': {
            loadLevel(true);
            break;
        }
		case 'm': {
			if (polygonMan.isMirrorImage ) polygonMan.isMirrorImage = false;
			else polygonMan.isMirrorImage = true;
			break;
		}
		case 'c': {
		    if(polygonMan.deviceID==0)
			{polygonMan.deviceID=1;break;}
			if(polygonMan.deviceID==1)
			{polygonMan.deviceID=0;break;}
//			if(polygonMan.deviceID==2)
//			{polygonMan.deviceID=0;}
			break;
		}

	}
}

//--------------------------------------------------------------
void gameScene::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void gameScene::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void gameScene::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void gameScene::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void gameScene::windowResized(int w, int h){

}

//-------------------------------------------------------------

void gameScene::box2dContactEventHandler ( const b2ContactPoint* point )
{

    //cout << "gamescene::box2dcontacteventhandler::start" << endl;

	vector<ofxBox2dCircle*> vF;
	vector <ofxBox2dCircle*> dropShapes;

	bool isDrop = false;
	int posToDel = -1;
	bool isFlower = false;
	b2Body* fToActivate;
	int pNToActivate = -1;
	int fNToActivate = -1;
	bool isDeform = false;
	int dPosToDelete = -1;



	// //iterate over raindrops
	for( int i=0; i<rainDrops->size(); i++ )
	{
		line.getDrop(i)->getRingList(dropShapes);
		for (int j=0; j<dropShapes.size(); ++j) {
			for( b2Body* s=dropShapes[j]->body/*->GetShapeList()*/; s; s=s->GetNext() )
			{
				//test if hit with flower
				if( point->shape1 == s || point->shape2 == s )
				{
					//cout << "Ball hit" << endl;
					isDrop = true;
					posToDel = i;
				}
			}
		}
	}

	//iterate over plants
	for( int i=0; i<plants.size(); i++ )
	{
		plants[i].getFlowers(vF);
		for (int j=0; j<vF.size(); ++j) {
			for( b2Body* s=vF[j]->body/*->GetShapeList()*/; s; s=s->GetNext() )
			{
				if( point->shape1 == s || point->shape2 == s )
				{
					//cout << "Flower hit" << endl;
					isFlower = true;
					fToActivate = s;
					pNToActivate = i;
					fNToActivate = j;
				}
			}
		}
	}

	//test deformation of drops
	for( int i=0; i<rainDrops->size(); i++ )
	{
		for( b2Body* s=line.getDrop(i)->center.body/*->GetShapeList()*/; s; s=s->GetNext() )
		{
			if( point->shape1 == s || point->shape2 == s )
			{
				isDeform = true;
				dPosToDelete = i;
			}
		}

	}
	if (isDeform) {
		WaterParticles p;
        ofxVec3f c;
        c.x = line.getDrop(dPosToDelete)->color.r;
        c.y = line.getDrop(dPosToDelete)->color.g;
        c.z = line.getDrop(dPosToDelete)->color.b;
        p.createParticles(box2d.getWorld(), 10, line.getDrop(dPosToDelete)->ring[0]->getPosition().x, line.getDrop(dPosToDelete)->ring[0]->getPosition().y, c);
        waterParticles.push_back(p);
		line.deleteDrop(dPosToDelete,false);
	}

	if (isDrop && isFlower) {

	    bool level_done = true;

	    if(plants[pNToActivate].cSchemeFlower == line.getDrop(posToDel)->cSchemeDrop) {

            plants[pNToActivate].flowers[fNToActivate].isflouring = true;

            for (int plant_id = 0; plant_id < plants.size(); plant_id++) {

                for (int flower_id = 0; flower_id < plants[plant_id].flowers.size(); flower_id++) {
                    if(plants[plant_id].flowers[flower_id].isflour != true && plants[plant_id].flowers[flower_id].isflouring != true) {
                        level_done = false;
                        break;
                    }
                }
                if(!level_done)
                    break;

            }

	    }
	    else
            level_done = false;

        WaterParticles p;
        ofxVec3f c;
        c.x = line.getDrop(posToDel)->color.r;
        c.y = line.getDrop(posToDel)->color.g;
        c.z = line.getDrop(posToDel)->color.b;
        p.createParticles(box2d.getWorld(), 10, line.getDrop(posToDel)->ring[0]->getPosition().x, line.getDrop(posToDel)->ring[0]->getPosition().y, c);
        waterParticles.push_back(p);
        line.deleteDrop(posToDel,true);


        if(level_done)
            new_level = true;

	}

	//cout << "gamescene::box2dcontacteventhandler::end" << endl;

}

//---------------------------------------------------------------------
void gameScene::audioReceived 	(float * input, int bufferSize, int nChannels){
	// samples are "interleaved"
	float maxL = 0;
	float maxR = 0;
	for (int i = 0; i < bufferSize; i++){
		if (input[i*2] > maxL) maxL = input[i*2];
		if (input[i*2+1] > maxL) maxR = input[i*2+1];
	}
	if (maxL > maxRecVolume)
		maxRecVolume = maxL;

	if (maxL > maxRecVolume*minVolStartWind) {
		b2Vec2 mid;
		mid.x = width/2;
		mid.y = height/2;

		float radius = width/2;
		float alpha = ofRandom(0, 2*PI);

		b2Vec2 sourceWind;
		sourceWind.x = mid.x + radius*cos(alpha);
		sourceWind.y = mid.y + radius*sin(alpha);

		b2Vec2 sourceAni;
		sourceAni.x = mid.x + radius*cos(alpha)/3*2;
		sourceAni.y = mid.y + radius*sin(alpha)/3*2;

		b2Vec2 dirWind = mid - sourceWind;
		ofxVec2f t1;
		t1.x = dirWind.x;
		t1.y = dirWind.y;
		t1.normalize();

		t1.scale(radius/7);

		b2Vec2 forceWind;
		forceWind.x =t1.x;
		forceWind.y =t1.y;

		//create flower list
		//vector<Flower*> tflower;
		for (int i=0; i<plants.size(); ++i)
		{
			for (int j=0; j<plants[i].flowers.size(); ++j)
			{
				//tflower.push_back(&plants[i].flowers[j]);
				b2Body* b=plants[i].flowers[j].body;
				b->ApplyForce(forceWind, sourceWind,true);
			}
		}

		//cout <<"force " <<forceWind.x <<" " <<forceWind.y << "  source " <<sourceWind.x <<" " <<sourceWind.y <<endl;
		background.setupWind(sourceAni, forceWind);

	}
}

//-------------contact listener functions---------------------------

void MyContactListener::Add( const b2ContactPoint* point )
{
	gameScene* appPtr = ((gameScene*)ofGetAppPtr());
	appPtr->box2dContactEventHandler( point );
}

void MyContactListener::Remove(const b2ContactPoint* point)
{
	//
}



