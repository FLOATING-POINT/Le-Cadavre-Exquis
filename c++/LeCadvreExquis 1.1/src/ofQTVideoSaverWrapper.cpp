#include "ofQTVideoSaverWrapper.h"


//static bool bRecording = false;

ofQTVideoSaverWrapper::ofQTVideoSaverWrapper(){

    if(settingsXML.loadFile("settings/settings.xml") ){
        message = "settings.xml loaded!";
    }else{
        message = "unable to load mySettings.xml check data/ folder";
    }
    printf("settings %s \n", message.c_str());

    nthFrameRecord      = 1;
    vidFrameRate        = 25.0f; //
    numRecordedFrames   = 0;
    videoFrameLength    = 25;   // How many frames per video
    curVideoNum         = settingsXML.getValue("settings:videoNum", 0);    // used to increment the current video name
    maxVideos           = 200;   // the number of videos that can be recorded before clean up should start

	vidOutputWidth 		= 1920;
	vidOutputHeight     = 1280;
	vidNamePrefix       = "CanonSLRMovie";
    vidDir              = "C:/wamp/www/lecadavreexquis.net/videos/";
    vidName             = vidNamePrefix + ofToString(curVideoNum) +".mov";
	prevVidName         = vidName;
	vidNameAndDir       = vidDir + vidName;

	video.listCodecs();   // you may need to uncomment this to choose the number of the codec for h.264
	video.setCodecType(4); // h.264 may not offer the best performance when saving
	video.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_NORMAL);
    //video.setup(vidOutputWidth,vidOutputHeight,vidNameAndDir, true);

    printf("--setup video %s \n",vidName.c_str());

}
void ofQTVideoSaverWrapper::setup(int vidWidth, int vidHeight, int vidFrameLength, float vidFrameRate ){

    vidFrameRate        = vidFrameRate;
    videoFrameLength    = vidFrameLength;
    vidOutputWidth 		= vidWidth;
	vidOutputHeight     = vidHeight;

   // vidName = vidDir + "/"+ vidNamePrefix + ofToString(curVideoNum) +".mov";
	video.setup(vidOutputWidth,vidOutputHeight,vidNameAndDir, true);


}
void ofQTVideoSaverWrapper::addFrame(ofImage newImg){

    // downscale if dims dont match
    if(newImg.width != vidOutputWidth || newImg.height != vidOutputHeight){
        // resize image before output
        newImg.resize(vidOutputWidth, vidOutputHeight);
    }

    video.addFrame(newImg.getPixels(), 1.0f / vidFrameRate);

    numRecordedFrames++;

   // printf("--numRecordedFrames %i \n",numRecordedFrames);

    if(numRecordedFrames == videoFrameLength) {

        // Save the movie to disk
        video.finishMovie();

        printf("++ save vidName %s \n",vidName.c_str());

        curVideoNum++;
        prevVidName     = vidName;
        vidName         = vidNamePrefix + ofToString(curVideoNum) +".mov";
        vidNameAndDir   = vidDir + vidName;

        video.setup(vidOutputWidth,vidOutputHeight,vidNameAndDir, true);

        settingsXML.setValue("settings:videoNum",curVideoNum,0);
        settingsXML.saveFile("settings/settings.xml");

        numRecordedFrames = 0;

        if(curVideoNum > maxVideos) {
            /* clean up code should start to run here*/
            /* this class will infinitly create mov files so you might want to delete some
            after a few have been made
            */
        };


    }

}
bool ofQTVideoSaverWrapper::bAmSetupForRecording(){
    return video.bAmSetupForRecording();

}
