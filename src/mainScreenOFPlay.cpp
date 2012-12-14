//
//  mainScreenOFPlay.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/7/12.
//
//

#include "mainScreenOFPlay.h"

mainScreenOFPlay::mainScreenOFPlay(){
    project = NULL;
    statusEnergy = 0;
}

void mainScreenOFPlay::setup( string _ofRoot, string _defaultPath, string _name ){
    
    ofRoot = _ofRoot;
    addonsPath = ofFilePath::getAbsolutePath(ofFilePath::join( ofRoot, "addons"));
    sketchPath = ofFilePath::getAbsolutePath(ofFilePath::join( ofRoot, _defaultPath));
    
    convertWindowsToUnixPath( ofRoot );
    convertWindowsToUnixPath( addonsPath );
    convertWindowsToUnixPath( sketchPath );
    
    //------------------------------------- GUI
    //
    defaultHeight       = 34;
    logo.loadImage("OFPlay.png");
    font.loadFont("Inconsolata.otf", 12, true,false,false,0.5,90);
    
    tab.set(0,0,ofGetWidth(),defaultHeight*2);
    tab.font.loadFont("Inconsolata.otf", 18, true,false,false,0.5,90);
    tab.addElement("MAKE");
    tab.addElement("UPDATE");
    tab.addElement("OPEN");
    
    //  PATH:
    //
    projectPath.x = defaultHeight;
    projectPath.y = tab.height + defaultHeight;
    projectPath.font = &font;
    projectPath.setSizeAndShapes(defaultHeight, 3);
    projectPath.setText( sketchPath );
    projectPath.setPrefix("Path: ");
    projectPath.setDivider("/");
    projectPath.setSubInfo("CHOOSE PATH");
    projectPath.containerBox.bCheckList = false;
    ofAddListener(projectPath.focusLost, this, &mainScreenOFPlay::pathChange);
    
    loadFolder( "apps/" );
    loadFolder( "examples/" );
    loadFolder( "addons/" );
    
    //  NAME:
    //
    projectName.x = defaultHeight;
    projectName.y = projectPath.y + projectPath.height + defaultHeight*0.5;
    projectName.font = &font;
    projectName.setText( _name );
    projectName.setPrefix("Name: ");
    projectName.setSizeAndShapes(defaultHeight, 3);
    projectName.setSubInfo("NEW PROJECT NAME");
    
    projectName.enable();
    ofAddListener(projectName.focusLost, this, &mainScreenOFPlay::nameChange);
    
    //  LOAD PLATFORMS (check if it have the template)
    //
    platformsList.x = defaultHeight;
    platformsList.y = projectName.y + projectName.height + defaultHeight*0.5;
    platformsList.font = &font;
    platformsList.setSizeAndShapes(defaultHeight,3);
    platformsList.setPrefix("Platform: ");
    platformsList.setDivider(", ");
    platformsList.setSubInfo("TARGET PLATFORM");
    platformsList.containerBox.maxHeight = 200;
    
    ofDirectory testDir(ofRoot+"scripts/win_cb");
    if (testDir.exists())
        platformsList.addElement("windows (codeblocks)",ofGetTargetPlatform()==OF_TARGET_WINGCC);
    
    testDir.open(ofRoot+"scripts/vs2010");
    if (testDir.exists())
        platformsList.addElement("windows (visualStudio)", ofGetTargetPlatform()==OF_TARGET_WINVS);
    
    testDir.open(ofRoot+"scripts/linux");
    if (testDir.exists()){
        platformsList.addElement("linux (codeblocks)",ofGetTargetPlatform()==OF_TARGET_LINUX);
        platformsList.addElement("linux64 (codeblocks)",ofGetTargetPlatform()==OF_TARGET_LINUX64);
    }
    
    testDir.open(ofRoot+"scripts/osx");
    if (testDir.exists())
        platformsList.addElement("osx (xcode)",ofGetTargetPlatform()==OF_TARGET_OSX);
    
    testDir.open(ofRoot+"scripts/ios");
    if (testDir.exists())
        platformsList.addElement("ios (xcode)",ofGetTargetPlatform()==OF_TARGET_IPHONE);

    platformsList.setText( platformsList.getSelectedAsString() );
    
    //  LOAD ADDONS
    //
    addonsList.x = defaultHeight;
    addonsList.y = platformsList.y + platformsList.height + defaultHeight*0.5;
    addonsList.maxWidth = 700;
    addonsList.font = &font;
    addonsList.setSizeAndShapes(defaultHeight,3);
    addonsList.setPrefix("Addons: ");
    addonsList.setDivider(", ");
    addonsList.setSubInfo("SELECTED ADDONS");
    addonsList.containerBox.maxHeight = ofGetHeight() - addonsList.y - defaultHeight*3.0;
    loadAddons();
    
    button.setShape(2, 76);
    button.color.set(0.0,0.0);
    button.font = &font;
    button.text = "NONE";
    button.textColor.addState(3);
    button.textColor.addState(2);
    button.textColor.addState(4);
    
//    checkProjectState();
    tab.setElement(0);
}

