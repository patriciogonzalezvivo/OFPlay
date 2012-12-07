#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofSetWindowShape(900,500);
    ofSetWindowTitle( "OFPlay" );
//    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetDataPathRoot("../Resources/");
    
    ofxXmlSettings XML;
    XML.loadFile("projectGeneratorSettings.xml");
    
    appToRoot = XML.getValue("appToRoot", "../../../../");
    ofRoot    = XML.getValue("ofRoot", "../../../");
    defaultLoc = XML.getValue("defaultNewProjectLocation", "apps");
    bOFFound = false;
    
    string binPath;
    //-------------------------------------
    // calculate the bin path (../../../ on osx) and the sketch path (bin -> root - > defaultLoc)
    //-------------------------------------
    // if appToRoot is wrong, we have alot of issues.  all these paths are used in this project:
    //
#ifdef TARGET_OSX
    binPath = ofFilePath::getAbsolutePath(ofFilePath::join(ofFilePath::getCurrentWorkingDirectory(), "../../../"));
#else
    binPath = ofFilePath::getCurrentExeDir();
#endif
    
    if ( !isOFFolder( ofFilePath::getAbsolutePath(ofFilePath::join( binPath, appToRoot)) ) ){
        
        while ( !isOFFolder(ofRoot)) {
            
            string command = "";
            ofFileDialogResult res = ofSystemLoadDialog("OF project generator", "choose the folder of your OF install");
            
            string result = res.filePath;
            convertWindowsToUnixPath(result);
            ofRoot = result;
            cout << ofRoot << endl;
        }
        
    } else {
        ofRoot = ofFilePath::getAbsolutePath( ofFilePath::join( binPath, appToRoot)  );
    }
    
    XML.setValue("ofRoot", ofRoot );
    XML.saveFile("projectGeneratorSettings.xml");
    
    addonsPath = ofFilePath::getAbsolutePath(ofFilePath::join( ofRoot, "addons"));
    sketchPath = ofFilePath::getAbsolutePath(ofFilePath::join( ofRoot, defaultLoc));
    
    convertWindowsToUnixPath( ofRoot );
    convertWindowsToUnixPath( addonsPath );
    convertWindowsToUnixPath( sketchPath );
    
    // there's some issues internally in OF with non unix paths for OF root
    //
    setOFRoot( ofRoot );
    
    mScreen.ofRoot = ofRoot;
    mScreen.addonsPath = addonsPath;
    mScreen.setup( sketchPath, "newProject");
    bOFFound = true;
}


//--------------------------------------------------------------
void testApp::update(){
    if (bOFFound)
        mScreen.update();
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(230);
    
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
