#ifndef _OPENNI_WRAPPER
#define _OPENNI_WRAPPER

#include "ofMain.h"
#include <XnOpenNI.h>
#include <XnLog.h>
#include <XnCppWrapper.h>

#define SAMPLE_XML_PATH "data/openNI/SamplesConfig.xml"
#define MAX_DEPTH 10000

#define CHECK_RC(rc, what)											\
    if (rc != XN_STATUS_OK)											\
    {																\
        printf("%s failed: %s\n", what, xnGetStatusString(rc));		\
                                                                    \
    }

using namespace xn;

class openNIWrapper : public ofBaseApp{

	public:

        openNIWrapper();
        ~openNIWrapper();

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

        void _AddPlayerEvent(int id);
        void _RemovePlayerEvent(int playerId);
        void _PoseDetectedEvent(int playerId);
        void _CalibrationStartedEvent(int playerId);
        void _CalibrationEndedEvent(int playerId, bool success);

        ofPoint getJoint(int userID, XnSkeletonJoint jointName);
        ofPoint getScreenPoint(ofPoint p);

        void drawLimb(ofPoint a, ofPoint b);

        bool doDrawLimb;

        const static int width = 640;
        const static int height = 480;

        Context * context;
        DepthGenerator depthGenerator;
        ImageGenerator imageGenerator;
        UserGenerator userGenerator;

        ImageMetaData imageMetaData;
        DepthMetaData depthMetaData;
        SceneMetaData sceneMetaData;

        unsigned char * videoPixels;
        unsigned short * depthRawPixels;
        unsigned char * depthPixels;

        ofImage rgbImage, depthImage;

        EnumerationErrors errors;
        XnStatus nRetVal;

        bool                debug;


        /* HANDS */
        ofPoint             rightHandUser1Pnt;
        ofPoint             leftHandUser1Pnt;

        ofPoint             rightHandUser2Pnt;
        ofPoint             leftHandUser2Pnt;

        float               scale;
        float               displayWidth;
        float               displayHeight;
        float               offsetX;
        float               offsetY;
        float               offsetYSkeletal;
        float               offsetYSkeletalAmt;
};

extern openNIWrapper * openNIWrapperPtr;

#endif
