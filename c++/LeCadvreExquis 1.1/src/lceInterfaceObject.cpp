#include "lceInterfaceObject.h"
#include "ofEvents.h"


void lceInterfaceObject::setup() {

    hit                 = false;
    intX                = 0;
    intY                = 0;
    lastHitTime         = ofGetElapsedTimef();
    timeBetweenEvents   = 1;// seconds
    alpha               = 1.0f;
    //enabled             = false;

}
void lceInterfaceObject::setImage(string image){
    img.loadImage(image);
    setSize(img.width, img.height);
}
void lceInterfaceObject::setPos(ofPoint pnt){

    intX = pnt.x;
    intY = pnt.y;

    if(hitTest(intX,intY)){
        if(ofGetElapsedTimef() > (lastHitTime+timeBetweenEvents)){
            printf("HIT! \n");
            hit = true;
            ofNotifyEvent(hitEvent, hit, this);
            lastHitTime = ofGetElapsedTimef();
        }
    }

}
void lceInterfaceObject::enable() {

    lastHitTime         = ofGetElapsedTimef();
    enabled             = true;
    alpha               = 1.0f;

}
void lceInterfaceObject::disable() {
    //alpha               = 0.0f;
    enabled             = false;
}
void lceInterfaceObject::update() {

     if(enabled){
            alpha = alpha*1.2;
            if(alpha > 255.0f) alpha = 255.0f;
     } else {
            alpha = alpha*0.8;
            if(alpha < 0.0f) alpha = 0.0f;

     }

}

void lceInterfaceObject::draw() {
    ofSetColor(255,255,255, (int)alpha);
    img.draw(x,y);

}

void lceInterfaceObject::onRollOver(int x, int y) {
    printf("lceInterfaceObject::onRollOver(x: %i, y: %i)\n", x, y);
}

void lceInterfaceObject::onRollOut() {
    printf("lceInterfaceObject::onRollOut()\n");
}

void lceInterfaceObject::onMouseMove(int x, int y){
    printf("lceInterfaceObject::onMouseMove(x: %i, y: %i)\n", x, y);
}

void lceInterfaceObject::onDragOver(int x, int y, int button) {
    printf("lceInterfaceObject::onDragOver(x: %i, y: %i, button: %i)\n", x, y, button);
}

void lceInterfaceObject::onDragOutside(int x, int y, int button) {
    printf("lceInterfaceObject::onDragOutside(x: %i, y: %i, button: %i)\n", x, y, button);
}

void lceInterfaceObject::onPress(int x, int y, int button) {
    printf("lceInterfaceObject::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
}

void lceInterfaceObject::onRelease(int x, int y, int button) {
    printf("lceInterfaceObject::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);
}

void lceInterfaceObject::onReleaseOutside(int x, int y, int button) {
    printf("lceInterfaceObject::onReleaseOutside(x: %i, y: %i, button: %i)\n", x, y, button);
}

void lceInterfaceObject::keyPressed(int key) {
    printf("lceInterfaceObject::keyPressed(key: %i)\n", key);
}

void lceInterfaceObject::keyReleased(int key) {
    printf("lceInterfaceObject::keyReleased(key: %i)\n", key);
}
void lceInterfaceObject::exit() {
    printf("lceInterfaceObject::exit() - goodbye!\n");
}

