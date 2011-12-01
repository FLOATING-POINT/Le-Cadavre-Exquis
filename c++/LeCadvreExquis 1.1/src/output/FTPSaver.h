#ifndef FTPSAVER_H_INCLUDED
#define FTPSAVER_H_INCLUDED

#include "ofUtils.h"

#include "Poco/Exception.h"
#include "Poco/Net/FTPClientSession.h"
#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/URI.h"

#include <iostream>
#include <sstream>

#define FTP_HOST    "kikko.fr"
#define FTP_LOGIN   "pop"
#define FTP_PASS    "peopleonpop"

using namespace Poco::Net;
using namespace Poco;
using namespace std;

/**
* FTP Saver let the installation manage and upload files on a FTP server
*/

class FTPSaver
{

    public :

        FTPSaver() {};
        ~FTPSaver() {};

        bool uploadToFTP(string folder, string fileName);
        bool updateConfig(bool connected, string tcpServerIP, int tcpServerPort);

    private :

        bool updateDataXML(FTPClientSession * session);
        bool directoryExist(FTPClientSession * session, string folder);

};


#endif // FTPSAVER_H_INCLUDED
