#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxHttpUtils.h"
#include "ofxXmlSettings.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void setNarratives(ofxHttpResponse & response);
		void setVideosFromXML();
		void getNextVideo();
		void getNarrative();
		void setCurrentNarrative();
		void updateVideos();


		float                           scale;
		int                             lastUpdateSecond;
        bool                            isUpdatingVideosXML;

		ofVideoPlayer                   playerOne;
		ofVideoPlayer                   playerTwo;

		vector<vector<string> >         videoPaths;
		vector<vector<string> >         videoNarratives;
		bool                            videoLoaded;

		ofxHttpUtils                    httpUtilsNarratives;
		ofxXmlSettings                  XMLNarratives;

		ofxHttpUtils                    httpUtilsVideos;
		ofxXmlSettings                  XMLVideos;
		int                             numVideos;
		int                             numNarratives;
		int                             curVideoID;
		string                          curVimeoID;

		string                          networkPath;
		string                          serverPath;
		string                          remoteCommentsFile;
		string                          remoteVideosFile;

		string                          curNarrative;
        ofRectangle                     narrativeRect;
        ofTrueTypeFont                  font;



};

#endif
