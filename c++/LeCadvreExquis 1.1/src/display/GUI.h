#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "ofMain.h"

/**
* GUI define the Graphical User Interface of the installation
*/

class GUI
{
    public :

        GUI()
        {
            appPhoto.loadImage("assets/app_photo.png");
            bandeau.loadImage("assets/pop.jpg");
            ocr.loadFont("assets/ocr_a_extended.ttf", 61, true, true);
        };
        ~GUI(){};

        void draw(float matchRate, float tolerance);

    private :

        static const int        triangleW = 50;
        static const int        barH = 100;

        ofImage                 appPhoto;
        ofImage                 bandeau;
        ofTrueTypeFont          ocr;
};

#endif // GUI_H_INCLUDED
