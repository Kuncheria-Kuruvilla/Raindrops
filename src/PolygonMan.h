#ifndef POLYGONMAN_H_INCLUDED
#define POLYGONMAN_H_INCLUDED


#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"
#include "ofxContourAnalysis.h"
#include "costumSoftBody.h"
#include "ofxTriangle.h"
#include "linkedList.h"

#define IMG_WIDTH 320
#define IMG_HEIGHT 240


class PolygonMan
{

private:

    int width;
    int height;


    ofVideoGrabber 	vidGrabber;

    unsigned char * diffWorkImage;
    unsigned char * diffWorkImage_skin;

    //Color Chanels for current and Background picture
    ofxCvColorImage		colorImg;
    ofxCvColorImage		colorBG;

    //holds sortet lists for median picture
    linkedList              medianListArray[IMG_WIDTH*IMG_HEIGHT*3];
    //Number of CameraFrames used for Median Picture
    int                     numberOfMedianPictures;
    int                     currentMedianPicture;
    unsigned char *         MedianWorkImage;


    ofxCvGrayscaleImage 	greenChannel;
    ofxCvGrayscaleImage 	redChannel;
    ofxCvGrayscaleImage 	blueChannel;

    ofxCvGrayscaleImage 	greenChannel_bg;
    ofxCvGrayscaleImage 	redChannel_bg;
    ofxCvGrayscaleImage 	blueChannel_bg;

    ofxCvGrayscaleImage 	GreyImg;
    ofxCvGrayscaleImage 	GreyBG;

    //Binary Picture
    ofxCvGrayscaleImage 	BinImage;
    ofxCvGrayscaleImage 	BinImage_skin;
    //Difference Picture
    ofxCvGrayscaleImage 	DiffImage;
    bool                    BackgroundCapturedOnce;
    void drawContour(vector <ofPoint> &contourToDraw);
    void cleanUp();
    void copyVector(vector <ofPoint> &vectorIn, vector <ofPoint> &vectorOut);
    //Info Output
    string                  info;
    // contour stuff
    ofxCvContourFinder		contour;
    ofxContourAnalysis		contourAnalysis;
    vector <ofPoint>		simpleCountour;
    vector <ofPoint>		simpleCountour_save;

    ofxTriangle              triangle; //Triangulated structure
    vector <ofxBox2dPolygon> polys; // polygons in Triangulated structure

public:
    //Basic Functions of Polygon Man
    void setup(int width, int height);
    void update(b2World * world, bool *polygonmanDeleted);
    void draw();


    void keyPressed  (int key);

    //Videograbber
    string                  currentDeviceName;
    int                     offsetY;
    int                     deviceID;
    //Treshold for Difference Picture to Binary Picture
    int 				    threshold;
    int 				    c_threshold;

    //Used for Interation, highest Point of Polygon Strucure
    struct Point
    {
        int   X;
        int   Y;
    } highestPoint;
    int                     temp;

    //Boolean to set still Background
    bool				    bLearnBakground;
    bool				    isMirrorImage;
    bool                    isFirstInitioalized;

    float					simpleAmount; //Simplify the polygon man values: 1- 25
    float					smoothAmount; // smooth the polygon structure values: 0.01 -1.00

    //Boolean to switch between Grey-Based and Color Based Difference Picture
    bool                    useFullCollorDifference;
    string                  currentChangingValue;
    int                     Imageblur;
    int                     colorDifferenceMethod;
    int                     diletate;
    int                     erode;
    int				        vw, vh;

    //timeValues
    float tLastUpdated;
};


#endif // POLYGONMAN_H_INCLUDED
