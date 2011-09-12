#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    //networkPath             = "//BRENDANOLIVERAP/c/wamp/www/lecadavreexquis.net/";
    networkPath             = "c:/wamp/www/lecadavreexquis.net/";
    //serverPath              = "http://www.lecadavreexquis.net/app/";
    serverPath              = "http://localhost/lecadavreexquis.net/app/";
    remoteCommentsFile      = "videoNarratives.php";
    remoteVideosFile        = "videos.xml";
    curNarrative            = "";
    numNarratives           = 0;
    lastUpdateSecond        = 0;
    isUpdatingVideosXML     = false;

    font.loadFont("fonts/FI_0.TTF",40, true, true);


    //==========================================================================================

    ofAddListener(httpUtilsNarratives.newResponseEvent, this, &testApp::setNarratives);
    httpUtilsNarratives.setTimeoutSeconds(60);
    httpUtilsNarratives.start();

    ofxHttpForm form;
    form.action = serverPath+remoteCommentsFile;
    form.method = OFX_HTTP_GET;
    httpUtilsNarratives.addForm(form);

    //==========================================================================================
     curVideoID  = 0;
    //ofDisableDataPath();
    if( XMLVideos.loadFile(networkPath+remoteVideosFile)){
		printf("%s loaded! \n",remoteVideosFile.c_str());
		setVideosFromXML();
		getNextVideo();
	}else{
		printf("unable to load %s check location \n",remoteVideosFile.c_str());
	}
    //ofEnableDataPath();
   // ofToggleFullscreen();

    //==========================================================================================
    scale = 1.0f;



}

//--------------------------------------------------------------
void testApp::update(){

    if(videoLoaded){

    if(curVideoID % 2 == 0){
        playerTwo.idleMovie();
        playerTwo.nextFrame();

        if(playerTwo.getCurrentFrame() == playerTwo.nFrames-1) getNextVideo();

    }else{
        playerOne.idleMovie();
        playerOne.nextFrame();

        if(playerOne.getCurrentFrame() == playerOne.nFrames-1) getNextVideo();


    }

    float wRatio, hRatio;
    if(curVideoID % 2 == 0){

        wRatio = ofGetWidth() / (float)playerTwo.width;
        hRatio = ofGetHeight() / (float)playerTwo.height;

    } else {

        wRatio = ofGetWidth() / (float)playerOne.width;
        hRatio = ofGetHeight() / (float)playerOne.height;
    }

   // scale = MIN( wRatio, hRatio );
    scale = MAX( wRatio, hRatio );

    int curSecs = ofGetElapsedTimef();

    if(curSecs % 60 == 0) {

        updateVideos();
	}
    }

}

//--------------------------------------------------------------
void testApp::draw(){

    if(videoLoaded){

        if(curVideoID % 2 == 0){
            playerTwo.draw(0,0, playerTwo.width*scale,playerTwo.height*scale );
        }else{
            playerOne.draw(0,0,playerOne.width*scale,playerOne.height*scale );
        }

        font.drawString(curNarrative, (ofGetWidth() - narrativeRect.width)*.5, ofGetHeight() - (narrativeRect.height*1.5));

    }
    //ofDrawBitmapString("FR: "+ofToString(ofGetFrameRate()), 20, 300);


}
//--------------------------------------------------------------
void testApp::setNarratives(ofxHttpResponse & response){

    XMLNarratives.loadFromBuffer(response.responseBody);

    printf(" response.responseBody %s \n",response.responseBody.c_str());

    XMLNarratives.pushTag("videos",0);

    //string narrative    = XMLNarratives.getValue("video:txt","",0);
    numNarratives          = XMLNarratives.getNumTags("video");

    string narrative, vimeoID;

    for( int i = 0; i <numNarratives; i++){

        narrative   = XMLVideos.getValue("video:txt","",i);
        vimeoID     = XMLVideos.getValue("video:vimeoID","",i);

        vector<string> data;
        data.push_back(vimeoID);
        data.push_back(narrative);
        videoNarratives.push_back(data);

    }

}
//--------------------------------------------------------------
void testApp::setCurrentNarrative(){

    curNarrative            = "";

     for( int i = 0; i <numNarratives; i++){

         if( curVimeoID == videoNarratives.at(i).at(0)){
            curNarrative = videoNarratives.at(i).at(1);
         }
     }

     narrativeRect = font.getStringBoundingBox(curNarrative,0,0);

}
//--------------------------------------------------------------
void testApp::setVideosFromXML(){

    printf("setVideosFromXML \n");

    videoPaths.clear();

    XMLVideos.pushTag("container",0);
    numVideos   = XMLVideos.getNumTags("video");

    string path, vimeoID;

    for( int i = 0; i <numVideos; i++){

        path        = XMLVideos.getValue("video:file","",i);
        vimeoID     = XMLVideos.getValue("video:vimeoID","",i);

        vector<string> data;
        data.push_back(path);
        data.push_back(vimeoID);

        videoPaths.push_back(data);

    }

}
//--------------------------------------------------------------
void testApp::getNarrative(){
}
//--------------------------------------------------------------
void testApp::updateVideos(){

    if(!isUpdatingVideosXML){

        lastUpdateSecond    = (int) ofGetElapsedTimef();
        isUpdatingVideosXML = true;

        printf("lastUpdateSecond %i \n",lastUpdateSecond);

        if( XMLVideos.loadFile(networkPath+remoteVideosFile)){
            printf("%s loaded! \n",remoteVideosFile.c_str());
            setVideosFromXML();
        }else{
            printf("unable to load %s check location \n",remoteVideosFile.c_str());
        }
    }

    int curSecs = (int) ofGetElapsedTimef();
    if(curSecs != lastUpdateSecond) {
        printf("reset \n");
        isUpdatingVideosXML = false;
    }
}
//--------------------------------------------------------------
void testApp::getNextVideo(){



    curVideoID++;
    if(curVideoID == numVideos) curVideoID = 0;

    string videoPath    = videoPaths.at(curVideoID).at(0);
    curVimeoID          = videoPaths.at(curVideoID).at(1);

     printf("getNextVideo %s \n",videoPath.c_str());

    if(curVideoID % 2 == 0){
        //ofDisableDataPath();
        //TODO: change this back to a local relative path
            if(playerTwo.loadMovie(networkPath+videoPath)){
                videoLoaded = true;
            }
            //playerOne.closeMovie();
        //ofEnableDataPath();
            playerTwo.play();

    }else{
        //ofDisableDataPath();
            if(playerOne.loadMovie(networkPath+videoPath)){
                videoLoaded = true;
            }
            //playerTwo.closeMovie();
        //ofEnableDataPath();
        playerOne.play();
    }

    setCurrentNarrative();

    printf("curVideoID %i \n",curVideoID);

}
//--------------------------------------------------------------
void testApp::keyPressed(int key){

    switch(key){

        case 'f' :
             ofToggleFullscreen();
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