void mainScreenOFPlay::loadAddons(){
    addonsList.containerBox.clear();
    
    ofDirectory addonsFolder(addonsPath);
    addonsFolder.listDir();
    for(int i=0; i < (int)addonsFolder.size();i++){
    	string addonName = addonsFolder.getName(i);
        
    	if(addonName.find("ofx")==0){
            if (isAddonCore(addonName)){
                addonsList.addElement(addonName,false);
            } else {
                addonsList.addElement(addonName,false,4);
            }
    	}
    }
}

void mainScreenOFPlay::loadFolder(string _path){
    ofDirectory folder( ofFilePath::getAbsolutePath(ofFilePath::join(ofRoot,_path)) );
    
    if (folder.exists()){
        
        //  Create Folder Element
        //
        froebelFolderElement *newFolder = new froebelFolderElement();
        newFolder->font = &font;
        newFolder->setSizeAndShapes(defaultHeight);
        newFolder->setText(_path);
        newFolder->rootPath = ofRoot;
        newFolder->setPrefix("");
        newFolder->fgColor.clear();
        newFolder->fgColor.addState(5);
        newFolder->fgColor.addState(5);
        newFolder->fgColor.addState(9);
        newFolder->bgColor.clear();
        newFolder->bgColor.addState(0);
        newFolder->bgColor.addState(0);
        newFolder->bgColor.addState(5);
        newFolder->containerBox.size = defaultHeight;
        
        newFolder->father = &projectPath.containerBox;
        
        newFolder->update();
        
        //  Added to the list
        //
        projectPath.containerBox.addElement( newFolder );
    }
}

void mainScreenOFPlay::pathChange(string &_path){
    
    string completePath = ofRoot + projectPath.getText();
    
    addonsList.containerBox.reset();
    addonsList.bChange = true;
    
    if (tab.getElementText() == "UPDATE" || tab.getElementText() == "OPEN"){
        if ( isProjectFolder(completePath) ){
            loadProject( completePath );
        } else {
            projectPath.containerBox.reset();
            projectPath.setText(ofRoot + projectPath.getText());
            projectName.setText("newProject");
        }
    }
    
//    checkProjectState();
}

void mainScreenOFPlay::nameChange(string &_path){
    addonsList.containerBox.reset();
    addonsList.bChange = true;
    
//    checkProjectState();
}

void mainScreenOFPlay::loadProject(string _path){
    //  Extract Name and Path
    //
    string folder = "";
    
    extractFolderFromPath(_path,folder);
//    projectPath.containerBox.reset();
    projectPath.setText(_path);
    projectName.setText(folder);
    
    setStatus("Project " + folder + " loaded ");
    
    //  Extracting Addons ( from addons.make)
    //
    addonsList.containerBox.reset();
    
    //  Have addons.make??
    //
    ofFile test;
    bool    isAddons = test.open(_path + "/" + folder + "/addons.make");
    if ( !isAddons )
        return;
    
    //  Add addons
    //
    ifstream fs( (_path + "/" + folder + "/addons.make").c_str());
    int counter = 0;
    string line;
    string addonsAdded = "";
    while(!(fs >> line).fail()){
        
        if ( addonsList.select(line) ){
            if (counter > 0)
                addonsAdded +=", ";
            addonsAdded += line;
        } else {
            cout << "Error: loading " << line << endl;
        }
        counter++;
    }
    fs.seekg(0,ios::beg);
    fs.clear();
    fs.close();
    
    addonsList.setText( addonsList.getSelectedAsString() );
}

string mainScreenOFPlay::setTarget(int targ){
    
    if(project){
		delete project;
	}
    
    string target;
    switch(targ){
        case OF_TARGET_OSX:
            project = new xcodeProject;
            target = "osx";
            break;
        case OF_TARGET_WINGCC:
            project = new CBWinProject;
            target = "win_cb";
            break;
        case OF_TARGET_WINVS:
            project = new visualStudioProject;
            target = "vs2010";
            break;
        case OF_TARGET_IPHONE:
            project = new xcodeProject();
            target = "ios";
            break;
        case OF_TARGET_ANDROID:
            break;
        case OF_TARGET_LINUX:
            project = new CBLinuxProject;
            target = "linux";
            break;
        case OF_TARGET_LINUX64:
            project = new CBLinuxProject;
            target = "linux64";
            break;
    }
    
    project->setup(target);
    return target;
}

void mainScreenOFPlay::setStatus(string newStatus){
    statusEnergy = 1;
    status = newStatus;
    statusSetTime = ofGetElapsedTimef();
}

