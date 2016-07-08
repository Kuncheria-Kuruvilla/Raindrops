/*
 *  Line.h
 *  Raindrops
 *
 *  Created by Deborah Schmidt.
 *  Copyright 2010 All rights reserved.
 *
 */
#ifndef _RAINDROPLINE_H
#define _RAINDROPLINE_H

#pragma once
#include "Drop.h"
#include "ColorSchemes.h"
#include <algorithm>
#include <vector>

typedef struct vertex {

	bool 	        beingMoved;
	ofxBox2dCircle  circle;
	ofxBox2dCircle  circle_top;

};

typedef struct {
	bool operator() (vector<int> i,vector<int> j) { return (i[1]>j[1]);}
} sortOperator;


class RaindropLine : public ofxBox2dBaseShape{


private:
    b2World*                myworld;
    sortOperator            so;
	int                     drop_time_count;
    int                     mywidth;
    int                     myheight;
    vector<int>             colorset;

public:
	const static int        line_offset = 0;
	constexpr static float      lineheight_factor = 2;
	const static int        drop_period = 40;
	const static int        drop_max = 10;
	const static int        vertex_count = 13;
	ofColor                 line_color;
	ofColor                 background_color;
	vector<vertex>          vertices;
	bool                    is_dropping[vertex_count];
    ofxBox2dJoint			joints[vertex_count-2];
    vector <Drop>	        raindrops;
    vector<costumSoftBody>	softdrops;
	int						*dropsDown;
    ofSoundPlayer           mySound;
    bool                    stop;
    bool                    please_clean_up;


//	void setup(b2World * world, int width, int height, vector<int> color, ofColor background_c, ofColor line_c, int *dropsFallDown) {
	void setup(b2World * world, int *dropsFallDown, int width = 1024, int height = 768, vector<int> color = vector<int>(0), ofColor background_c = ofColor(), ofColor line_c = ofColor()) {

		//--cmt--mySound.setMultiPlay(true);
	    cout << "raindropline::setup::clear"<< endl;
        vertices.reserve(13);
		softdrops.clear();
	    raindrops.clear();
	    colorset.clear();
	    vertices.clear();

	    cout << "raindropline::setup::init"<< endl;

        myworld = world;
	    colorset = color;
	    mywidth = width;
	    myheight = height;

	    background_color = background_c;
	    line_color = line_c;

	    drop_time_count = 0;
	    stop = false;
	    please_clean_up = false;

		dropsDown = dropsFallDown;

		//cout << "raindropline::setup::vertex0"<< endl;

		vertices.push_back(vertex());

        //vertices[0].circle.setPhysics(10.0, 0.0, 0.0);
		vertices[0].circle.setup(world, 0, line_offset, 10);

		for(int i=vertices.size(); i<vertex_count-1;i=vertices.size()) {
		    cout << "raindropline::setup::vertex" << i << endl;
		    vertices.push_back(vertex());
            cout<<" Vertex "<<i<<" Halfway : ";
		    is_dropping[i] = false;

		    int temp_height = line_offset+lineheight_factor*(pow((double)vertex_count/2,
																 (double)2)-pow((double)(i-(vertex_count/2)),(double)2));
            int temp_width = mywidth/(vertex_count-1)*i;

            //setPhysics(mass, bounce, friction)
            vertices[i].circle.setPhysics(10.0, 0.0, 0.0);
            vertices[i].circle.setup(world, temp_width, temp_height, 10);
            vertices[i].circle_top.setup(world, temp_width, 0, 10);

            // vertices[i].circle.disableCollition();
            // vertices[i].circle_top.disableCollition();

            joints[i-1].setWorld(world);
            joints[i-1].setup(world,vertices[i].circle_top.body, vertices[i].circle.body, 3.0, 0.5);
            cout<<"  Vertex "<<i<<"complete\n";

		}


		cout << "raindropline::setup::vertex" << vertex_count-1<<endl;

		vertices.push_back(vertex());

        //vertices[vertices.size()-1].circle.setPhysics(10.0, 0.0, 0.0);
		vertices[vertices.size()-1].circle.setup(world, mywidth, line_offset, 10);

		cout << "raindropline::setup::end"<< endl;
	}


    void drop() {

		for(int i = 0; i < raindrops.size(); i++) {

            if(raindrops[i].dead) {

                // mySound.loadSound("sound/indon_harp_note_5.wav");
                // mySound.play();

                float r = 30;//raindrops[i].drop_radius;
                costumSoftBody p;
                p.setup(myworld, raindrops[i].p4.x,raindrops[i].p4.y, r,false, raindrops[i].colorScheme);
                p.color.r = raindrops[i].color[0];
                p.color.g = raindrops[i].color[1];
                p.color.b = raindrops[i].color[2];
                softdrops.push_back(p);

                is_dropping[raindrops[i].vertex_id] = false;
                raindrops.erase(raindrops.begin()+i);
            }

        }


        if(drop_time_count >= drop_period && raindrops.size() < drop_max) {

            vector< vector<int> > order;
            for (int i = 1; i < vertices.size()-1; i++){
                vector<int> temp;
                temp.push_back(i);
                temp.push_back(vertices[i].circle.getPosition().y);
                order.push_back(temp);
            }

            sort(order.begin(),order.end(),so);

            int lowest = order[0][0];

            if(!is_dropping[lowest] && !is_dropping[lowest-1] && !is_dropping[lowest+1]) {

                is_dropping[lowest] = true;

                Drop drop;
                drop.setPhysics(1.0,0,0);
                int colorScheme = colorset[(int)ofRandom(0,colorset.size())];
                drop.setup(myworld, &vertices[lowest-1].circle, &vertices[lowest].circle,
                           &vertices[lowest+1].circle, &vertices[1].circle,
                           &vertices[vertex_count-2].circle, lowest, colorScheme);

                int temp[3];
                cSgetColor(colorScheme, (int)ofRandom(0,4), temp);
                drop.setColor(temp[0],temp[1],temp[2]);

                raindrops.push_back(drop);

                drop_time_count = 0;

            }

        }
        else {
            drop_time_count++;
        }

    }

