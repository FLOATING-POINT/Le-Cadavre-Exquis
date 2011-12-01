#ifndef OSCCONTROL_H_INCLUDED
#define OSCCONTROL_H_INCLUDED

#include "ofxOscReceiver.h"
#include "ofxOscSender.h"
#include "ofEvents.h"

#define IN_PORT        12345

/**
* OSC Control let the installation be controlled by external computer/devices
*/

class OscControl
{
    public :

        OscControl() {};
        ~OscControl() {};

        void update();
        void setup(string hostname, int port);

        int*    tolerance;
        int*    threshold;
        int*    choreThreshold;
        float*  scale;
        bool*   setupMode;
        bool*   learnBackground;

        ofEvent<int> ChoreThresholdChanged;
        ofEvent<int> TakePictureAsked;
        ofEvent<int> RandomPoseAsked;

    private :

        ofxOscReceiver oscReceiver;
        ofxOscSender oscSender;

        string deviceHostname;
        int    devicePort;

};

#endif // OSCCONTROL_H_INCLUDED
