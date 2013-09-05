#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main(  int argc, char *argv[]  ){
    ofAppGlutWindow window;
    window.setGlutDisplayString("rgba double samples>=4");
    ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
    testApp * app = new testApp;
    ofRunApp( app );    
}
