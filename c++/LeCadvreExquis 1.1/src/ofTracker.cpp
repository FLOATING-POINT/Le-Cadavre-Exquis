#include "ofTracker.h"
#include "ofxOpenCv.h"

//--------------------------------------------------------------
ofTracker::ofTracker(){
}

//--------------------------------------------------------------
void ofTracker::setup(ofVideoGrabber *videoGrabber, int camWidth, int camHeight){


    vidGrabber      = videoGrabber;
    debug           = true;

    cWidth          = camWidth;//720
    cHeight         = camHeight;//576
    threshold       = 60;
    maxBlobs        = 20;//15;
    _blur           = 11;
    _minArea        = 20;
    _maxArea        = int(round((cWidth*cHeight)/3));

    finderScale     = 0.5f;

    finderWidth 	= ceil(int(cWidth * finderScale));
    finderHeight 	= ceil(int(cHeight * finderScale));

    // colorImg.allocate(cWidth,cHeight);
    //colorImgSml.allocate(finderWidth,finderHeight);

	grayImage.allocate(finderWidth,finderHeight);
	grayImagePrev.allocate(finderWidth,finderHeight);
	grayDiff.allocate(finderWidth,finderHeight);

	_scaleX = 1.0f;//float(ofGetScreenWidth())/float(cWidth);
	_scaleY = 1.0f;//float(ofGetScreenHeight())/float(cHeight);

}

//--------------------------------------------------------------
void ofTracker::update(ofxCvGrayscaleImage *newGrayImage){

   //if (vidGrabber->isFrameNew()){

        //colorImg.setFromPixels(vidGrabber->getPixels(), cWidth,cHeight);
        //colorImgSml.scaleIntoMe(colorImg);
        grayImage.setFromPixels(newGrayImage->getPixels(), finderWidth, finderHeight);// = &newGrayImage;
        grayImage.mirror(false, true);
		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayImagePrev, grayImage);
		grayDiff.threshold(threshold);
        grayDiff.mirror(false, true);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours( grayDiff, _minArea, _maxArea, maxBlobs, true);	// find holes

        //updateMotion( vidGrabber->getPixels() );
        //updateVideoMask();

        //record the last frame
        grayImagePrev = grayImage;

	//}

}

void ofTracker::updateVideoMask(){

}

//--------------------------------------------------------------
void ofTracker::draw(){

    int sWidth  = ofGetScreenWidth();
    int sHeight = ofGetScreenHeight();

	// Video Mask

	ofEnableAlphaBlending();
        //maskTexture.draw(0,0,sWidth,sHeight);
        grayDiff.draw(0,0,sWidth,sHeight);
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofTracker::keyPressed  (int key){

    switch (key){

		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
        break;

		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
        break;

        case 'b':
            //grayImagePrev = grayImage;
        break;
	}
}

//--------------------------------------------------------------
ofxCvContourFinder *ofTracker::getContourFinder(){
    return &contourFinder;
}
float ofTracker::getScaleX(){
    return _scaleX;
}
float ofTracker::getScaleY(){
    return _scaleY;
}
int ofTracker::getMaxBlobs(){
    return maxBlobs;
}
