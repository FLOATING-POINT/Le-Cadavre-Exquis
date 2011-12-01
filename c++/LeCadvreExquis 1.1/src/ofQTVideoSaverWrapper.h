#ifndef _QT_VIDEO_SAVER_WRAPPER
#define _QT_VIDEO_SAVER_WRAPPER

#include "ofMain.h"
#include "ofQtVideoSaver.h"
#include "ofImage.h"
#include "ofxXmlSettings.h"

class ofQTVideoSaverWrapper {

	public:

        ofQTVideoSaverWrapper();

		void setup(int vidWidth,int vidHeight,int vidFrameLength ,float vidFrameRate );
		void addFrame(ofImage newImg);
		void draw();

		void keyPressed(int key);
		void record (bool start);
		bool bAmSetupForRecording();

		ofImage             img;

        int                 nthFrameRecord;
        float               vidFrameRate;

		int 				vidOutputWidth;
		int 				vidOutputHeight;

		int                 numRecordedFrames;
		int                 videoFrameLength;
		int                     curVideoNum;

		int                     maxVideos;

		string                  vidNamePrefix;
		string                  vidDir;

        ofQtVideoSaver		    video;
        string                  vidName;
        string                  prevVidName;
        string                  vidNameAndDir;

        ofxXmlSettings          settingsXML;
		string                  message;

};

#endif
