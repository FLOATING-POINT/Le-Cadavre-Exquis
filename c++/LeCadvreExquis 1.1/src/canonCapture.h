#ifndef _CANON_CAPTURE
#define _CANON_CAPTURE


#include "ofMain.h"
#include "ofxEOSRemoteCapture.h"
#include "CanonCameraWrapper.h"
#include "ofxImage.h"
#include "ofxVectorMath.h"
#include "ofQTVideoSaverWrapper.h"

#include "ofxHttpUtils.h"

#include "ofEvents.h"
#include "eventsObject.h"
#include "windows.h"


class canonCapture{

	public:
		void setup();
		void update();
		void draw();

		void exit();
		void takePicture();

        void newFileAvailableForDownload(int & i);
        void uploadResponse(ofxHttpResponse & response);
        void doSaveVideo();
        void reset();


		CanonCameraWrapper      remoteCapture;
		ofQTVideoSaverWrapper   videoMaker;

		ofEvent<bool>           photoTakenEvent;
        bool                    photoTaken;

        ofEvent<bool>           videoFileCreatedEvent;
        bool                    videoFileCreated;

		ofImage                 capturedImage;
		ofImage                 resizedImage;
		ofImage                 displayImage;

		int                     numFramesPerFrame; // the number of times a photo is recorded for each frame of the video
		int                     numFramesPerVideo; // the number of times in each video submission

		int                     videoWidth;
		int                     videoHeight;
        int                     numPhotosTaken;
       // string                  imgPaths[10];
       vector<string>           imgPaths;

		int                     numOutputImgsProcessed;
		float                   SLRImageRotation;

		int                     curOutputImgID;

		ofxHttpUtils            httpUtils1;
        string                  upload_url;
        string                  responseStr;
        float                   saveTime;
        float                   saveWaitDuration;
        bool                    isSaving;

};

#endif
