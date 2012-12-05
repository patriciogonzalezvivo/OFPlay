#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofSetWindowTitle( "OFPlay" );
//    ofSetLogLevel(OF_LOG_VERBOSE);
//    ofSetWindowShape(768,768);
    ofBackground(230);//225,223,229);
//    ofSetDataPathRoot("../Resources/");
    
    project             = NULL;
    statusEnergy        = 0;
        
    string  sketchName  = "newSketch";

    ofxXmlSettings XML;
    XML.loadFile("projectGeneratorSettings.xml");
    string appToRoot = XML.getValue("appToRoot", "../../../../");
    defaultLoc = XML.getValue("defaultNewProjectLocation", "apps");
    
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
    
    ofRoot = ofFilePath::getAbsolutePath(ofFilePath::join(binPath, appToRoot));
    addonsPath = ofFilePath::getAbsolutePath(ofFilePath::join(ofRoot,"addons"));
    string sketchPath = ofFilePath::getAbsolutePath(ofFilePath::join(ofRoot, defaultLoc));
    
    convertWindowsToUnixPath(ofRoot);
    convertWindowsToUnixPath(addonsPath);
    convertWindowsToUnixPath(sketchPath);
    
    // there's some issues internally in OF with non unix paths for OF root
    //
    setOFRoot(ofRoot);
    
    //------------------------------------- GUI
    //
    defaultHeight       = 34;
    logo.loadImage("OFPlay.png");
    font.loadFont("Inconsolata.otf", 12, true,false,false,0.5,90);
    
    //  PATH:
    //
    projectPath.setText(sketchPath);
    projectPath.setPrefix("Path: ");
    projectPath.setDivider("/");
    projectPath.font = &font;
    projectPath.setSizeAndShapes(defaultHeight, 3);
    projectPath.x = defaultHeight;
    projectPath.y = defaultHeight;
    loadFolder( "apps/" );
    loadFolder( "examples/" );
    
    froebelTextBox *subProjectPath = new froebelTextBox();
    *subProjectPath = projectPath;
    subProjectPath->setPrefix("<< ");
    subProjectPath->setText("CHANGE THE DIRECTORY");
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
    projectName.setText(sketchName);
    projectName.setPrefix("Name: ");
    projectName.font = &font;
    projectName.setSizeAndShapes(defaultHeight, 3);
    projectName.x = defaultHeight;
    projectName.y = projectPath.y + projectPath.height + defaultHeight*0.5;
    projectName.enable();
    
    froebelTextBox *subProjectName = new froebelTextBox();
    *subProjectName = projectName;
    subProjectName->setPrefix("<< ");
    subProjectName->setText("CHANGE THE NAME");
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
    platformsList.conteinerBox.maxHeight = 200;
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
    subPlatformList->setText("CHANGE THE PLATFORM");
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
    addonsList.conteinerBox.maxHeight = ofGetHeight() - addonsList.y - defaultHeight*3.0;
    loadAddons();
    
    froebelTextBox *subAddonsList = new froebelTextBox();
    *subAddonsList = addonsList;
    subAddonsList->setPrefix("<< ");
    subAddonsList->setText("SELECT ADDONS");
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
    
    generateButton.setText( "GENERATE" );
    generateButton.font = &font;
    generateButton.bFixedSize = true;
    generateButton.bgColor.clear();
    generateButton.bgColor.addState(ofFloatColor(0.0,0.0));
    generateButton.bgColor.addState(ofFloatColor(0.0,0.0));
    generateButton.bgColor.addState(ofFloatColor(0.0,0.0));
    generateButton.setSizeAndShapes(defaultHeight);
}

