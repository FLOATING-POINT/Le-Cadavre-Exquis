#include "GUI.h"

void GUI::draw(float matchRate, float tolerance)
{
    bandeau.draw(15, 15);

    int appPos = (int)(tolerance*ofGetWidth()*0.01);
    int percentPos = (int)(matchRate*ofGetWidth());

    ofPushMatrix();
    ofTranslate(0, ofGetHeight()-barH-34, 0);

        ofSetColor(65, 0, 190);
        ofFill();

        ofRect(appPos+triangleW, 0, ofGetWidth()-appPos, barH);
        ofTriangle(appPos, 0, appPos+triangleW, 0, appPos+triangleW, barH*0.5);
        ofTriangle(appPos+triangleW, barH*0.5, appPos+triangleW, barH, appPos, barH);

        ofRect(0, 0, percentPos, barH);
        ofTriangle(percentPos, 0, percentPos+triangleW, barH*0.5, percentPos, barH);

        ofSetColor(255, 255, 255);
        ofCircle(appPos+triangleW+70, barH*0.5, 70);

        ofSetColor(65, 0, 190);
        ofNoFill();
        ofEnableSmoothing();
        ofSetLineWidth(7);
        ofSetCircleResolution(32);
        ofCircle(appPos+triangleW+70, barH*0.5, 70);

        ofSetColor(255, 255, 255);

        ofEnableAlphaBlending();
        appPhoto.draw(appPos+triangleW+30, 18);
        ofDisableAlphaBlending();

        char mrChar[3];
        sprintf(mrChar,"%i%c", (int)(matchRate*100), 37);

        ocr.drawString(mrChar, percentPos-170, 75);

    ofPopMatrix();

}