void mainScreenOFPlay::generateProject(){
    
    vector <int> targetsToMake;
    for(int i = 0; i < platformsList.containerBox.elements.size(); i++){
        if ( platformsList.containerBox.elements[i]->bSelected == true ){
            if (platformsList.containerBox.elements[i]->getText() == "windows (codeblocks)" ){
                targetsToMake.push_back(OF_TARGET_WINGCC);
            } else if (platformsList.containerBox.elements[i]->getText() == "windows (visualStudio)"){
                targetsToMake.push_back(OF_TARGET_WINVS);
            } else if (platformsList.containerBox.elements[i]->getText() == "linux (codeblocks)"){
                targetsToMake.push_back(OF_TARGET_LINUX);
            } else if (platformsList.containerBox.elements[i]->getText() == "linux64 (codeblocks)"){
                targetsToMake.push_back(OF_TARGET_LINUX64);
            } else if (platformsList.containerBox.elements[i]->getText() == "osx (xcode)"){
                targetsToMake.push_back(OF_TARGET_OSX);
            } else if (platformsList.containerBox.elements[i]->getText() == "ios (xcode)"){
                targetsToMake.push_back(OF_TARGET_IPHONE);
            }
        }
    }
    
    cout << targetsToMake.size() << endl;
    
	if( targetsToMake.size() == 0 ){
		cout << "Error: makeNewProjectViaDialog - must specifiy a project to generate " <<endl;
		ofSystemAlertDialog("Error: makeNewProjectViaDialog - must specifiy a project platform to generate");
        return;
	}
    
    if (projectName.getText().size() == 0){
        ofSystemAlertDialog("Error: project must have a name");
        return;
    }
    
    printf("start with project generation \n");
    
    string path = ofFilePath::join(projectPath.getText(), projectName.getText());
    
	for(int i = 0; i < (int)targetsToMake.size(); i++){
		string target = setTarget(targetsToMake[i]);
        
        if(project->create(path)){
            
            vector<string> addons = addonsList.containerBox.getSelected();
            for (int i = 0; i < addons.size(); i++){
                ofAddon addon;
                addon.pathToOF = getOFRelPath(path);
                addon.fromFS(ofFilePath::join(addonsPath, addons[i]),target);
                project->addAddon(addon);
            }
            
            project->save(true);
        }
        
        setStatus("generated: " + projectPath.getText() + "/" + projectName.getText() + " for " + platformsList.containerBox.getSelected()[i]);
	}
    
//    checkProjectState();
    printf("done with project generation \n");
}

void mainScreenOFPlay::update(){
    tab.x = 0;
    tab.y = 0;
    tab.update();
    
    projectPath.x = defaultHeight;
    projectPath.y = tab.y + tab.height + defaultHeight;
    projectPath.update();
    
    ofRectangle prev = projectPath.getBoundingBox();
    
    if ( tab.getElementText() == "MAKE"){
        projectPath.subInfo->setText("CHOOSE A DIRECTORY");
        
        projectName.x = prev.x;
        projectName.y = ofLerp(projectName.y,prev.y + prev.height + defaultHeight*0.5,0.1);
        projectName.update();
        projectName.enable();
        projectName.subInfo->setText("NEW PROJECT NAME");
        prev = projectName.getBoundingBox();
        
        platformsList.x = prev.x;
        platformsList.y = ofLerp(platformsList.y, prev.y + prev.height + defaultHeight*0.5,0.1);
        platformsList.update();
        
        prev = platformsList.getBoundingBox();
        addonsList.x = prev.x;
        addonsList.y = ofLerp(addonsList.y, prev.y + prev.height + defaultHeight*0.5,0.1);
        addonsList.update();
        
    } else if ( tab.getElementText() == "UPDATE"){
        projectPath.subInfo->setText("CHOSE A PROJECT FOLDER OR DRAG ONE");
        
        projectName.x = prev.x;
        projectName.y = ofLerp(projectName.y,prev.y + prev.height + defaultHeight*0.5,0.1);
        projectName.update();
        projectName.disable();
        projectName.subInfo->setText("PROJECT TO BE UPDATE");
        prev = projectName.getBoundingBox();
        
        platformsList.x = prev.x;
        platformsList.y = ofLerp(platformsList.y, prev.y + prev.height + defaultHeight*0.5,0.1);
        platformsList.update();
        
        prev = platformsList.getBoundingBox();
        addonsList.x = prev.x;
        addonsList.y = ofLerp(addonsList.y, prev.y + prev.height + defaultHeight*0.5,0.1);
        addonsList.update();
    } else {
        projectPath.subInfo->setText("PICK A PROJECT");
        
        projectName.x = prev.x;
        projectName.y = ofLerp(projectName.y,prev.y + prev.height + defaultHeight*0.5,0.1);
        projectName.update();
        projectName.disable();
        projectName.subInfo->setText("PROJECT TO BE OPEN");
        prev = projectName.getBoundingBox();
    }
    
    button.text = tab.getElementText();
    button.update();
    
    float diff = ofGetElapsedTimef()- statusSetTime;
    if (diff > 3){
        statusEnergy *= 0.99;;
    }
}

