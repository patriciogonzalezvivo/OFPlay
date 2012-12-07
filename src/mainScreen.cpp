//
//  mainScreen.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/7/12.
//
//

#include "mainScreen.h"

mainScreen::mainScreen(){
    project             = NULL;
    statusEnergy        = 0;
    
    
}

void mainScreen::setup( string _path, string _name ){
    
    //------------------------------------- GUI
    //
    defaultHeight       = 34;
    logo.loadImage("OFPlay.png");
    font.loadFont("Inconsolata.otf", 12, true,false,false,0.5,90);
    
    //  PATH:
    //
    projectPath.setText( _path );
    projectPath.setPrefix("Path: ");
    projectPath.setDivider("/");
    projectPath.font = &font;
    projectPath.setSizeAndShapes(defaultHeight, 3);
    projectPath.x = defaultHeight;
    projectPath.y = defaultHeight;
    projectPath.containerBox.bCheckList = false;
    
    ofAddListener(projectPath.focusLost, this, &mainScreen::pathChange);
    
    loadFolder( "apps/" );
    loadFolder( "examples/" );
    loadFolder( "addons/" );
    
    froebelTextBox *subProjectPath = new froebelTextBox();
    *subProjectPath = projectPath;
    subProjectPath->setPrefix("<< ");
    subProjectPath->setText("CURRENT PATH");
    subProjectPath->font = &font;
    subProjectPath->bLeftAlign = false;
    subProjectPath->bFixedSize = true;
    subProjectPath->setSizeAndShapes(defaultHeight,1);
    subProjectPath->fgColor.clear();
    subProjectPath->fgColor.addState(5);
    subProjectPath->fgColor.addState(7);
    subProjectPath->fgColor.addState(5);
    subProjectPath->bgColor.clear();
    subProjectPath->bgColor.addState(0);
    subProjectPath->bgColor.addState(2);
    subProjectPath->bgColor.addState(0);
    subProjectPath->width = ofGetWidth() - defaultHeight;
    projectPath.subInfo = subProjectPath;
    
    //  NAME:
    //
    projectName.setText( _name );
    projectName.setPrefix("Name: ");
    projectName.font = &font;
    projectName.setSizeAndShapes(defaultHeight, 3);
    projectName.x = defaultHeight;
    projectName.y = projectPath.y + projectPath.height + defaultHeight*0.5;
    projectName.enable();
    ofAddListener(projectName.focusLost, this, &mainScreen::nameChange);
    
    froebelTextBox *subProjectName = new froebelTextBox();
    *subProjectName = projectName;
    subProjectName->setPrefix("<< ");
    subProjectName->setText("NEW PROJECT NAME");
    subProjectName->font = &font;//&secondFont;
    subProjectName->bLeftAlign = false;
    subProjectName->bFixedSize = true;
    subProjectName->setSizeAndShapes(defaultHeight,1);
    subProjectName->fgColor.clear();
    subProjectName->fgColor.addState(5);
    subProjectName->fgColor.addState(7);
    subProjectName->fgColor.addState(5);
    subProjectName->bgColor.clear();
    subProjectName->bgColor.addState(0);
    subProjectName->bgColor.addState(2);
    subProjectName->bgColor.addState(0);
    subProjectName->width = ofGetWidth() - defaultHeight;
    projectName.subInfo = subProjectName;
    
    //  LOAD PLATFORMS
    //
    platformsList.x = defaultHeight;
    platformsList.y = projectName.y + projectName.height + defaultHeight*0.5;
    platformsList.font = &font;
    platformsList.setPrefix("Platform: ");
    platformsList.setDivider(", ");
    platformsList.setSizeAndShapes(defaultHeight,3);
    platformsList.containerBox.maxHeight = 200;
    platformsList.addElement("windows (codeblocks)",ofGetTargetPlatform()==OF_TARGET_WINGCC);
	platformsList.addElement("windows (visualStudio)", ofGetTargetPlatform()==OF_TARGET_WINVS);
	platformsList.addElement("linux (codeblocks)",ofGetTargetPlatform()==OF_TARGET_LINUX);
	platformsList.addElement("linux64 (codeblocks)",ofGetTargetPlatform()==OF_TARGET_LINUX64);
    
    //#define MAKE_IOS
#ifdef MAKE_IOS
	platformsList.addElement("osx (xcode)",false);
	platformsList.addElement("ios (xcode)",true);
#else
    platformsList.addElement("osx (xcode)",ofGetTargetPlatform()==OF_TARGET_OSX);
	platformsList.addElement("ios (xcode)",ofGetTargetPlatform()==OF_TARGET_IPHONE);
#endif
    platformsList.setText( platformsList.getSelectedAsString() );
    
    froebelTextBox *subPlatformList = new froebelTextBox();
    *subPlatformList = platformsList;
    subPlatformList->setPrefix( "<< " );
    subPlatformList->setText("TARGET PLATFORM");
    subPlatformList->font = &font;//&secondFont;
    subPlatformList->bLeftAlign = false;
    subPlatformList->bFixedSize = true;
    subPlatformList->setSizeAndShapes(defaultHeight,1);
    subPlatformList->fgColor.clear();
    subPlatformList->fgColor.addState(5);
    subPlatformList->fgColor.addState(7);
    subPlatformList->fgColor.addState(5);
    subPlatformList->bgColor.clear();
    subPlatformList->bgColor.addState(0);
    subPlatformList->bgColor.addState(2);
    subPlatformList->bgColor.addState(0);
    subPlatformList->width = ofGetWidth() - defaultHeight;
    platformsList.subInfo = subPlatformList;
    
    //  LOAD ADDONS
    //
    addonsList.x = defaultHeight;
    addonsList.y = platformsList.y + platformsList.height + defaultHeight*0.5;
    addonsList.font = &font;
    addonsList.setPrefix("Addons: ");
    addonsList.setDivider(", ");
    addonsList.setSizeAndShapes(defaultHeight,3);
    addonsList.maxWidth = 700;
    addonsList.containerBox.maxHeight = ofGetHeight() - addonsList.y - defaultHeight*3.0;
    loadAddons();
    
    froebelTextBox *subAddonsList = new froebelTextBox();
    *subAddonsList = addonsList;
    subAddonsList->setPrefix("<< ");
    subAddonsList->setText("SELECTED ADDONS");
    subAddonsList->font = &font;//&secondFont;
    subAddonsList->bLeftAlign = false;
    subAddonsList->bFixedSize = true;
    subAddonsList->setSizeAndShapes(defaultHeight,1);
    subAddonsList->fgColor.clear();
    subAddonsList->fgColor.addState(5);
    subAddonsList->fgColor.addState(7);
    subAddonsList->fgColor.addState(5);
    subAddonsList->bgColor.clear();
    subAddonsList->bgColor.addState(0);
    subAddonsList->bgColor.addState(2);
    subAddonsList->bgColor.addState(0);
    subAddonsList->width = ofGetWidth() - defaultHeight;
    addonsList.subInfo = subAddonsList;
    
    generateButton.setShape(0,132);
    generateButton.color.set(0.0,0.0);
    generateButton.font = &font;
    generateButton.text = "GENERATE";
    generateButton.textColor.addState(4);
    generateButton.textColor.addState(5);
    generateButton.textColor.addState(3);
    
    openButton.setShape(2, 132);
    openButton.color.set(0.0,0.0);
    openButton.font = &font;
    openButton.text =  "OPEN";
    openButton.textColor.addState(3);
    openButton.textColor.addState(2);
    openButton.textColor.addState(4);
    
    checkProjectState();
}

