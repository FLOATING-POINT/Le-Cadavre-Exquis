#include "imagesPlayback.h"


//--------------------------------------------------------------
void imagesPlayback::setup(canonCapture &captureObj ){

    imagePaths      = &captureObj.imgPaths;
    playbackRate    = 30.0f;
    capture         = &captureObj;
    curImgID        = 0;
    curFrame        = 0;
    systemStartFrame = 0;

}

//--------------------------------------------------------------
void imagesPlayback::update(){

    int curSystemFrame  = ofGetFrameNum();
    int curLoopFrame    = curSystemFrame - systemStartFrame;

    //printf(" curLoopFrame %i \n",curLoopFrame);

    if(curLoopFrame % 10 == 0) {
        curFrame++;
        if(curImgID < 10) img.loadImage(imagePaths->at(curImgID).c_str());
        //if(curImgID < 10) printf("imagesPlayback curFrame %i curImgID %i img %s \n",curFrame,curImgID,imagePaths->at(curImgID).c_str());

        curImgID++;

    }

    if(curImgID == 10){
        complete = true;
        ofNotifyEvent(completeEvent, complete, this);

    }

}

//--------------------------------------------------------------
void imagesPlayback::draw(){
    img.draw(capture->videoWidth,0,capture->videoWidth*-1, capture->videoHeight);
}

//--------------------------------------------------------------
void imagesPlayback::reset(){

    systemStartFrame    = ofGetFrameNum();
    curImgID            = 0;
    curFrame            = 0;
   if(imagePaths->size()>0) img.loadImage(imagePaths->at(curImgID).c_str());

    //printf("systemStartFrame %i \n",systemStartFrame);


}

