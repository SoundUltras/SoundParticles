#include "ofMain.h"
#include "ofApp.h"
#include "SubApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    
    ofGLFWWindowSettings settings;
    
    //main window
    settings.width = 1280;
    settings.height = 720;
    settings.setPosition(ofVec2f(0,0));
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
    //sub window
    settings.width = 1280;
    settings.height = 100;
    settings.setPosition(ofVec2f(0,ofGetHeight()+80));
    shared_ptr<ofAppBaseWindow> subWindow = ofCreateWindow(settings);
    
    //app generate
    shared_ptr<ofApp> mainApp(new ofApp);
    shared_ptr<SubApp> subApp(new SubApp);
    subApp->displaymsg = mainApp;
    
    ofRunApp(subWindow, subApp);
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();

    
}