void mainScreen::loadAddons(){
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

void mainScreen::loadFolder(string _path){
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
        newFolder->fgColor.addState(7);
        newFolder->fgColor.addState(4);
        newFolder->bgColor.clear();
        newFolder->bgColor.addState(0);
        newFolder->bgColor.addState(2);
        newFolder->bgColor.addState(7);
        newFolder->containerBox.size = defaultHeight;
        
        newFolder->father = &projectPath.containerBox;
        
        newFolder->update();
        
        //  Added to the list
        //
        projectPath.containerBox.addElement( newFolder );
    }
}

void mainScreen::pathChange(string &_path){
    string completePath = ofRoot + projectPath.getText();
    
    addonsList.containerBox.reset();
    addonsList.bChange = true;
    
    if ( isProjectFolder(completePath) ){
        loadProject( completePath );
    } else {
        projectPath.setText(ofRoot + projectPath.getText());
        projectName.setText("newProject");
    }
    
    checkProjectState();
}

void mainScreen::nameChange(string &_path){
    addonsList.containerBox.reset();
    addonsList.bChange = true;
    
    checkProjectState();
}

void mainScreen::loadProject(string _path){
    //  Extract Name and Path
    //
    string folder = "";
    
    extractFolderFromPath(_path,folder);
    projectName.setText(folder);
    projectPath.setText(_path);
    
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

string mainScreen::setTarget(int targ){
    
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

void mainScreen::setStatus(string newStatus){
    statusEnergy = 1;
    status = newStatus;
    statusSetTime = ofGetElapsedTimef();
}

void mainScreen::checkProjectState(){
    openButton.bEnable = isProjectGenerated(projectPath.getText(),projectName.getText());
    string projectPathString = projectPath.getText()+"/"+projectName.getText();
    ofDirectory projectPath( projectPathString );
    
    if (openButton.bEnable || projectPath.exists()){
        projectName.subInfo->setText("PROJECT NAME");
    } else {
        projectName.subInfo->setText("NEW PROJECT NAME");
        
    }
}

void mainScreen::generateProject(){
    
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
    
    checkProjectState();
    printf("done with project generation \n");
}

void mainScreen::update(){
    projectPath.update();
    ofRectangle prev = projectPath.getBoundingBox();
    projectName.x = prev.x;
    projectName.y = prev.y + prev.height + defaultHeight*0.5;
    projectName.update();
    
    prev = projectName.getBoundingBox();
    platformsList.x = prev.x;
    platformsList.y = prev.y + prev.height + defaultHeight*0.5;
    platformsList.update();
    
    prev = platformsList.getBoundingBox();
    addonsList.x = prev.x;
    addonsList.y = prev.y + prev.height + defaultHeight*0.5;
    addonsList.update();
    
    generateButton.update();
    openButton.update();
    
    float diff = ofGetElapsedTimef()- statusSetTime;
    if (diff > 3){
        statusEnergy *= 0.99;;
    }
}

void mainScreen::draw(){
    projectName.draw();
    projectPath.draw();
    addonsList.draw();
    platformsList.draw();
    
    ofSetColor(255);
    logo.draw(ofGetWidth() - defaultHeight - logo.getWidth(),ofGetHeight() - defaultHeight - logo.getHeight());
    generateButton.draw();
    openButton.draw();
    
    ofFill();
    ofSetColor(0 + 220 * (1-statusEnergy),0 + 220 * (1-statusEnergy),0 + 220 * (1-statusEnergy));
    ofRect(0,ofGetHeight(), ofGetWidth(), -25);
    ofSetColor(255,255,255, 255 * statusEnergy);
    ofDrawBitmapString(status, 10,ofGetHeight()-8);
}

void mainScreen::mousePressed(ofPoint _mouse){
    if ( projectPath.checkMousePressed( _mouse ) ){
        /*
         string command = "";
         ofDirectory dir(ofFilePath::join(getOFRoot(),defaultLoc));
         
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
         */
        
        projectPath.bSelected   = true;
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
    } else if ( generateButton.checkMousePressed(_mouse)){
        projectPath.bSelected   = false;
        projectName.bSelected   = false;
        platformsList.bSelected = false;
        addonsList.bSelected    = false;
        generateProject();
    } else if ( openButton.checkMousePressed(_mouse)){
        platformsList.bSelected = false;
        addonsList.bSelected    = false;
        projectPath.bSelected   = false;
        projectName.bSelected   = false;
        
        string path = "open " + projectPath.getText() + "/"+ projectName.getText() + "/" +  projectName.getText() + ".xcodeproj";
        system( path.c_str() );
        
    } else {
        platformsList.bSelected = false;
        addonsList.bSelected    = false;
        projectPath.bSelected   = false;
        projectName.bSelected   = false;
    }
}

void mainScreen::resized(){
    projectName.subInfo->width = ofGetWidth() - defaultHeight * 2.0;
    projectPath.subInfo->width = ofGetWidth() - defaultHeight * 2.0;
    platformsList.subInfo->width = ofGetWidth() - defaultHeight * 2.0;
    addonsList.subInfo->width = ofGetWidth() - defaultHeight * 2.0;
    
    addonsList.containerBox.maxHeight = ofGetHeight() - addonsList.y - defaultHeight*3.0;
    addonsList.containerBox.adjustShape();
    
    generateButton.x = ofGetWidth() - defaultHeight - logo.getWidth() + generateButton.size*0.5 + 3;
    generateButton.y = ofGetHeight() - defaultHeight - logo.getHeight() + generateButton.size*0.5 + 2;
    
    openButton.x = ofGetWidth() - defaultHeight - logo.getWidth()*0.535 + openButton.size;
    openButton.y = ofGetHeight() - defaultHeight - logo.getHeight() + openButton.size*0.5 + 2;
}