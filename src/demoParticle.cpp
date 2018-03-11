#include "demoParticle.h"

//------------------------------------------------------------------
demoParticle::demoParticle(){
    attractPoints = NULL;
}

//------------------------------------------------------------------
void demoParticle::setMode(particleMode newMode){
    mode = newMode;
}

//------------------------------------------------------------------
void demoParticle::setAttractPoints( vector <ofPoint> * attract ){
    attractPoints = attract;
}

//------------------------------------------------------------------
void demoParticle::reset(){
    //the unique val allows us to set properties slightly differently for each particle
    uniqueVal = ofRandom(-10000, 10000);
    
    pos.x = ofRandomWidth();
    pos.y = ofRandomHeight();
    pos.z = ofRandom(30.0, 200.0);
    
    vel.x = ofRandom(-3.9, 3.9);
    vel.y = ofRandom(-3.9, 3.9);
    vel.z = ofRandom(-3.9, 3.9);
    
    frc   = ofPoint(0,0,0);
    
    scale = ofRandom(0.5, 1.0);
    
}

//------------------------------------------------------------------
void demoParticle::update(ofPoint hand_pos){
    
    //1 - APPLY THE FORCES BASED ON WHICH MODE WE ARE IN
    
    if( mode == PARTICLE_MODE_ATTRACT ){
        ofPoint attractPt(hand_pos);
        frc = attractPt-pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
        frc.normalize(); //by normalizing we disregard how close the particle is to the attraction point
        
        vel += frc * 0.6; //apply force
    }
    else if( mode == PARTICLE_MODE_REPEL ){
        ofPoint attractPt(hand_pos);
        frc = attractPt-pos;
        
        //let get the distance and only repel points close to the mouse
        float dist = frc.length();
        frc.normalize();
        
        if( dist < 150 ){
            vel += -frc * 0.6; //notice the frc is negative
        }else{
            //if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy.
            frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
            frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
            vel += frc * 0.04;
        }
    }
    
    //2 - UPDATE OUR POSITION
    
    pos += vel*0.2;
    
    
    //3 - (optional) LIMIT THE PARTICLES TO STAY ON SCREEN
    //we could also pass in bounds to check - or alternatively do this at the ofApp level
    if( pos.x > 5000 ){
        pos.x = 5000;
        vel.x *= -1.0;
    }
    else if( pos.x < -5000 ){
        pos.x = -5000;
        vel.x *= -1.0;
    }
    if( pos.y > 5000 ){
        pos.y = 5000;
        vel.y *= -1.0;
    }
    else if( pos.y < 0 ){
        pos.y = 0;
        vel.y *= -1.0;
    }
    if( pos.z > 5000 ){
        pos.z = 5000;
        vel.z *= -1.0;
    }
    else if( pos.z < -5000 ){
        pos.z = 5000;
        vel.z *= -1.0;
    }
    
}

//------------------------------------------------------------------
void demoParticle::draw(){
    
    ofEnableAlphaBlending();
    
    if( mode == PARTICLE_MODE_ATTRACT ){
        ofSetColor(255, 63, 180, 100);
    }
    else if( mode == PARTICLE_MODE_REPEL ){
        ofSetColor(208, 255, 63, 100);
    }
    
    ofDrawSphere(pos.x, pos.y, pos.z, scale * 5.0 );
    
}