    void updateJoints() {

        for(int i=1; i<vertices.size()-1; i++) {
            vertices[i].circle_top.setPosition(vertices[i].circle.getPosition().x,0);
        }

    }

    void clearDrops() {

        bool changed = false;
		//cout << "start clear" <<endl;
        for(int i = 0; i<softdrops.size();i++) {
			if (softdrops[i].destroyMe) {
				deleteDrop(i, true, false);
				(*dropsDown)++;
			}
		}

		 for(int i = 0; i<softdrops.size();i++) {
            if( (softdrops[i].ring[0]->getPosition().y > mywidth) || (softdrops[i].ring[0]->getPosition().y > myheight-30) ||
			   (softdrops[i].ring[0]->getPosition().y < 0) || (softdrops[i].ring[0]->getPosition().y < 0)) {

                changed = true;

                // mySound.loadSound("sound/cell_death.wav");
                // mySound.play();

                deleteDrop(i, true, false);

				(*dropsDown)++;
            }
        }

        if(changed)
            please_clean_up = true;
		//cout << "end clear" <<endl;
    }

	void draw() {

	    //cout << "line::draw::cleardrops" << endl;

	    clearDrops();

        //ofSoundUpdate();

        if(!stop) {

            //cout << "line::draw::drop" << endl;

            drop();

            for(int i=0; i<raindrops.size(); i++) {
                raindrops[i].update();
                raindrops[i].draw();
            }
        }

	    //updateJoints();

	    //cout << "line::draw::background" << endl;

        ofFill();
        ofSetColor(background_color.r,background_color.g,background_color.b);
        ofBeginShape();

        for (int i = 0; i < vertices.size(); i++){


            //first vertex
            if (i == 0){
                ofCurveVertex(vertices[0].circle.getPosition().x, vertices[0].circle.getPosition().y);
                ofCurveVertex(vertices[0].circle.getPosition().x, vertices[0].circle.getPosition().y);
            }
            //last vertex
            else if (i == vertices.size()-1){
                ofCurveVertex(vertices[i].circle.getPosition().x, vertices[i].circle.getPosition().y);
                ofCurveVertex(vertices[i].circle.getPosition().x, vertices[i].circle.getPosition().y);
            }
            //rest which could have drops falling down at their place
            else {
                ofCurveVertex(vertices[i].circle.getPosition().x, vertices[i].circle.getPosition().y);
            }

        }

        ofEndShape();

        //cout << "line::draw::line" << endl;

        ofNoFill();
		ofEnableSmoothing();
		ofSetLineWidth(2);
        ofSetColor(line_color.r, line_color.g, line_color.b);
        ofBeginShape();

        for (int i = 0; i < vertices.size(); i++){

            if (i == 0){
                ofCurveVertex(vertices[0].circle.getPosition().x, vertices[0].circle.getPosition().y);
                ofCurveVertex(vertices[0].circle.getPosition().x, vertices[0].circle.getPosition().y);
            }
            else if (i == vertices.size()-1){
                ofCurveVertex(vertices[i].circle.getPosition().x, vertices[i].circle.getPosition().y);
                ofCurveVertex(vertices[i].circle.getPosition().x, vertices[i].circle.getPosition().y);
            }
            else {
                ofCurveVertex(vertices[i].circle.getPosition().x, vertices[i].circle.getPosition().y);
            }

        }

        ofEndShape();

        for(int i=0; i<softdrops.size(); i++) {

            //cout << "line::draw::softdrop" << i << endl;

            softdrops[i].draw();
        }

        /*
		 for(int i=0; i<(vertex_count-2); i++) joints[i].draw();
		 for(int i=0; i<vertex_count; i++) vertices[i].circle.draw();
		 for(int i=1; i<vertex_count-1; i++) vertices[i].circle.draw();
		 */

        //cout << "line::draw::end" << endl;

	}

	int getDropWithID(int id){

        for (int i = 0; i<raindrops.size(); i++) {
            if(raindrops[i].vertex_id == id) {
                return i;
            }
        }

	}

	costumSoftBody* getDrop(int id) {

        return &(softdrops[id]);

	}

	void deleteDrop(int id, bool good, bool last = true) {

        if(good) {
            //mySound.loadSound("sound/arrive.wav");
        }
        else {
            // mySound.loadSound("sound/tropfengehtkaputt.wav");
            // mySound.play();
        }

        softdrops.erase(softdrops.begin()+id);

        if(last)
            please_clean_up = true;

        //cout << "cell death" << endl;
	}

};


#endif // LINE
