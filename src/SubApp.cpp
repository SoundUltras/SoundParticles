#include "SubApp.h"

void SubApp::setup(){
    
    ofBackground(0,0,0);
    
}

void SubApp::update(){
    
}

void SubApp::draw(){
    
    ofSetColor(225, 225, 225);
    ofDrawBitmapString( displaymsg->currentModeStr + "  1 or 2", 0, 20);
    ofDrawBitmapString( displaymsg->cameraMode + "  k or h", 0, 40);
    ofDrawBitmapString( displaymsg->headpos, 0, 60);
    ofDrawBitmapString( displaymsg->headrot, 0, 80);
    ofDrawBitmapString( displaymsg->handpos, 0, 100);
    
}
