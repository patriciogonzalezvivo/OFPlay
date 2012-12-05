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
 
    //  GUI
    //
    ofImage         logo;
    ofTrueTypeFont  font;
    float           defaultHeight;
    
    //  Basic Paths
    //
    string          binPath;
    string          ofRoot;
//    string          appToRoot;
    string          defaultLoc;
    string          addonsPath;
    
    //  Project Generator
    //
    void            loadAddons();
    void            loadFolder(string _path);
    void            loadProject(string _path);
    
    string          setTarget(int targ);
    void            generateProject();
    
    bool            isAddonCore(string _addonName);
    
    froebelEditBox  projectName;
    froebelListBox  projectPath;
    froebelListBox  platformsList;
    froebelListBox  addonsList;
    froebelTextBox  generateButton;
    
    baseProject     *project;
    
    //  Status
    //
    void            setStatus(string newStatus);
    float           statusSetTime, statusEnergy;
    string          status;
};
