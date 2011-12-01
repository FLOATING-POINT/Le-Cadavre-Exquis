#include "canonCapture.h"

//--------------------------------------------------------------
void canonCapture::setup(){

    videoWidth              = 1920;
    videoHeight             = 1280;
    numFramesPerFrame       = 10;
    numFramesPerVideo       = 10;
    saveWaitDuration        = 5; // seconds
    saveTime                = ofGetElapsedTimef();
    isSaving                = false;
    numPhotosTaken          = 0;


    remoteCapture.setup(0);
    remoteCapture.setDeleteFromCameraAfterDownload(true);
    remoteCapture.setDownloadPath( ofToDataPath("images"));
    remoteCapture.enableDownloadOnTrigger();
    remoteCapture.openSession();

    ofAddListener(remoteCapture.newIntEvent,this, &canonCapture::newFileAvailableForDownload);

    videoMaker.setup(videoWidth,videoHeight, numFramesPerFrame*numFramesPerVideo, 30.0f);

    //ofSetVerticalSync(true);
    //ofAddListener(httpUtils1.newResponseEvent, this, &canonCapture::uploadResponse);
    //httpUtils1.setVerbose(true) ;
    upload_url              = "http://localhost/lecadavreexquis.net/vimeoUploader.php";
    httpUtils1.setTimeoutSeconds(600);
    httpUtils1.start();

}
//--------------------------------------------------------------
void canonCapture::update(){

    if(ofGetFrameNum() % 1000 == 0) remoteCapture.keepCameraAlive();



}//--------------------------------------------------------------
void canonCapture::draw(){
}
void canonCapture::newFileAvailableForDownload(int & newFileAvailableForDownload){

    string p                    = remoteCapture.getLastImagePath();
    imgPaths.push_back(p);

    numPhotosTaken++;
    printf("=== new File Available Downloaded numPhotosTaken %i \n",numPhotosTaken);

    saveTime = ofGetElapsedTimef();

    ofNotifyEvent(photoTakenEvent, photoTaken, this);

}
void canonCapture::doSaveVideo(){

    isSaving = true;

    printf("CREATING MOVIE \n");
    int timeStart = ofGetElapsedTimeMillis();

    for( int j =0 ; j <numFramesPerFrame; j++){

        int timeStartFrame = ofGetElapsedTimeMillis();

        string imgPath    = imgPaths[j];
        capturedImage.loadImage(imgPath);

        for( int i =0 ; i <numFramesPerFrame; i++){
            videoMaker.addFrame(capturedImage);
        }

        int timeEndFrame    = ofGetElapsedTimeMillis();
        int diffTimeFrame   = timeEndFrame - timeStartFrame;

    }

    int timeEnd    = ofGetElapsedTimeMillis();
    int diffTime   = timeEnd - timeStart;
    //printf("Movie duration: %i \n",diffTime);

    // ============================================================================
    // construct request and do upload
    // ============================================================================
    ofSleepMillis(4500); // wait for the file to be created on the local system

    printf("canon capture httpUtils \n");

    ofxHttpForm form;
    form.action = upload_url;
    form.method = OFX_HTTP_GET;
    string name = videoMaker.prevVidName;
    form.addFormField("videoName",name );
    //httpUtils.setTimeoutSeconds(-1);
    httpUtils1.addForm(form);

    // ============================================================================
    // reset
    // ============================================================================
    numPhotosTaken  = 0;
    isSaving        = false;

    imgPaths.clear();

    ofNotifyEvent(videoFileCreatedEvent, videoFileCreated, this);


}
//--------------------------------------------------------------
void canonCapture::uploadResponse(ofxHttpResponse & response){
    printf("RESPONSE");
	 //responseStr = ofToString(response.status) + ": " + response.responseBody;
	 //printf("UPLOAD RESPONSE: %s \n",responseStr.c_str());
}
//--------------------------------------------------------------
void canonCapture::reset(){
	imgPaths.clear();
}

//--------------------------------------------------------------
void canonCapture::exit(){

}

void canonCapture::takePicture(){

     bool taken  = remoteCapture.takePicture();

}