void mainScreenOFPlay::draw(){
    tab.draw();
    
    projectPath.draw();
    projectName.draw();
    
    if ( tab.getElementText() == "MAKE" || tab.getElementText() == "UPDATE"){
        addonsList.draw();
        platformsList.draw();
    }
    
    ofSetColor(255);
    logo.draw(ofGetWidth() - defaultHeight - logo.getWidth(),ofGetHeight() - defaultHeight - logo.getHeight());
    button.draw();
    
    ofFill();
    ofSetColor(0 + 220 * (1-statusEnergy),0 + 220 * (1-statusEnergy),0 + 220 * (1-statusEnergy));
    ofRect(0,ofGetHeight(), ofGetWidth(), -25);
    ofSetColor(255,255,255, 255 * statusEnergy);
    ofDrawBitmapString(status, 10,ofGetHeight()-8);
}

void mainScreenOFPlay::keyPressed(int key){
    if ( key == OF_KEY_RIGHT){
        if (tab.getElementText() == "MAKE"){
            //string completePath = projectPath.getText() + "/" + projectName.getText();
            //projectPath.setText(completePath);
        }
        tab.setNext();
    } else if ( key == OF_KEY_LEFT){
        tab.setPrev();
    }
}

void mainScreenOFPlay::mousePressed(ofPoint _mouse){
    if ( projectPath.checkMousePressed( _mouse ) ){
        
        if (tab.getElementText() == "MAKE"){
            projectPath.bSelected = false;
            
            string command = "";
            ofDirectory dir(ofFilePath::join(getOFRoot(),sketchPath));
            
            if (!dir.exists()){
                dir.create();
            }
            
#ifdef TARGET_WIN32
            ofFileDialogResult res = ofSystemLoadDialog("please select sketch folder", true, windowsFromUnixPath(dir.path()));
#else
            ofFileDialogResult res = ofSystemLoadDialog("please select sketch folder", true, dir.path());
#endif
            if (res.bSuccess){
                string result = res.filePath;
                convertWindowsToUnixPath(result);
                projectPath.setText(result);
                setStatus("path set to: " + result);
            }
        } else {
            projectPath.bSelected   = true;
        }
        
        
        projectName.bSelected   = false;
        platformsList.bSelected = false;
        addonsList.bSelected    = false;
        
    } else if ( addonsList.checkMousePressed(_mouse)){
        projectPath.bSelected   = false;
        projectName.bSelected   = false;
        platformsList.bSelected = false;
        addonsList.bSelected    = true;
    } else if ( platformsList.checkMousePressed(_mouse)){
        projectPath.bSelected   = false;
        projectName.bSelected   = false;
        platformsList.bSelected = true;
        addonsList.bSelected    = false;
    } else if ( button.checkMousePressed(_mouse)){
        projectPath.bSelected   = false;
        projectName.bSelected   = false;
        platformsList.bSelected = false;
        addonsList.bSelected    = false;
        
        if (tab.getElementText() == "MAKE" || tab.getElementText() == "UPDATE"){
            generateProject();
        } else {
            string path = "open " + projectPath.getText() + "/"+ projectName.getText() + "/" +  projectName.getText() + ".xcodeproj";
            system( path.c_str() );
        }
        
    } else if (tab.checkMousePressed(_mouse)){
        platformsList.bSelected = false;
        addonsList.bSelected    = false;
        projectPath.bSelected   = false;
        projectName.bSelected   = false;
    } else {
        platformsList.bSelected = false;
        addonsList.bSelected    = false;
        projectPath.bSelected   = false;
        projectName.bSelected   = false;
    }
}

void mainScreenOFPlay::resized(){
    tab.width = ofGetWidth();
    
    projectPath.subInfo->width = ofGetWidth() - defaultHeight * 2.0;
    
    projectName.subInfo->width = ofGetWidth() - defaultHeight * 2.0;
    
    platformsList.subInfo->width = ofGetWidth() - defaultHeight * 2.0;
    addonsList.subInfo->width = ofGetWidth() - defaultHeight * 2.0;
    
    addonsList.containerBox.maxHeight = ofGetHeight() - addonsList.y - defaultHeight*3.0;
    addonsList.containerBox.adjustShape();
    
    button.x = ofGetWidth() - defaultHeight - logo.getWidth()*0.535 + button.size;
    button.y = ofGetHeight() - defaultHeight - logo.getHeight() + button.size*0.5 + 2;
}