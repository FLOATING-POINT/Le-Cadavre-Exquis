#ifndef _LCE_INTERFACE
#define _LCE_INTERFACE

#include "ofMain.h"
#include "lceInterfaceObject.h"
#include "ofEvents.h"
#include "countDownObject.h"
#include "imagesPlayback.h"
#include "ofxHttpUtils.h"

class lceInterface : public ofBaseApp{

	public:

		void setup(canonCapture &captureObj);
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		void nextStage();
		void nextStageEvt(bool & args);
        void prevStage();
        void prevStageEvt(bool & args);
        void startAgain(bool & args);

		void configBtns();

		void setPos(ofPoint pnt);
		void takePhoto(bool & args);
		void startCountdown();

		void setLastVideoPath(ofxHttpResponse & response);
		void getLastVideoPath();
		void restart();
		void loadLastVideo();

        ofImage                     footer;
        int                         section;
        int                         maxSections;
        imagesPlayback              imgsPlayback;

        ofImage                     insImg; // general instruction image
        ofImage                     ins1_img; // welcome
        ofImage                     ins2_img; // your about to watch prev submission
        ofImage                     ins3_img; // watch again?
        ofImage                     ins4_img; // get started?
        ofImage                     ins5_img; // number of frames, 10 seconds between each frame
        ofImage                     ins6_img; // keep your submission or start again?
        ofImage                     ins7_img; //  your film has been added to the story, go visit the website
        ofImage                     bgImg; //  your film has been added to the story, go visit the website

        canonCapture                *capture;

        lceInterfaceObject          startBtnIntObj;
        lceInterfaceObject          viewBtnIntObj;
        lceInterfaceObject          watchBtnIntObj;
        lceInterfaceObject          beginBtnIntObj;
        lceInterfaceObject          saveBtnIntObj;
        lceInterfaceObject          startAgainBtnIntObj;

        ofVideoPlayer               prevVideo;
        string                      prevVideoURL;

        ofEvent<bool>               takePhotoEvent;
        bool                        doTakePhoto;

        ofEvent<bool>               createMovieEvent;
        bool                        createMovie;

        countDownObject             countDown;

        int                         numPhotosTaken;
        int                         maxPhotos;

        ofTrueTypeFont              helvetica95;
        int                         numPhotosX, numPhotosY;
        string                      numPhotosInfo;
        ofRectangle                 numPhotosInfoRect;

        ofxHttpUtils                httpUtils;

        int                         finalStageStartTime;
        int                         finalStageWaitTime;

        float                       offsetYSkeletal;

};

#endif
