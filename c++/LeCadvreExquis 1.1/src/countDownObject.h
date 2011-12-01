#ifndef _COUNTDOWN_OBJECT
#define _COUNTDOWN_OBJECT

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "countDownObject.h"



class countDownObject {
public:

	void setup();
	void exit();
	void update();
	void draw();
	void start();
	void stop();

	ofEvent<bool>               finishedEvent;
    bool                        hasFinished;
    bool                        active;

    int                         countDownDuration; // seconds
    float                       startTime;
    int                         currentCount;

    ofTrueTypeFont              font;

};

#endif

