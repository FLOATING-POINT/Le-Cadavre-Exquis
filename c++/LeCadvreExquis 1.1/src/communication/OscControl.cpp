#include "OscControl.h"

 void OscControl::setup(string hostname, int port)
{
    deviceHostname = hostname;
    devicePort = port;

    oscSender.setup( deviceHostname, devicePort);
    cout << endl << "OSC Control : Sender setup on port " << hostname << ":" << port << endl;

    oscReceiver.setup( IN_PORT );
    cout << "OSC Control : Receiver setup on port " << IN_PORT << endl << endl;
}

void OscControl::update()
{
    while( oscReceiver.hasWaitingMessages() )
	{
		ofxOscMessage m;
		oscReceiver.getNextMessage( &m );

		//cout << "OSC Control : Received Message: " << m.getAddress() << endl;

        if(m.getAddress() == "/oscRemote/hello")
        {
            cout << "OSC Control : Sending current values to remote device" << endl;

            ofxOscMessage toleranceMessage;
            toleranceMessage.setAddress("tolerance");
            oscSender.sendMessage( toleranceMessage );

            ofxOscMessage thresholdMessage;
            thresholdMessage.setAddress("/threshold");
            oscSender.sendMessage( thresholdMessage );

            ofxOscMessage cthresholdMessage;
            cthresholdMessage.setAddress("oscRemote/cthreshold");
            oscSender.sendMessage( cthresholdMessage );

            ofxOscMessage scaleMessage;
            scaleMessage.setAddress("/scale");
            oscSender.sendMessage( scaleMessage );
        }

        else if (m.getAddress() == "/oscRemote/tolerance")
        {
            *tolerance = (int)m.getArgAsFloat(0);
        }
        else if (m.getAddress() == "/oscRemote/setup")
        {
            *setupMode = (bool)(m.getArgAsInt32(0));
        }
        else if (m.getAddress() == "/oscRemote/scale")
        {
            *scale = m.getArgAsFloat(0);
        }
        else if (m.getAddress() == "/oscRemote/threshold")
        {
            *threshold = (int)m.getArgAsFloat(0);
        }
        else if (m.getAddress() == "/oscRemote/cthreshold")
        {
            *choreThreshold = (int)m.getArgAsFloat(0);
            ofNotifyEvent(ChoreThresholdChanged, *choreThreshold, this);
        }
        else if (m.getAddress() == "/oscRemote/take")
        {
            ofNotifyEvent(TakePictureAsked, *choreThreshold, this);
        }
        else if (m.getAddress() == "/oscRemote/rdm")
        {
            ofNotifyEvent(RandomPoseAsked, *choreThreshold, this);
        }
        else if (m.getAddress() == "/oscRemote/bg")
        {
            *learnBackground = (bool)(m.getArgAsInt32(0));
        }

	}

}
