#ifndef FRAMESAVER_H_INCLUDED
#define FRAMESAVER_H_INCLUDED

//#ifdef unix
//# include < unistd.h>
//# include < utime.h>
//# include < sys/types.h>
//# include < sys/stat.h>
//#else
//# include < direct.h>
//#endif

#include "ofMain.h"
#include "ofxXmlSettings.h"
//#include "ofxOsc.h"
#include "FTPSaver.h"
#include "../utils/JPGImage.h"

#include "Poco/Exception.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Path.h"
#include "Poco/File.h"

#define HOST "localhost"
//#define PORT 1234

using Poco::Path;
using Poco::DirectoryIterator;
using Poco::File;

class FrameSaver
{
    public :

        FrameSaver() {};
        ~FrameSaver() {};

        void init(string currentLocation);
        bool saveFrame(int frameNum, int scale, string filename);

        string currentLocation;
        JPGImage resizedImg;
        bool isLastImageOnline;
        string lastFileName;

        FTPSaver            ftpSaver;

    private :

        bool saveToXML(int frameNum, int scale, string path);
        void sendOSCEvent(int frameNum, string path);

        ofxXmlSettings      dataXML;
//        ofxOscSender        sender;
};

#endif // FRAMESAVER_H_INCLUDED
