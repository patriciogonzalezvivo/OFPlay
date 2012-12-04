#pragma once

#include "ofMain.h"

#include "ofxXmlSettings.h"

#include "froebelGui.h"
#include "projectGenerator.h"

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
 
    void    loadProject(string _path);
    bool    isAddonCore(string _addonName);
    string  setTarget(int targ);
    void    setStatus(string newStatus);
    void    generateProject();
    
    ofImage         logo;
    
    ofTrueTypeFont  font;
    ofTrueTypeFont  secondFont;

    froebelEditBox  projectName;
    froebelEditBox  projectPath;
    froebelListBox  platformsList;
    froebelListBox  addonsList;
    froebelTextBox  generateButton;
    baseProject     *project;
    
    string          appToRoot;
    string          defaultLoc;
    string          addonsPath;
    
    string          status;
    float           statusSetTime;
    float           statusEnergy;
    
    float           defaultHeight;
    
    
};
