#pragma once

#include "ofMain.h"

#include "ofxXmlSettings.h"

#include "TextSequencer.h"
#include "mainScreenOFPlay.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
 
    void searchForOF();
    
    enum { INSTALL_OF, INSTALL_GIT, INSTALL_XCODE, OFPLAY_INTRO };
    int  nStep;
    
    ofImage             logo;
    ofTrueTypeFont      font;
    
    TextSequencer       textSeq;
    
    mainScreenOFPlay    *mScreen;
    
    bool    bXcodeInstalled;
};
