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
    
    nStep = INSTALL_XCODE;
    
    ofDirectory testDir("/Applications/Xcode.app");
    if (testDir.exists()){
        cout << "XCODE PRESENT" << endl;
        nStep = INSTALL_GIT;
    }
        
    ofFile testFile("/usr/bin/git");
    if (testFile.exists()){
        cout << "GIT PRESENT" << endl;
        nStep = INSTALL_OF;
    }
    
    mScreen = NULL;
}


//--------------------------------------------------------------
void testApp::update(){
    
    if (mScreen == NULL){
        
        if (nStep == INSTALL_XCODE){
            
        } else if (nStep == INSTALL_GIT){
            
        } else {
            searchForOF();
        }
        
    } else {
        mScreen->update();
    }
    
}

void testApp::searchForOF(){
    //  The XML will store basic information like the OF path
    //
    ofxXmlSettings XML;
    XML.loadFile("config.xml");
    string appToRoot = XML.getValue("appToRoot", "../../../../");
    string ofRoot    = XML.getValue("ofRoot", "../../../");
    string defaultLoc = XML.getValue("defaultNewProjectLocation", "apps/myApps");
    
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
    
    //  Try to search using the appToRoot that by defaul search 4 levels down ( distance from a /bin )
    //
    if ( !isOFFolder( ofFilePath::getAbsolutePath(ofFilePath::join( binPath, appToRoot)) ) ){
        
        //  Keep looping until found a OF path
        //
        while ( !isOFFolder(ofRoot)) {
            
            string command = "";
            ofFileDialogResult res = ofSystemLoadDialog("OF project generator", "choose the folder of your OF install");
            
            string result = res.filePath;
            convertWindowsToUnixPath(result);
            ofRoot = result + "/";
        }
        
    } else {
        ofRoot = ofFilePath::getAbsolutePath( ofFilePath::join( binPath, appToRoot)  );
    }
    XML.setValue("appToRoot", appToRoot);
    XML.setValue("ofRoot", ofRoot );
    XML.setValue("defaultNewProjectLocation", defaultLoc);
    XML.saveFile("config.xml");
    
    // there's some issues internally in OF with non unix paths for OF root
    //
    setOFRoot( ofRoot );
    
    mScreen = new mainScreenOFPlay( ofRoot , defaultLoc, "newProject");
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(230);
    
    if (mScreen != NULL)
        mScreen->draw();
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
    
    if (mScreen != NULL)
        mScreen->mousePressed(mouse);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    if (mScreen != NULL)
        mScreen->resized();
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
            
            if (mScreen != NULL)
                mScreen->loadProject(open);
        }
    }
}
