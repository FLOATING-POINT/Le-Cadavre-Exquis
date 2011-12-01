#include "FrameSaver.h"

void FrameSaver::init(string location)
{
    currentLocation = location;

    if(dataXML.loadFile("output/data.xml"))
    {
       dataXML.pushTag("data", 0);
       dataXML.pushTag(location, 0);
    }
    else
    {
        cout << "FrameSaver ERROR : data could not be loaded" << endl;
    }

//    sender.setup( HOST, PORT );
}

bool FrameSaver::saveFrame(int frameNum, int scale, string filename)
{
	
    Path picPath;
    picPath.tryParse(ofToDataPath(filename));

    Path outputPath;
	outputPath.tryParse( ofToDataPath("output/" + ofToString(frameNum)) );

    Path outputWebPath;
    outputWebPath.tryParse(ofToDataPath("output_web/" + ofToString(frameNum)));

    try
    {
		File* pic = new File( picPath );
		File* outputDir = new File( outputPath );
		File* outputWebDir = new File( outputWebPath );
		
        outputDir->createDirectory(); //return false and do nothing if directory already exist
        outputWebDir->createDirectory(); //return false and do nothing if directory already exist

        vector<string> content;
        outputDir->list(content);
        int frameId = (int)content.size();

        string path = ofToString(frameNum) + "_" + ofToString(frameId) + "_" + ofToString(scale) +".jpg";

        if( saveToXML(frameNum, scale, ofToString(frameNum)+"/" + path) )
        {			
            string outputFileName  = outputPath.toString() + "/" + path;
			
            pic->copyTo(outputFileName);

            string imgURL = "output/" + ofToString(frameNum) + "/" + path;
            string imgWebURL = "output_web/" + ofToString(frameNum) + "/" + path;
			
            resizedImg.loadImage(imgURL);
            resizedImg.resize(1080, 720);
            resizedImg.saveAsJpg(imgURL, 90);
			
            sendOSCEvent(frameNum, ofToString(frameNum)+"/"+path);

            cout << "FrameSaver : photo successfully saved on disk" << endl;

            cout << "FrameSaver : uploading online copy..." << endl;

            resizedImg.resize(640, 427);
            resizedImg.saveAsJpg(imgWebURL, 75);

            //if( ftpSaver.uploadToFTP(ofToString(frameNum), path) )
            //{
                cout << "FrameSaver : photo successfully saved online" << endl;
                lastFileName = ofToString(frameNum)+"/"+path;
                isLastImageOnline = true;
            //}
            //else isLastImageOnline = false;
			
            return true;
        }
        else return false;
    }
    catch (Poco::Exception& exc)
    {
        cerr << exc.displayText() << endl;
        return false;
    }

}

bool FrameSaver::saveToXML(int frameTime, int scale, string path)
{
    int tagNum = round( (frameTime * 12.f) / 1000.f ); // we divide by the fps to get tagNum because xmlsetting doesn't handle attributes yet

    if( !dataXML.tagExists("frame", tagNum) )
    {
        int frame_id = dataXML.addTag("frame");

        if(tagNum != frame_id)
        {
            cout << "FrameSaver ERROR : xml / frame sync error adding " << path << " " << tagNum << " != " << frame_id << endl;
            return false;
        }
    }

    dataXML.pushTag("frame", tagNum);
        int id = dataXML.addTag("photo");
        dataXML.setValue("photo", path, id);
    dataXML.popTag();

    dataXML.saveFile("output/data.xml");

    return true;
}


void FrameSaver::sendOSCEvent(int frameNum, string path)
{
    //ofxOscMessage m;
    //m.setAddress("/pop/newpic");
    //m.addIntArg(frameNum);
    //m.addStringArg(path);

 //   sender.sendMessage(m);
}
