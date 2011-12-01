#include "countDownObject.h"
#include "ofEvents.h"


void countDownObject::setup() {

    hasFinished         = true;
    active              = false;
    countDownDuration   = 5;//10;
    currentCount        = 0;
    startTime           = ofGetElapsedTimef();

	font.loadFont("fonts/HelveticaNeueLTStd-BlkCn.otf",180, true, true);

}
void countDownObject::update() {

    if(active){

        currentCount = (countDownDuration+1) - ((int) ofGetElapsedTimef() - startTime);

        if(currentCount == 0) {
            ofNotifyEvent(finishedEvent, hasFinished, this);
            active          = false;
            currentCount    = 0;
        }
    }
}

void countDownObject::draw() {

   // ofSetColor(105,158,188);

    ofRectangle rect = font.getStringBoundingBox(ofToString(currentCount),0,0);

	if(ofInRange(currentCount, 1,countDownDuration)) {

	    ofSetColor(0,0,0,150);
        ofRect((ofGetWidth() - 350)*.5, (ofGetHeight() - 400)*.5, 350, 250);

	    ofSetColor(255,255,255);
	    font.drawString(ofToString(currentCount), (ofGetWidth()-rect.width)*.5 , ofGetHeight()*.5);
	}

}
void countDownObject::start() {

    active              = true;
    currentCount        = 0;
    startTime           = ofGetElapsedTimef();
}
void countDownObject::stop() {
    active = false;
}
void countDownObject::exit() {
}

