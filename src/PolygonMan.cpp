#include "PolygonMan.h"

void PolygonMan::setup(int width_, int height_){

    width=width_;
    height=height_;

    cleanUp();

    if(!isFirstInitioalized)
    {
        offsetY=0;
        isMirrorImage = false;
        useFullCollorDifference=true;
        BackgroundCapturedOnce=false;
        //Simplify Polygon Structure
        simpleAmount     = 1.0f;
        //smooth Polygon Structure
        smoothAmount     = 1.0f;
        numberOfMedianPictures =5;
        currentMedianPicture =0;
        Imageblur= 3;
        diletate=0;
        erode=0;
        colorDifferenceMethod=0;
        currentChangingValue="Binary_Treshold";
        threshold = 24;
        c_threshold = 24;
        MedianWorkImage=new unsigned char[IMG_WIDTH*IMG_HEIGHT*3];
        diffWorkImage=new unsigned char[IMG_WIDTH*IMG_HEIGHT];
        diffWorkImage_skin=new unsigned char[IMG_WIDTH*IMG_HEIGHT];
        colorBG.allocate(IMG_WIDTH,IMG_HEIGHT);
        greenChannel_bg.allocate(IMG_WIDTH,IMG_HEIGHT);
        redChannel_bg.allocate(IMG_WIDTH,IMG_HEIGHT);
        blueChannel_bg.allocate(IMG_WIDTH,IMG_HEIGHT);
        GreyBG.allocate(IMG_WIDTH,IMG_HEIGHT);
        bLearnBakground = false;
        deviceID=0;

        isFirstInitioalized=true;
    }
    colorImg.allocate(IMG_WIDTH,IMG_HEIGHT);
    DiffImage.allocate(IMG_WIDTH,IMG_HEIGHT);
    BinImage.allocate(IMG_WIDTH,IMG_HEIGHT);
    BinImage_skin.allocate(IMG_WIDTH,IMG_HEIGHT);
    greenChannel.allocate(IMG_WIDTH,IMG_HEIGHT);
    redChannel.allocate(IMG_WIDTH,IMG_HEIGHT);
    blueChannel.allocate(IMG_WIDTH,IMG_HEIGHT);
    GreyImg.allocate(IMG_WIDTH,IMG_HEIGHT);

	contourAnalysis.setSize(IMG_WIDTH,IMG_HEIGHT);

/*    cout << "Changing device to:" <<deviceID<<endl;*/
    vidGrabber.close();
    if(deviceID==NULL)
    { cout<<"deviceid is null"<<endl;
        deviceID=0;}
    vidGrabber.setDeviceID(deviceID);
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(IMG_WIDTH,IMG_HEIGHT);
    tLastUpdated = 0;
};

