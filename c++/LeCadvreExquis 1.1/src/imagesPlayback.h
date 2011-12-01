#ifndef _IMAGES_PLAYBACK
#define _IMAGES_PLAYBACK


#include "ofMain.h"
#include "canonCapture.h"

class imagesPlayback{

	public:
		void setup(canonCapture &captureObj);
		void update();
		void draw();
		void reset();

		vector<string> *imagePaths;
		canonCapture    *capture;

		float           playbackRate;
		int             numFramesPerFrame;
		int             curFrame;
		int             curImgID;
		int             systemStartFrame;

		ofImage         img;

		ofEvent<bool>           completeEvent;
        bool                    complete;


};

#endif
