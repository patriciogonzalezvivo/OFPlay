#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofSetWindowTitle( "OFPlay" );
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
    ofSetWindowShape(900,500);
    ofBackground(230);
//    ofSetDataPathRoot("../Resources/");
    
    ofxXmlSettings XML;
    XML.loadFile("projectGeneratorSettings.xml");
    string appToRoot = XML.getValue("appToRoot", "../../../../");
    string defaultLoc = XML.getValue("defaultNewProjectLocation", "apps");
    
    //-------------------------------------
    // calculate the bin path (../../../ on osx) and the sketch path (bin -> root - > defaultLoc)
    //-------------------------------------
    // if appToRoot is wrong, we have alot of issues.  all these paths are used in this project:
    //
#ifdef TARGET_OSX
    mScreen.binPath = ofFilePath::getAbsolutePath(ofFilePath::join(ofFilePath::getCurrentWorkingDirectory(), "../../../"));
#else
    mScreen.binPath = ofFilePath::getCurrentExeDir();
#endif
    
    mScreen.ofRoot = ofFilePath::getAbsolutePath(ofFilePath::join( mScreen.binPath, appToRoot));
    mScreen.addonsPath = ofFilePath::getAbsolutePath(ofFilePath::join( mScreen.ofRoot,"addons"));
    string sketchPath = ofFilePath::getAbsolutePath(ofFilePath::join( mScreen.ofRoot, defaultLoc));
    
    convertWindowsToUnixPath( mScreen.ofRoot );
    convertWindowsToUnixPath( mScreen.addonsPath );
    convertWindowsToUnixPath( sketchPath );
    
    // there's some issues internally in OF with non unix paths for OF root
    //
    setOFRoot( mScreen.ofRoot);
    
    mScreen.setup( sketchPath,"newProject");
}


//--------------------------------------------------------------
void testApp::update(){
    if (bOFFound)
        mScreen.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    if (bOFFound)
        mScreen.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    ofPoint mouse = ofPoint(x, y);
    
    if (bOFFound)
        mScreen.mousePressed(mouse);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    if (bOFFound)
        mScreen.resized();
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    if (dragInfo.files.size() > 1){ 
        
    } else if (dragInfo.files.size() == 1) {
        string open = dragInfo.files[0];
        if (isProjectFolder(open)){
            
            if (bOFFound)
                mScreen.loadProject(open);
        }
    }
}