void testApp::loadAddons(){
    addonsList.conteinerBox.clear();
    
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

void testApp::loadFolder(string _path){
    ofDirectory folder( ofFilePath::getAbsolutePath(ofFilePath::join(ofRoot,_path)) );

    if (folder.exists()){
        projectPath.addElement(_path);
        folder.listDir();
        for(int i=0; i < (int)folder.size();i++){
            if (folder.getFiles()[i].isDirectory()){
                
            }
            
        }
    }
}

//------------------------------------------------------
bool testApp::isAddonCore(string addon){
    //  Pre define what's a core addon
    //
    vector<string>  coreAddons;
    coreAddons.push_back("ofx3DModelLoader");
    coreAddons.push_back("ofxAssimpModelLoader");
    coreAddons.push_back("ofxDirList");
    coreAddons.push_back("ofxNetwork");
    coreAddons.push_back("ofxOpenCv");
    coreAddons.push_back("ofxOsc");
    coreAddons.push_back("ofxThread");
    coreAddons.push_back("ofxThreadedImageLoader");
    coreAddons.push_back("ofxVectorGraphics");
    coreAddons.push_back("ofxVectorMath");
    coreAddons.push_back("ofxXmlSettings");
    coreAddons.push_back("ofxSvg");
    
    for (int i = 0; i < coreAddons.size(); i++){
        if (coreAddons[i] == addon){
            return true;
        }
    }
    return false;
}

void testApp::loadProject(string _path){
    //  Extract Name and Path
    //
    string folder = "";
    
    extractFolderFromPath(_path,folder);
    projectName.setText(folder);
    projectPath.setText(_path);
    
    setStatus("Project " + folder + " loaded ");
    
    //  Extracting Addons ( from addons.make)
    //
    addonsList.conteinerBox.reset();
    
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

string testApp::setTarget(int targ){
    
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

void testApp::setStatus(string newStatus){
    statusEnergy = 1;
    status = newStatus;
    statusSetTime = ofGetElapsedTimef();
}

void testApp::generateProject(){
    
    vector <int> targetsToMake;
    for(int i = 0; i < platformsList.conteinerBox.elements.size(); i++){
        if ( platformsList.conteinerBox.elements[i]->bSelected == true ){
            if (platformsList.conteinerBox.elements[i]->getText() == "windows (codeblocks)" ){
                targetsToMake.push_back(OF_TARGET_WINGCC);
            } else if (platformsList.conteinerBox.elements[i]->getText() == "windows (visualStudio)"){
                targetsToMake.push_back(OF_TARGET_WINVS);
            } else if (platformsList.conteinerBox.elements[i]->getText() == "linux (codeblocks)"){
                targetsToMake.push_back(OF_TARGET_LINUX);
            } else if (platformsList.conteinerBox.elements[i]->getText() == "linux64 (codeblocks)"){
                targetsToMake.push_back(OF_TARGET_LINUX64);
            } else if (platformsList.conteinerBox.elements[i]->getText() == "osx (xcode)"){
                targetsToMake.push_back(OF_TARGET_OSX);
            } else if (platformsList.conteinerBox.elements[i]->getText() == "ios (xcode)"){
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
            
            vector<string> addons = addonsList.conteinerBox.getSelected();
            for (int i = 0; i < addons.size(); i++){
                ofAddon addon;
                addon.pathToOF = getOFRelPath(path);
                addon.fromFS(ofFilePath::join(addonsPath, addons[i]),target);
                project->addAddon(addon);
            }
            
            project->save(true);
        }
        
        setStatus("generated: " + projectPath.getText() + "/" + projectName.getText() + " for " + platformsList.conteinerBox.getSelected()[i]);
	}
    
    printf("done with project generation \n");
}

//--------------------------------------------------------------
void testApp::update(){
    ofPoint mouse = ofPoint(mouseX, mouseY);
    
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
    
    float diff = ofGetElapsedTimef()- statusSetTime;
    if (diff > 3){
        statusEnergy *= 0.99;;
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){

    projectName.draw();
    projectPath.draw();
    addonsList.draw();
    platformsList.draw();
    
    ofSetColor(255);
    logo.draw(ofGetWidth() - defaultHeight - logo.getWidth(),ofGetHeight() - defaultHeight - logo.getHeight());
    generateButton.draw();
    
    ofFill();
    ofSetColor(0 + 220 * (1-statusEnergy),0 + 220 * (1-statusEnergy),0 + 220 * (1-statusEnergy));
    ofRect(0,ofGetHeight(), ofGetWidth(), -25);
    ofSetColor(255,255,255, 255 * statusEnergy);
    ofDrawBitmapString(status, 10,ofGetHeight()-8);
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
    
    if ( projectPath.checkMousePressed( mouse )){
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
        platformsList.bSelected = false;
        addonsList.bSelected    = false;
        projectName.bSelected   = false;
        addonsList.bSelected    = false;
        projectPath.bSelected   = true;
    } else if ( addonsList.checkMousePressed(mouse)){
        platformsList.bSelected = false;
        addonsList.bSelected    = false;
        projectPath.bSelected   = false;
        projectName.bSelected   = false;
        addonsList.bSelected    = true;
    } else if ( platformsList.checkMousePressed(mouse)){
        platformsList.bSelected = false;
        addonsList.bSelected    = false;
        projectPath.bSelected   = false;
        projectName.bSelected   = false;
        platformsList.bSelected = true;
    } else if ( generateButton.checkMousePressed(mouse)){
        platformsList.bSelected = false;
        addonsList.bSelected    = false;
        projectPath.bSelected   = false;
        projectName.bSelected   = false;
        generateProject();
        generateButton.bSelected = false;
    } else {
        platformsList.bSelected = false;
        addonsList.bSelected    = false;
        projectPath.bSelected   = false;
        projectName.bSelected   = false;
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
    projectName.subInfo->width = ofGetWidth() - defaultHeight * 2.0;
    projectPath.subInfo->width = ofGetWidth() - defaultHeight * 2.0;
    platformsList.subInfo->width = ofGetWidth() - defaultHeight * 2.0;
    addonsList.subInfo->width = ofGetWidth() - defaultHeight * 2.0;
    
    addonsList.conteinerBox.maxHeight = ofGetHeight() - addonsList.y - defaultHeight*3.0;
    addonsList.bChange = true;
    
    generateButton.x = ofGetWidth() - defaultHeight - generateButton.width - defaultHeight*0.7;
    generateButton.y = ofGetHeight() - logo.getHeight()*0.5 - defaultHeight*1.5;
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    if (dragInfo.files.size() > 1)
        for (int i = 0; i < dragInfo.files.size(); i++){
            cout << dragInfo.files[i] << endl;
    } else if (dragInfo.files.size() == 1) {
        
        string open = dragInfo.files[0];
        if (isProjectFolder(open)){
            loadProject(open);
        }
    }
}
