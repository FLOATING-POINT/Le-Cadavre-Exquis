#include "FlashTCPServer.h"

void FlashTCPServer::setup(int port)
{
    ofxTCPServer::setup(port);

    policyFileServer.setup(843);

    //cout << "\nFlashTCPServer Started on port " << port << endl << endl;

    ofAddListener(ofEvents.update, this, &FlashTCPServer::update);
}

void FlashTCPServer::update(ofEventArgs &e)
{
    // Policy files management

    while( clientsReq.size() <= policyFileServer.getNumClients() )
	{
	    clientsReq.push_back( string() );
	    clientsAccepted.push_back( false );
	}

	for(int i = 0; i < policyFileServer.getNumClients(); i++)
	{
	    if(!clientsAccepted[i])
	    {
            char buff[22];
            policyFileServer.receiveRawBytes(i, buff, 22);
            clientsReq[i] += buff;

            int pos = clientsReq[i].find("<policy-file-request/>");

            if( pos != string::npos)
            {
                string crossdomain = "<?xml version=\"1.0\"?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"11999\" /></cross-domain-policy>";
                crossdomain += (char)0;
                policyFileServer.sendRawBytes( i, crossdomain.c_str(), crossdomain.length() );

                cout << "crossdomain sent to client " << i << endl;

                /// TODO : this should replace clientsAccepted
                //policyFileServer.disconnectClient(i);
                //clientsReq.erase(clientsReq.begin()+i);

                clientsAccepted[i] = true;
            }
	    }
	}
}
