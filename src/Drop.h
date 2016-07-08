/*
 *  Drop.h
 *  Raindrops
 *
 *  Created by Deborah Schmidt.
 *  Copyright 2010 All rights reserved.
 *
 */
#ifndef _DROP_H
#define _DROP_H


class Drop : public ofxBox2dBaseShape {


private:
    ofPoint             p1;
    ofPoint             p7;
    ofxBox2dCircle*     left_vertex;
    ofxBox2dCircle*     center_vertex;
    ofxBox2dCircle*     right_vertex;
    ofxBox2dCircle*     first_vertex;
    ofxBox2dCircle*     last_vertex;

public:
    int                 lifelength;
    int                 max_depth;
    int                 drop_radius;
    int                 lifetime;
	int                 color[3];
    int                 vertex_id;
    int                 colorScheme;
    ofPoint             p2;
    ofPoint             p3;
    ofPoint             p4;
    ofPoint             p5;
    ofPoint             p6;

    bool 				dead;

    void setup(b2World* world, ofxBox2dCircle* vertex1,ofxBox2dCircle* vertex2, ofxBox2dCircle* vertex3,
               ofxBox2dCircle* vertex4, ofxBox2dCircle* vertex5, int id, int cScheme) {

        lifelength      = 180;
        max_depth       = 70;
        drop_radius     = 13;

        lifetime        = 0;

        vertex_id       = id;
        left_vertex     = vertex1;
        center_vertex   = vertex2;
        right_vertex    = vertex3;
        first_vertex    = vertex4;
        last_vertex     = vertex5;

        colorScheme     = cScheme;

        color[0] = 0;
        color[1] = 0;
        color[2] = 0;

        update_vertices();

    }

    void setColor(int r, int g, int b) {

        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    void update() {

        if(lifetime >= lifelength)  {
            this->destroy();
            dead = true;
        }
        else {
            lifetime++;
            update_vertices();
        }

    }

    void update_vertices(){

        //fixed at the left and right vertex
        p1.set( left_vertex->getPosition().x,               left_vertex->getPosition().y );
        p7.set( right_vertex->getPosition().x,              right_vertex->getPosition().y );

        //fixed in x direction at center vertex, dynamic in y direction
        p4.set( center_vertex->getPosition().x,             center_vertex->getPosition().y + (float)max_depth/(float)lifelength * lifetime );

        //dynamic
        if((float)lifetime>(float)lifelength/2)
            p2.set( p1.x + (p4.x - p1.x) * 2 * ((float)lifetime/(float)lifelength-0.5) ,
                    p1.y + (p7.y-p1.y + 30) *((float)lifetime/(float)lifelength-0.5) );
        else
            p2.set( p1.x, p1.y);

        if((float)lifetime>(float)lifelength/2)
            p6.set( p7.x - (p7.x - p4.x) * 2 * ((float)lifetime/(float)lifelength-0.5) ,
                    p7.y + (p1.y-p7.y +30) *((float)lifetime/(float)lifelength-0.5) );
        else
            p6.set( p7.x, p7.y);

        p3.set( (p1.x+p4.x)/2 + ((p4.x-p1.x)/2-drop_radius)*(float)lifetime/(float)lifelength,
                (p1.y+center_vertex->getPosition().y)/2 + (center_vertex->getPosition().y+max_depth-drop_radius*1.2
                    -(p1.y+center_vertex->getPosition().y)/2)*(float)lifetime/(float)lifelength );

        //fixed. symmetric to vertical line at p4: p2<->p6, p3<->p5
        p5.set( 2*p4.x-p3.x,                                p3.y );
        //dp6.set( 2*p4.x-p2.x,                                p2.y );

    }


	void draw() {

        ofFill();

        float d_time = (float)lifetime/(float)lifelength;
        int r,g,b;

        if(color[0]==0 && color[1]==0 && color[2]==0) {
            float d_width = (center_vertex->getPosition().x-first_vertex->getPosition().x)
                            /(last_vertex->getPosition().x-first_vertex->getPosition().x);
            r = (d_width > 0.5) ? 0 : (255-105*d_width*2);
            g = 100+50*d_time;
            b = (d_width < 0.5) ? 0 : (100+155*(d_width-0.5)*2);
        }
        else {
            r = color[0];
            g = color[1];
            b = color[2];

        }

        ofSetColor(r,g,b);
        ofBeginShape();

        ofVertex(p4.x, 0);
        ofCurveVertex(p1.x, p1.y);
        ofCurveVertex(p1.x, p1.y);
        ofCurveVertex(p2.x, p2.y);
        ofCurveVertex(p3.x, p3.y);
        ofCurveVertex(p4.x, p4.y);
        ofCurveVertex(p5.x, p5.y);
        ofCurveVertex(p6.x, p6.y);
        ofCurveVertex(p7.x, p7.y);
        ofCurveVertex(p7.x, p7.y);
        ofVertex(p4.x, 0);

        ofEndShape();
/*
        ofEnableAlphaBlending();
		ofFill();
		ofSetColor(0,0,0,100);
		ofCircle(p1.x,p1.y,4);
		ofCircle(p2.x,p2.y,4);
		ofCircle(p3.x,p3.y,4);
		ofCircle(p4.x,p4.y,4);
		ofCircle(p5.x,p5.y,4);
		ofCircle(p6.x,p6.y,4);
		ofCircle(p7.x,p7.y,4);
        ofDisableAlphaBlending();
*/

	}
};


#endif