void PolygonMan::update(b2World * world, bool *polygonmanDeleted){

    //------------------------------------IMAGE STUFF----------------------------------------------------
	//cout << "update the polygon man" <<endl;
    bool bNewFrame = false;
    vidGrabber.update();
	bNewFrame = vidGrabber.isFrameNew();

if (bLearnBakground == true)
    {
        if(currentMedianPicture==0)
        {
             for (int i = 0; i < (IMG_WIDTH*IMG_HEIGHT*3); i++)
            {
                medianListArray[i].~linkedList();
            }
        }
        if(currentMedianPicture<numberOfMedianPictures)
        {
            for (int i = 0; i < (IMG_WIDTH*IMG_HEIGHT*3); i++)
            {
                medianListArray[i].sort_add(colorImg.getPixels()[i]);
            }
            currentMedianPicture++;
        }
        else
        {
            for (int i = 0; i < (IMG_WIDTH*IMG_HEIGHT*3); i++)
            {
                MedianWorkImage[i]=medianListArray[i].getValueAt(numberOfMedianPictures/2);
            }
            colorBG.setFromPixels(MedianWorkImage,IMG_WIDTH,IMG_HEIGHT);
            bLearnBakground=false;
            currentMedianPicture=0;
            BackgroundCapturedOnce=true;
        }
    }

	if (bNewFrame)
	{
		colorImg = vidGrabber.getPixels();
        colorImg.blur(Imageblur);

		if (isMirrorImage)
			colorImg.mirror(false, true);

	    if(useFullCollorDifference)//Generate Binary Picture from Highest Contrast of each Color Chanel
	    {
            //Get Color Chanels
            int totalPixels = IMG_WIDTH*IMG_HEIGHT;

            //colorImg=SmoothedColorWorkImage;
            colorImg.convertToGrayscalePlanarImages(redChannel,greenChannel,blueChannel);
            colorBG.convertToGrayscalePlanarImages(redChannel_bg,greenChannel_bg,blueChannel_bg);

            unsigned char * pixels_r = redChannel.getPixels();
            unsigned char * pixels_g = greenChannel.getPixels();
            unsigned char * pixels_b = blueChannel.getPixels();

            unsigned char * pixels_r_bg = redChannel_bg.getPixels();
            unsigned char * pixels_g_bg = greenChannel_bg.getPixels();
            unsigned char * pixels_b_bg = blueChannel_bg.getPixels();


/*            Generate Difference Picture by searching for biggest difference in each of the 3 Color Channels*/
           if(colorDifferenceMethod==1)
           {
                for (int i = 0; i < totalPixels; i++)
                    diffWorkImage[i]= min(255,max(max( abs(pixels_r[i]-pixels_r_bg[i]),abs(pixels_g[i]-pixels_g_bg[i])), abs(pixels_b[i]-pixels_b_bg[i])));

                DiffImage.setFromPixels(diffWorkImage,IMG_WIDTH,IMG_HEIGHT);
                BinImage=DiffImage;
                BinImage.threshold(threshold);
           }
/*           Generate Difference Picture from Distance in Color Space and Distance in lightness*/
           if(colorDifferenceMethod==0)
           {
               for (int i = 0; i < totalPixels; i++)
               {
                    double a=(pixels_r[i]*pixels_r_bg[i])+(pixels_g[i]*pixels_g_bg[i])+(pixels_b[i]*pixels_b_bg[i]);
                    double b=((pixels_r_bg[i]*pixels_r_bg[i])+(pixels_g_bg[i]*pixels_g_bg[i])+(pixels_b_bg[i]*pixels_b_bg[i]));
                    double d=a/b;
                    double v1=(pixels_r[i]-d*pixels_r_bg[i]);
                    v1=v1*v1;
                    double v2=(pixels_g[i]-d*pixels_g_bg[i]);
                    v2=v2*v2;
                    double v3=(pixels_b[i]-d*pixels_b_bg[i]);
                    v3=v3*v3;
                    diffWorkImage[i]=0;
                    diffWorkImage_skin[i]=0;

                    //treshold für ligthness and color
                    if((abs((d)-1))>(double)threshold/100||sqrt(v1+v2+v3)>c_threshold)
                    {
                        diffWorkImage[i]=255;
                    }
               }
               DiffImage.setFromPixels(diffWorkImage,IMG_WIDTH,IMG_HEIGHT);
               BinImage=DiffImage;
            }

            for(int i = 0;i<diletate;i++)
                BinImage.dilate();

            for(int i = 0;i<erode;i++)
                BinImage.erode();
	    }

	    else //Get Binary from two grey Pictures
	    {
            GreyImg=colorImg;
	        GreyBG=colorBG;
	        BinImage.absDiff(GreyBG, GreyImg);
            BinImage.threshold(threshold);
	    }
	    //----------------------------------POLYGON STUFF------------------------------------------------

        //Generate Contour Lines for White Areas in Binary Picture
	    contour.findContours(BinImage, 30, IMG_WIDTH*IMG_HEIGHT, 20, true);
		triangle.clear();
		int i;

        if (!*polygonmanDeleted){
            for (i=polys.size()-1; i>=0; i--)
                world->DestroyBody(polys[i].body);
        }
		*polygonmanDeleted = false;

        polys.clear();
        simpleCountour.clear();
        simpleCountour_save.clear();
        highestPoint.Y=1000;
        highestPoint.X=0;

		for(i=0;i<min(2,contour.nBlobs);i++){
			contourAnalysis.simplify(contour.blobs[i].pts, simpleCountour, simpleAmount);
			contourAnalysis.smooth(simpleCountour,simpleCountour,smoothAmount);

            //save first simple contour for drawing
            if(i==0)
               copyVector(simpleCountour,simpleCountour_save);

            //triangulate Contour
			if(simpleCountour.size()>3)
                triangle.triangulate(simpleCountour, max(3.0f, (float)simpleCountour.size()));

            //generate polygons out of triangulated mesh and add them to world
            for (int j=triangle.triangles.size()-1; j>=0; j--)
            {
                ofxTriangleData* tData;
				tData = &triangle.triangles[j];
                //Get highest point of polygon structure
                if(tData->a.y*3.2<highestPoint.Y){
                    highestPoint.Y=tData->a.y*height/IMG_HEIGHT;
                    highestPoint.X=tData->a.x*width/IMG_WIDTH;}
				ofxBox2dPolygon poly;

				poly.addVertex((tData->a.x)*width/IMG_WIDTH, ((tData->a.y)*height/IMG_HEIGHT)+offsetY);
				poly.addVertex((tData->b.x)*width/IMG_WIDTH, ((tData->b.y)*height/IMG_HEIGHT)+offsetY);
				poly.addVertex((tData->c.x)*width/IMG_WIDTH, ((tData->c.y)*height/IMG_HEIGHT)+offsetY);

				if(poly.isGoodShape()) {

					poly.create(world);
					polys.push_back(poly);
                }
            }
			//cout << "end poly creation" <<endl;
		}
    }
	//cout <<"end the polyman update"<<endl;
};


