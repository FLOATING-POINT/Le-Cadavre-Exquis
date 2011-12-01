#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    //TODO: Amend vimeo filenames to include venues

    ofSetFrameRate(30);
    tracker.setup();
    capture.setup();
    interfaceObj.setup(capture);
    //intObj.setSize(200, 100);

    //ofPoint p;
   // p.set(0, 300,0);
    //intObj.x = 50;
    //intObj.y = 200;

    //ofAddListener(intObj.hitEvent, this, &testApp::hitObject);
    ofAddListener(interfaceObj.takePhotoEvent, this, &testApp::takePhoto);
    ofAddListener(interfaceObj.createMovieEvent, this, &testApp::createMovie);
    ofAddListener(capture.photoTakenEvent, this, &testApp::startCountdown);
    ofAddListener(capture.videoFileCreatedEvent, this, &testApp::restart);

    debug = true;

    gui.addToggle("debug", debug).setSize(250,30);
    gui.addSlider("Skeletal Offset Y", tracker.offsetYSkeletalAmt, 1, 150).setSize(250,30);
    gui.loadFromXML();
    gui.show();


    ofToggleFullscreen();


}
//--------------------------------------------------------------
void testApp::update(){

    tracker.debug = debug;

    tracker.update();
    capture.update();
    interfaceObj.update();

    interfaceObj.offsetYSkeletal = tracker.offsetYSkeletal;

    if(interfaceObj.section != 5 || interfaceObj.section != 7){

        interfaceObj.setPos(tracker.rightHandUser1Pnt);
        interfaceObj.setPos(tracker.leftHandUser1Pnt);

        interfaceObj.setPos(tracker.rightHandUser2Pnt);
        interfaceObj.setPos(tracker.leftHandUser2Pnt);

    }



    if(interfaceObj.section == 5){
        tracker.doDrawLimb = false;
    } else {
        tracker.doDrawLimb = true;

    }

    //tracker.doDrawLimb = true;

}//--------------------------------------------------------------
void testApp::draw(){




    if(interfaceObj.section != 9) tracker.draw();
    capture.draw();
    interfaceObj.draw();

    if(debug) {
        gui.draw();
        ofSetHexColor(0xffffff);
        ofDrawBitmapString("Interface stage: "+ofToString(interfaceObj.section), 20, 300);
    }

        //
    /*ofFill();
    ofSetColor(0,0,255);
    ofCircle(tracker.rightHandUser1Pnt.x, tracker.rightHandUser1Pnt.y+tracker.offsetYSkeletal, 25);
    ofCircle(tracker.leftHandUser1Pnt.x, tracker.leftHandUser1Pnt.y+tracker.offsetYSkeletal, 25);

    ofCircle(tracker.rightHandUser2Pnt.x, tracker.rightHandUser2Pnt.y+tracker.offsetYSkeletal, 25);
    ofCircle(tracker.leftHandUser2Pnt.x, tracker.leftHandUser2Pnt.y+tracker.offsetYSkeletal, 25);*/

}
//--------------------------------------------------------------
void testApp::takePhoto(bool & args){
    capture.takePicture();
}
//--------------------------------------------------------------
void testApp::createMovie(bool & args){
    printf("CREATE MOVIE \n");
    capture.doSaveVideo();
}
//--------------------------------------------------------------
void testApp::restart(bool & args){

    interfaceObj.prevVideoURL = "C:/wamp/www/lecadavreexquis.net/videos/"+capture.videoMaker.prevVidName;
    interfaceObj.restart();

}
//--------------------------------------------------------------
void testApp::startCountdown(bool & args){
    interfaceObj.startCountdown();

}
//--------------------------------------------------------------
void testApp::keyPressed(int key){

    switch (key){

	    case 'f':
			ofToggleFullscreen();
		break;

        case '<':
            interfaceObj.prevStage();
        break;

        case '>':
            interfaceObj.nextStage();
        break;

        case 'd':
            debug = !debug;
        break;

    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){


}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

