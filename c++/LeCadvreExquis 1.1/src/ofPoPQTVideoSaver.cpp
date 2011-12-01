#include "ofPoPQTVideoSaver.h"

//static bool bRecording = false;

ofPoPQTVideoSaver::ofPoPQTVideoSaver(){

    nthFrameRecord      = 1;
    vidFrameRate        = 25.0f; //
    numRecordedFrames   = 0;
    videoFrameLength    = 25;   // How many frames per video
    curVideoNum         = 0;    // used to increment the current video name
    maxVideos           = 20;   // the number of videos that can be recorded before clean up should start

	vidOutputWidth 		= 640;	// try to grab at this size.
	vidOutputHeight     = 480;
	vidNamePrefix       = "CanonSLRMovie";
    vidDir              = "movies";

	//video.listCodecs();   // you may need to uncomment this to choose the number of the codec for h.264
	video.setCodecType(11); // h.264 may not offer the best performance when saving
	video.setCodecQualityLevel(OF_QT_SAVER_CODEC_QUALITY_NORMAL);

}
void ofPoPQTVideoSaver::setup(int vidWidth, int vidHeight, int vidFrameLength = 25, float vidFrameRate = 25.0f ){

    vidFrameRate        = vidFrameRate;
    videoFrameLength    = vidFrameLength;
    vidOutputWidth 		= vidWidth;
	vidOutputHeight     = vidHeight;

}
void ofPoPQTVideoSaver::addFrame(ofImage newImg){

    // downscale if dims dont match
    if(newImg.width != vidOutputWidth || newImg.height != vidOutputHeight){
        // resize image before output
        newImg.resize(vidOutputWidth, vidOutputHeight);
    }

    video.addFrame(newImg.getPixels(), 1.0f / vidFrameRate);
    numRecordedFrames++;

    if(numRecordedFrames % videoFrameLength == 0 ) {

        numRecordedFrames = 1;
        string vidName = vidDir + "/"+ vidNamePrefix + ofToString(curVideoNum) +".mov";

        printf("save curVideoNum  \n");

        // Save the movie to disk
        video.finishMovie();
        video.setup(vidOutputWidth,vidOutputHeight,vidName);

        curVideoNum++;

        if(curVideoNum > maxVideos) {
            /* clean up code should start to run here*/
            /* this class will infinitly create mov files so you might want to delete some
            after a few have been made
            */
        };


    }

}
bool ofPoPQTVideoSaver::bAmSetupForRecording(){
    return video.bAmSetupForRecording();

}
