#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#include "demoParticle.h"

#define HOST "192.168.0.4"
#define PORT 1234
#define PORT2 7777

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void resetParticles();
    
        ofLight pointLight;
        ofLight pointLight2;
        ofLight pointLight3;
    
        particleMode currentMode;
        string currentModeStr;
    
        vector <demoParticle> p;
        vector <ofPoint> attractPoints;
        vector <ofPoint> attractPointsWithMovement;
    
        ofPoint target_pos, head_rot, head_pos, handposR, handposL;
    
        ofPoint p_hpos2[5];  //cart2sph
    
        ofCamera cam1, cam2;
    
        ofBoxPrimitive cam_target, headcam;
        ofSpherePrimitive handL,handR, head;
    
        string headpos, handpos, headrot;
    
        string cameraMode;
    
    private:
        ofxOscReceiver receiver;
        ofxOscSender sender;


    
		
};
