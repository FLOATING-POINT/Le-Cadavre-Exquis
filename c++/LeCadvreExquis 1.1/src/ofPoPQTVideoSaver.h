#ifndef _QT_POP_VIDEO_SAVER
#define _QT_POP_VIDEO_SAVER

#include "ofMain.h"
#include "ofQtVideoSaver.h"
#include "ofImage.h"

class ofPoPQTVideoSaver {

	public:

        ofPoPQTVideoSaver::ofPoPQTVideoSaver();

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
		int                 curVideoNum;

		int                 maxVideos;

		string              vidNamePrefix;
		string              vidDir;

        ofQtVideoSaver		video;

};

#endif
