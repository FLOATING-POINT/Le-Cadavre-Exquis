#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "canonCapture.h"
#include "openNIWrapper.h"
#include "lceInterface.h"
#include "ofxSimpleGuiToo.h"


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

		void takePhoto(bool & args);
		void createMovie(bool & args);
		void restart(bool & args);

		void startCountdown(bool & args);

		bool                        debug;


        lceInterface                interfaceObj;
        canonCapture                capture;
        openNIWrapper               tracker;

};

#endif
