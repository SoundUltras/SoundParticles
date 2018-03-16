#include "ofApp.h"
#include "math.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    
    int num = 5;  //particle's number
    p.assign(num, demoParticle());
    currentMode = PARTICLE_MODE_ATTRACT;
    
    currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to right hand";
    
    head_pos.set(0,170,300);
    head_rot.set(0,0,0);
    handposR.set(-20,120,270);
    handposL.set(20,120,270);
    cameraMode = "kinect";
    
    resetParticles();
    
    //light setup
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    pointLight2.setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    pointLight2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));
    
    pointLight3.setDiffuseColor( ofFloatColor(19.f/255.f,94.f/255.f,77.f/255.f) );
    pointLight3.setSpecularColor( ofFloatColor(18.f/255.f,150.f/255.f,135.f/255.f) );
    
    //camera
    cam1.setPosition(0, 170, 300);  //eye
    cam1.setFov(160.0f);
    cam2.setPosition(0,0,0);  //kinect
    cam2.lookAt(ofVec3f(0,0,300),ofVec3f(0,1,0));
    cam2.setFov(120.0f);
    
    //OSC setup
    receiver.setup(PORT);
    sender.setup( HOST, PORT2 );

}

//--------------------------------------------------------------
void ofApp::resetParticles(){
    
    attractPoints.clear();
    for(int i = 0; i < 4; i++){
        attractPoints.push_back( ofPoint( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) ) );
    }
    
    attractPointsWithMovement = attractPoints;
    
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].setMode(currentMode);
        p[i].setAttractPoints(&attractPointsWithMovement);;
        p[i].reset();
    }	
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //OSC Receive
    while( receiver.hasWaitingMessages() ){
        ofxOscMessage m;
        receiver.getNextMessage( m );
        //hand position
        if ( m.getAddress() == "/hand_r" ){
            handposR.x = m.getArgAsFloat( 0 ) * 100;
            handposR.y = m.getArgAsFloat( 1 ) * 100;
            handposR.z = m.getArgAsFloat( 2 ) * 100;
        }
        if ( m.getAddress() == "/hand_l" ){
            handposL.x = m.getArgAsFloat( 0 ) * 100;
            handposL.y = m.getArgAsFloat( 1 ) * 100;
            handposL.z = m.getArgAsFloat( 2 ) * 100;
        }
        //head rotate
        else if ( m.getAddress() == "/head_rot" ) {
            head_rot.x = m.getArgAsFloat( 0 ) ;
            head_rot.y = m.getArgAsFloat( 1 ) ;
            head_rot.z = m.getArgAsFloat( 2 ) ;
        }
        //head position
        else if ( m.getAddress() == "/head_pos" ) {
            head_pos.x = m.getArgAsFloat( 0 ) * 100 ;
            head_pos.y = m.getArgAsFloat( 1 ) * 100 ;
            head_pos.z = m.getArgAsFloat( 2 ) * 100 ;
        }
    }
    
    //OSC Sender
    ofxOscMessage m1;
    
    //head position
    m1.setAddress( "/head_pos" );
    m1.addFloatArg( head_pos.x / 100 );
    m1.addFloatArg( head_pos.y / 100 );
    m1.addFloatArg( head_pos.z / 100 );
    sender.sendMessage( m1 );
    
    //head rotation
    m1.setAddress( "/head_rot" );
    m1.addFloatArg( head_rot.x );
    m1.addFloatArg( head_rot.y );
    m1.addFloatArg( head_rot.z );
    sender.sendMessage( m1 );
    
    for(unsigned int i = 0; i < p.size(); i++){
    	p[i].setMode(currentMode);
    	p[i].update(handposR);
        
        //particle position
        ofPoint p_hpos = p[i].pos - head_pos;  //head center coordinates
        
        p_hpos2[i].x = sqrt( pow(p_hpos.x,2)+pow(p_hpos.y,2)+pow(p_hpos.z,2) );
        p_hpos2[i].y = atan2(p_hpos.x, p_hpos.z) / M_PI * 180;
        p_hpos2[i].z = atan2( p_hpos.y, sqrt( pow(p_hpos.z,2) + pow(p_hpos.x,2) ) ) / M_PI * 180;
        
        string oscmsg = "particle"+to_string(i);  //daijobu?
        m1.setAddress( oscmsg );
        m1.addFloatArg( p_hpos2[i].x / 100 );
        m1.addFloatArg( p_hpos2[i].y / 100 );
        m1.addFloatArg( p_hpos2[i].z / 100 );
        sender.sendMessage( m1 );

    }

    
    //lets add a bit of movement to the attract points
    for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
        attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
        attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
        attractPointsWithMovement[i].z = attractPoints[i].z + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
    }
    
    //light update
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);
    pointLight3.setPosition(
                            cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.2) * ofGetWidth()
                            );
    
    //camera_target
    target_pos.x = head_pos.x + 5.0 * sin(head_rot.y/180*pi) * cos(head_rot.z/180*pi);
    target_pos.y = head_pos.y + 5.0 * sin(head_rot.y/180*pi) * sin(head_rot.z/180*pi);
    target_pos.z = head_pos.z + 5.0 * cos(head_rot.y/180*pi);
    
    //camera
    cam1.setPosition(head_pos.x, head_pos.y, head_pos.z);
    cam1.lookAt(target_pos, ofVec3f(0,1,0));  //ayashii

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
    
    if(cameraMode == "human"){
        cam1.begin();
    }
    else if(cameraMode == "kinect"){
        cam2.begin();
    }
    
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].draw();
    }
    
    ofSetColor(230);
    ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-2 to change mode.", 10, 20);
    
    //light draw
    ofEnableLighting();
    pointLight.enable();
    pointLight2.enable();
    pointLight3.enable();
    
    //hand draw
    ofSetColor(30, 255, 30);
    handR.setPosition(handposR.x, handposR.y, handposR.z);
    handR.set(5, 10);
    handR.draw();
    ofSetColor(30, 255, 30);
    handL.setPosition(handposL.x, handposL.y, handposL.z);
    handL.set(5, 10);
    handL.draw();
    
    //camera_target draw
    ofSetColor(255, 255, 0);
    cam_target.setPosition(target_pos.x, target_pos.y, target_pos.z);
    cam_target.set(2, 2, 2);
    //cam_target.draw();
    
    //head draw
    ofSetColor(30, 30, 255);
    cam_target.setPosition(head_pos.x, head_pos.y, head_pos.z);
    cam_target.set(10, 10, 10);
    //cam_target.draw();
    
    //display data
    headpos = "headposition : " + ofToString( head_pos.x ) + ", " + ofToString( head_pos.y ) + ", " + ofToString( head_pos.z );
    headrot = "headrotation : " + ofToString( head_rot.x ) + ", " + ofToString( head_rot.y ) + ", " + ofToString( head_rot.z );
    handpos = "handposition : " + ofToString( handposR.x ) + ", " + ofToString( handposR.y ) + ", " + ofToString( handposR.z );

    
    //floor
    ofSetColor(120, 120, 120);
    floor.setPosition(0,0,0);
    floor.set(2000, 1, 2000);
    //floor.draw();
    
    if(cameraMode == "human"){
        cam1.end();
    }
    else if(cameraMode == "kinect"){
        cam2.end();
    }


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if( key == '1'){
        currentMode = PARTICLE_MODE_ATTRACT;
        currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
    }
    if( key == '2'){
        currentMode = PARTICLE_MODE_REPEL;
        currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse";
    }

    if( key == ' ' ){
        resetParticles();
    }
    
    if( key == 'k'){
        cameraMode = "kinect";
    }
    if( key == 'h'){
        cameraMode = "human";
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