void PolygonMan::draw(){
    cout << "polygonman::draw::start" << endl;
        ofSetHexColor(0xffffff);

        drawContour(simpleCountour_save);
        drawContour(simpleCountour);


    if (ofGetSystemTime()-tLastUpdated < 50000)
    {
        ofSetColor(0xffffff);
        colorImg.draw(280,20);
        BinImage.draw(280,280);
        colorBG.draw( 20 ,280);
        DiffImage.draw(20,20);
        ofSetColor(255,255,255);
        ofRect(20,500,300,300);
        ofSetColor(0,0,0);

        info="";
        info += "highestpoint_X: " + ofToString(highestPoint.X, 1) + "\n";
        info += "highestpoint_Y: " + ofToString(highestPoint.Y, 1) + "\n\n";
        info += "currentChangingValue: " + currentChangingValue + "\n";
        info += "threshold: " + ofToString(threshold, 1) + "\n";
        info += "simpleAmount: " + ofToString(simpleAmount, 1) + "\n";
        info += "smoothAmount: " + ofToString(smoothAmount, 1) + "\n";
        info += "Imageblur: " + ofToString(Imageblur, 1) + "\n";
        info += "colorDifferenceMethod: " + ofToString(colorDifferenceMethod, 1) + "\n";
        info += "diletate: " + ofToString(diletate, 1) + "\n";
        info += "erode: " + ofToString(erode, 1) + "\n";
        info += "chosen Device: "+ofToString(deviceID, 1);
        ofDrawBitmapString(info,20, 280*2);
    }
        cout << "polygonman::draw::info" << endl;
     cout << "polygonman::draw::end" << endl;
};

void PolygonMan::cleanUp(){
    vidGrabber.close();
    colorImg.clear();
    DiffImage.clear();
    BinImage.clear();
    greenChannel.clear();
    redChannel.clear();
    blueChannel.clear();
    GreyImg.clear();
    polys.clear();
}

void PolygonMan::drawContour(vector <ofPoint> &contourToDraw){
        int i;
  	    ofPoint p1;
        ofPoint p2;
        ofBeginShape();
        ofEnableAlphaBlending();
        ofSetColor(156, 156, 156,100);
        ofSetLineWidth(5);
        if(contourToDraw.size()>1){
            p1.x = contourToDraw[1].x;
            p1.y = contourToDraw[1].y;
            p2.x = contourToDraw[contourToDraw.size()-1].x;
            p2.y = contourToDraw[contourToDraw.size()-1].y;
            ofLine(p1.x*width/IMG_WIDTH, (p1.y*height/IMG_HEIGHT)+offsetY,p2.x*width/IMG_WIDTH, (p2.y*height/IMG_HEIGHT)+offsetY);
            ofVertex(p2.x*width/IMG_WIDTH, (p2.y*height/IMG_HEIGHT)+offsetY);
        }
        for(int i=contourToDraw.size()-2; i>0; i--) {
            p1.x = contourToDraw[i].x;
            p1.y = contourToDraw[i].y;
            p2.x = contourToDraw[i+1].x;
            p2.y = contourToDraw[i+1].y;
            ofLine(p1.x*width/IMG_WIDTH, (p1.y*height/IMG_HEIGHT)+offsetY,p2.x*width/IMG_WIDTH, (p2.y*height/IMG_HEIGHT)+offsetY);
            ofVertex(p1.x*width/IMG_WIDTH, (p1.y*height/IMG_HEIGHT)+offsetY);
        }
        ofEndShape();
        ofSetLineWidth(1);
        ofDisableAlphaBlending();
        ofSetColor(0, 0, 0);
    };

//Copys a Vector
    void PolygonMan::copyVector(vector <ofPoint> &vectorIn, vector <ofPoint> &vectorOut){
        int length = vectorIn.size();
        vectorOut.clear();
		vectorOut.assign(length, ofPoint());
        for(int i = 0; i < length; i++){
			vectorOut[i].x = vectorIn[i].x;
			vectorOut[i].y = vectorIn[i].y;
		}
    }
