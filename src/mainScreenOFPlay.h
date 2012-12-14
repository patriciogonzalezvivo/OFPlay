//
//  mainScreen.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/7/12.
//
//

#ifndef MAINSCREEN
#define MAINSCREEN

#include "ofMain.h"

#include "froebelGui.h"
#include "projectGenerator.h"

class mainScreenOFPlay {
public:
    
    mainScreenOFPlay();
    
    void    setup(string _ofpath, string _path, string _name );
    void    update();
    void    draw();

    void    keyPressed(int key);
    void    mousePressed(ofPoint _mouse);
    void    resized();

    //  GUI
    //
    ofImage         logo;
    ofTrueTypeFont  font;
    float           defaultHeight;
    froebelTab      tab;

    //  Basic Paths
    //
    string          ofRoot;
    string          addonsPath;
    string          sketchPath;

    //  Project Generator
    //
    void            loadAddons();
    void            loadFolder(string _path);
    void            loadProject(string _path);
//    void            checkProjectState();

    void            pathChange(string &_path);
    void            nameChange(string &_path);

    string          setTarget(int targ);
    void            generateProject();

    froebelEditBox  projectName;
    froebelListBox  projectPath;
    froebelListBox  platformsList;
    froebelListBox  addonsList;

    froebelShapeButton  button;

    baseProject     *project;

    //  Status
    //
    void            setStatus(string newStatus);
    float           statusSetTime, statusEnergy;
    string          status;
};

#endif
