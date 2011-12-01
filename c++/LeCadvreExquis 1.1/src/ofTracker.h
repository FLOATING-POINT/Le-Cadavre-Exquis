#ifndef _OF_TRACKER
#define _OF_TRACKER

#include "ofMain.h"
#include "ofxOpenCv.h"

#define _USE_LIVE_VIDEO		// use a live camera

class ofTracker {

	public:

		ofTracker();
		void setup(ofVideoGrabber *vidGrabber, int camWidth, int camHeight);
		void update(ofxCvGrayscaleImage *newGrayImage);
		void draw();

		void keyPressed  (int key);

		void drawMotion();

		void updateMotion( unsigned char *pixels );
		void updateVideoMask();
		float getScaleX();
		float getScaleY();
		int getMaxBlobs();

		ofxCvContourFinder *getContourFinder();

		int cWidth;//width of cam
		int cHeight;// height of cam
		int _minArea;
		int _maxArea;

		bool debug;

        #ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		*vidGrabber;
		#else
		  ofVideoPlayer 		vidGrabber;
		#endif

		ofxCvColorImage		    colorImg;
		ofxCvColorImage		    *colorImgSml;
		ofxCvGrayscaleImage     grayDiff;
		ofxCvGrayscaleImage     grayImagePrev;
		ofxCvGrayscaleImage     grayImage;

		ofxCvContourFinder	    contourFinder;
		//ofxCvContourFinder	    contourFinderRef;

		ofxCvFloatImage		    cameraMotionFloatImage;
		ofxCvFloatImage			cameraDiffFloatImage;

		float                   finderScale;
        int                     finderHeight;
        int                     finderWidth;

		float                   cameraMotionFadeAmount;
		float                   _scaleX;
		float                   _scaleY;

		int 				    threshold;
		int                     maxBlobs;
		int                     _blur;
		int                     _videoAlpha;

		ofTexture               maskTexture;
		//ofTexture               glowTexture;
		unsigned char           *maskPixels;
		unsigned char           *glowPixels;
		unsigned char           *videoPixels;
		unsigned char           *motionPixels;



};

#endif
