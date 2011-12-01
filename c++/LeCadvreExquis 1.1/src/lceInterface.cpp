#include "lceInterface.h"

//--------------------------------------------------------------

void lceInterface::setup(canonCapture &captureObj){

    imgsPlayback.setup(captureObj);

    capture = &captureObj;

    section             = 0;
    maxSections         = 11;
    numPhotosTaken      = 0;
    maxPhotos           = 10;
    offsetYSkeletal    = 0.0f;


    // -----------------------------------------------------------------

    ins1_img.loadImage("interfaceImages/screen1.png");
    ins2_img.loadImage("interfaceImages/screen2.png");
    ins3_img.loadImage("interfaceImages/screen3.png");
    ins4_img.loadImage("interfaceImages/screen4.png");
    ins5_img.loadImage("interfaceImages/screen5.png");
    ins6_img.loadImage("interfaceImages/screen6.png");
    ins7_img.loadImage("interfaceImages/screen7.png");

    bgImg.loadImage("interfaceImages/background.jpg");

    // -----------------------------------------------------------------

    startBtnIntObj.setImage("interfaceImages/startBtn.png");
    startBtnIntObj.x    = 50;
    startBtnIntObj.enable();
    startBtnIntObj.enableAppEvents();

    // -----------------------------------------------------------------

    viewBtnIntObj.setImage("interfaceImages/viewBtn.png");
    viewBtnIntObj.disable();
    viewBtnIntObj.x    = 50;

    // -----------------------------------------------------------------

    watchBtnIntObj.setImage("interfaceImages/watchBtn.png");
    watchBtnIntObj.disable();

    // -----------------------------------------------------------------

    beginBtnIntObj.setImage("interfaceImages/beginBtn.png");
    beginBtnIntObj.disable();

    // -----------------------------------------------------------------

    saveBtnIntObj.setImage("interfaceImages/saveBtn.png");
    saveBtnIntObj.disable();

     // -----------------------------------------------------------------

    startAgainBtnIntObj.setImage("interfaceImages/startAgainBtn.png");
    startAgainBtnIntObj.disable();

    // -----------------------------------------------------------------

    configBtns();

    helvetica95.loadFont("fonts/calibrib.ttf",34, true, true);
	helvetica95.setLineHeight(20.0f);

    // -----------------------------------------------------------------

    footer.loadImage("interfaceImages/footer.png");

    // -----------------------------------------------------------------

    countDown.setup();
    ofAddListener(countDown.finishedEvent, this, &lceInterface::takePhoto);


    countDown.setup();
    ofAddListener(imgsPlayback.completeEvent, this, &lceInterface::nextStageEvt);

    //--------------------------------------------------------------

    ofAddListener(httpUtils.newResponseEvent, this, &lceInterface::setLastVideoPath);
    httpUtils.setTimeoutSeconds(60);
    httpUtils.start();
    //httpUtils.setVerbose(true) ;

    getLastVideoPath();

    createMovie = false;
    finalStageWaitTime = 3;

    //prevVideoURL = "C:/wamp/www/lecadavreexquis.net/videos/CanonSLRMovie215.mov";

}
//--------------------------------------------------------------
void lceInterface::getLastVideoPath(){

    printf("ofxHttpForm getLastVideoPath \n");

    ofxHttpForm form;
    form.action = "http://localhost/lecadavreexquis.net/getLastVideoPath.php";
    form.method = OFX_HTTP_GET;
    httpUtils.addForm(form);

}
//--------------------------------------------------------------
void lceInterface::setLastVideoPath(ofxHttpResponse & response){

    prevVideoURL = "C:/wamp/www/lecadavreexquis.net/"+response.responseBody;
    printf("prevVideoURL %s \n", prevVideoURL.c_str());
    httpUtils.stop();

}
//--------------------------------------------------------------
void lceInterface::update(){

    startBtnIntObj.update();
    viewBtnIntObj.update();
    beginBtnIntObj.update();
    saveBtnIntObj.update();
    startAgainBtnIntObj.update();

    switch(section){

        case 0:
            startBtnIntObj.x    = (ofGetWidth()-startBtnIntObj.width)*.5;

        break;

        case 1:
            viewBtnIntObj.x    = (ofGetWidth()-viewBtnIntObj.width)*.5;

        break;

        case 2:

            prevVideo.idleMovie();
            prevVideo.nextFrame();

           // printf(" curFrame %i nFrames %i \n",prevVideo.getCurrentFrame(),prevVideo.nFrames);

            if(prevVideo.getCurrentFrame() == prevVideo.nFrames-1) {
                nextStage();
            }

        break;

        case 3:
            watchBtnIntObj.x = ((ofGetWidth()*.5)-watchBtnIntObj.width)*.5;
            beginBtnIntObj.x = (ofGetWidth()*.5) + ((ofGetWidth()*.5)-beginBtnIntObj.width)*.5;
        break;

        case 4:
            startBtnIntObj.x = (ofGetWidth()-startBtnIntObj.width)*.5;
        break;

        case 5:
            countDown.update();
            numPhotosInfo       = "Frames taken: "+ofToString(numPhotosTaken)+"/"+ofToString(maxPhotos);
            numPhotosInfoRect   = helvetica95.getStringBoundingBox(numPhotosInfo,0,0);
            numPhotosX          = ofGetWidth()-(numPhotosInfoRect.width+150);
            numPhotosY          = ofGetHeight() - 50;
        break;

        case 6:
            watchBtnIntObj.x    = (ofGetWidth()- watchBtnIntObj.width)*.5;
        break;

        case 7:
            imgsPlayback.update();
        break;

        case 8:

            saveBtnIntObj.x         = ((ofGetWidth()*.5)-saveBtnIntObj.width)*.5;
            watchBtnIntObj.x        = (ofGetWidth()- watchBtnIntObj.width)*.5;
            startAgainBtnIntObj.x   = (ofGetWidth()*.5) + ((ofGetWidth()*.5)-startAgainBtnIntObj.width)*.5;

        break;

        case 9:

            int curSecs = ofGetElapsedTimef();
            if((curSecs - finalStageStartTime) > finalStageWaitTime && !createMovie){
                nextStage();
            }
        break;
    }


}//--------------------------------------------------------------
//--------------------------------------------------------------
void lceInterface::draw(){

    ofSetColor(255,255,255);

    switch(section){
        case 0:
            insImg = ins1_img;
            //tartBtnIntObj.draw();
        break;

        case 1:
            insImg = ins2_img;
            //viewBtnIntObj.draw();
        break;
        case 2:
            // showing previous video
            insImg.clear();
            ofSetColor(255,255,255);
            prevVideo.draw(0,0);
        break;

        case 3:
            insImg = ins3_img;
        break;

        case 4:
            insImg = ins4_img;
        break;

        case 5:
            // record submission
            insImg.clear();
            countDown.draw();

        break;

        case 6:
            insImg = ins5_img;
        break;

        case 7:
            insImg.clear();
            // review submission
            imgsPlayback.draw();
        break;

        case 8:
            // start again or keep?
            insImg = ins6_img;
        break;

        case 9:
            insImg = ins7_img;
            bgImg.draw(0,0);
        break;

        case 10:
            // create the film
            insImg = ins7_img;
            ofSetColor(255,255,255);
            bgImg.draw(0,0);

        break;
    }

    ofSetColor(255,255,255);
    insImg.draw((ofGetWidth()-insImg.width)*.5, (ofGetHeight() - insImg.height)*.5);

    ofSetColor(0,0,0,150);
    ofRect(0, ofGetHeight() - 130, ofGetWidth(), 130);


    if(section == 5 ){

        ofSetColor(255,255,255);
        helvetica95.drawString(numPhotosInfo, numPhotosX , numPhotosY );

    }

    ofSetColor(255,255,255);
    footer.draw(0, ofGetHeight()-footer.height);

}
//--------------------------------------------------------------
void lceInterface::takePhoto(bool & args){

    ofNotifyEvent(takePhotoEvent, doTakePhoto, this);
    numPhotosTaken++;

}
//--------------------------------------------------------------
void lceInterface::startCountdown(){

    if(numPhotosTaken < maxPhotos){
        countDown.start();
    } else {
        numPhotosTaken = 0;
        nextStage();
    }
}
//--------------------------------------------------------------
void lceInterface::setPos(ofPoint pnt){

    //printf("lceInterface::setPos \n");
    pnt.y += offsetYSkeletal;

     switch(section){
        case 0:
            startBtnIntObj.setPos(pnt);
        break;
        case 1:
            viewBtnIntObj.setPos(pnt);
        break;
        case 2:
            // view the prev submission
        break;
        case 3:
            beginBtnIntObj.setPos(pnt);
            watchBtnIntObj.setPos(pnt);
        break;
        case 4:
            startBtnIntObj.setPos(pnt);
        break;

        case 5:
        break;

        case 6:
            watchBtnIntObj.setPos(pnt);
        break;

        case 7:
        break;

        case 8:
            saveBtnIntObj.setPos(pnt);
            watchBtnIntObj.setPos(pnt);
            startAgainBtnIntObj.setPos(pnt);
        break;

        case 9:
        break;


    }

}
//--------------------------------------------------------------
void lceInterface::nextStageEvt(bool & args){

    nextStage();
    printf("nextStageEvt section %i \n",section);


}
//--------------------------------------------------------------
void lceInterface::nextStage(){

    section++;

    switch(section){

        case 2:
            loadLastVideo();
            prevVideo.setFrame(1);
            prevVideo.play();

        break;

        case 10:
            createMovie = true;
            ofNotifyEvent(createMovieEvent, createMovie, this);

        break;
    }

    if(section == maxSections) section = 0;
    configBtns();

}
//--------------------------------------------------------------
void lceInterface::prevStageEvt(bool & args){

    prevStage();
}
//--------------------------------------------------------------
void lceInterface::startAgain(bool & args){

    //printf("startAgain section %i \n",section);

    saveBtnIntObj.disable();
    saveBtnIntObj.disableAppEvents();
    ofRemoveListener(saveBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);

    startAgainBtnIntObj.disable();
    startAgainBtnIntObj.disableAppEvents();
    ofRemoveListener(startAgainBtnIntObj.hitEvent, this, &lceInterface::startAgain);

    section = 4;
    configBtns();

    capture->reset();

    imgsPlayback.reset();



}
//--------------------------------------------------------------
void lceInterface::prevStage(){

    section--;

    printf("prevStage section %i \n",section);

    configBtns();

    switch(section){
        case 2:
            prevVideo.play();
        break;

        case 7:
            imgsPlayback.reset();
        break;
    }

}
//--------------------------------------------------------------
void lceInterface::restart(){

    //printf("restart section \n");

    section = 0;
    configBtns();
    createMovie = false;
    //getLastVideoPath();
    loadLastVideo();

}
//--------------------------------------------------------------
void lceInterface::loadLastVideo(){

    ofDisableDataPath();
        prevVideo.loadMovie(prevVideoURL);
        printf("loadLastVideo prevVideoURL %s \n",prevVideoURL.c_str());
    ofEnableDataPath();

}
//--------------------------------------------------------------
void lceInterface::configBtns(){

    switch(section){
        case 0:
            startBtnIntObj.enableAppEvents();
            startBtnIntObj.enable();
            ofAddListener(startBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);
        break;

        case 1:

            startBtnIntObj.disableAppEvents();
            startBtnIntObj.disable();
            ofRemoveListener(startBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);

            viewBtnIntObj.enableAppEvents();
            viewBtnIntObj.enable();
            ofRemoveListener(startBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);
            ofAddListener(viewBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);


        break;
        case 2:

            prevVideo.setFrame(1);
            prevVideo.play();

            viewBtnIntObj.disableAppEvents();
            viewBtnIntObj.disable();
            ofRemoveListener(viewBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);

            beginBtnIntObj.disableAppEvents();
            beginBtnIntObj.disable();
            ofRemoveListener(beginBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);

            watchBtnIntObj.disableAppEvents();
            watchBtnIntObj.disable();
            ofRemoveListener(watchBtnIntObj.hitEvent, this, &lceInterface::prevStageEvt);


        break;

        case 3:

            beginBtnIntObj.enableAppEvents();
            beginBtnIntObj.enable();
            ofAddListener(beginBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);

            watchBtnIntObj.enableAppEvents();
            watchBtnIntObj.enable();
            ofAddListener(watchBtnIntObj.hitEvent, this, &lceInterface::prevStageEvt);

        break;

        case 4:

            startBtnIntObj.enableAppEvents();
            startBtnIntObj.enable();
            ofAddListener(startBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);

            beginBtnIntObj.disableAppEvents();
            beginBtnIntObj.disable();
            ofRemoveListener(beginBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);

            watchBtnIntObj.disableAppEvents();
            watchBtnIntObj.disable();
            ofRemoveListener(watchBtnIntObj.hitEvent, this, &lceInterface::prevStageEvt);

        break;

         case 5:

            startBtnIntObj.disableAppEvents();
            startBtnIntObj.disable();
            ofRemoveListener(startBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);

            countDown.start();

         break;

         case 6:

            watchBtnIntObj.enableAppEvents();
            watchBtnIntObj.enable();
            ofAddListener(watchBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);

         break;

         case 7:

            imgsPlayback.reset();


            watchBtnIntObj.disableAppEvents();
            watchBtnIntObj.disable();
            ofRemoveListener(watchBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);

            saveBtnIntObj.disable();
            saveBtnIntObj.disableAppEvents();

            startAgainBtnIntObj.disable();
            startAgainBtnIntObj.disableAppEvents();

         break;

         case 8:

            saveBtnIntObj.enable();
            saveBtnIntObj.enableAppEvents();
            ofAddListener(saveBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);

            startAgainBtnIntObj.enable();
            startAgainBtnIntObj.enableAppEvents();
            ofAddListener(startAgainBtnIntObj.hitEvent, this, &lceInterface::startAgain);

            watchBtnIntObj.enableAppEvents();
            watchBtnIntObj.enable();
            ofAddListener(watchBtnIntObj.hitEvent, this, &lceInterface::prevStageEvt);

         break;

         case 9:

            saveBtnIntObj.disable();
            saveBtnIntObj.disableAppEvents();
            ofRemoveListener(saveBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);

            startAgainBtnIntObj.disable();
            startAgainBtnIntObj.disableAppEvents();
            ofRemoveListener(startAgainBtnIntObj.hitEvent, this, &lceInterface::startAgain);

            watchBtnIntObj.disableAppEvents();
            watchBtnIntObj.disable();
            ofRemoveListener(watchBtnIntObj.hitEvent, this, &lceInterface::nextStageEvt);

            finalStageStartTime = ofGetElapsedTimef();

         break;


    }



}
//--------------------------------------------------------------
void lceInterface::keyPressed(int key){
}
//--------------------------------------------------------------
void lceInterface::keyReleased(int key){

}
//--------------------------------------------------------------
void lceInterface::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void lceInterface::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void lceInterface::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void lceInterface::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void lceInterface::windowResized(int w, int h){

}

