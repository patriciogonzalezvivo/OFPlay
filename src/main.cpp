#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main(  int argc, char *argv[]  ){
    
    
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1024, 640, OF_WINDOW);
    testApp * app = new testApp;
    //app->buildAllExamples = false;
    ofRunApp( app );    
}
