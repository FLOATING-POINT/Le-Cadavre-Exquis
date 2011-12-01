#ifndef FLASHTCPSERVER_H_INCLUDED
#define FLASHTCPSERVER_H_INCLUDED

#include "ofMain.h"
#include "ofxNetwork.h"

/**
* FlashTCPServer let the installation update online flash client
*/

class FlashTCPServer : public ofxTCPServer
{
    public :

        FlashTCPServer() {};
        ~FlashTCPServer(){};

        void setup(int port);
        void update(ofEventArgs &e);

    private :

        ofxTCPServer policyFileServer; // needed by Flash Security Sandbox
        ofxTCPServer flashServer;

        vector <string> clientsReq;
		vector <bool> clientsAccepted;
};

#endif // FLASHTCPSERVER_H_INCLUDED
