#include "testApp.h"
#include "Utils.h"
#include <stdio.h>
#include "ofConstants.h"

void convertWindowsToUnixPath(string & path){
    for (int i = 0; i < path.size(); i++){
        if (path[i] == '\\') path[i] = '/';
    }
}

string windowsFromUnixPath(string path){
    for (int i = 0; i < path.size(); i++){
        if (path[i] == '/') path[i] = '\\';
    }
	return path;
}

void extractFolderFromPath(string &_path, string &_folder){
    string completePath = _path;
    _folder = "";
    _path = "";
    
    int i;
    for (i = completePath.size()-1 ; completePath[i] != '/'; i--){
        _folder.insert(_folder.begin(), completePath[i]);
    }
    for (i-- ; completePath[i] >= 0; i--){
        _path.insert(_path.begin(), completePath[i]);
    }
}

void fixStringCharacters(string &toFix){
    
    // replace all non alpha numeric (ascii) characters with _
    for (int i = 0; i < toFix.size(); i++){
        int which = (int)toFix[i];
        if ((which >= 48 && which <= 57) ||
            (which >= 65 && which <= 90) ||
            (which >= 97 && which <= 122)){
        } else {
            toFix[i] = '_';
        }
    }
}

bool isProjectFolder(string &_projFolder){
    //  Return true or false if a project Folder structure it's found and change the _projFolder string
    //  to become the correct path to a folder structure
    //
    
    //  1. If is a directory
    //
    ofDirectory dir;
    string searchFor = _projFolder;
    dir.open(searchFor);
    if ( dir.isDirectory() ){
        
        //  Is a project directory or a src directory?
        //
        string folder;
        extractFolderFromPath(searchFor, folder);
        if ( (folder == "src") || (folder == "bin") || ( (int)folder.find(".xcodeproj") > 0) ){
            _projFolder = searchFor;
        } else {
            searchFor = _projFolder;
        }
        
    } else {
        
        //  If is a file it have something related to a project?
        //
        string name;
        extractFolderFromPath(searchFor, name);
        if (((int)name.find(".cbp") > 0) ||
            ((int)name.find(".workspace") > 0) ||
            ((int)name.find(".plist") > 0) ||
            ((int)name.find(".xcconfig") > 0) ||
            ((int)name.find(".make") > 0) ||
            ((int)name.find(".vcxproj") > 0 )){
            _projFolder = searchFor;
        } else {
            return false;
        }
    }
    
    //  3. Have src/
    //
    searchFor = searchFor+"/src";
    dir.open( searchFor );
    if (!dir.isDirectory())
        return false;
    
    //  4. Have main.cpp, testApp.h, testApp.cpp?
    //
    ofFile test;
    bool    isMainCpp = test.open(searchFor+"/main.cpp");
    bool    isTestAppH = test.open(searchFor+"/testApp.h");
    bool    isTestAppCpp = test.open(searchFor+"/testApp.cpp");
    
    if ( !(isMainCpp && isTestAppH && isTestAppCpp) )
        return false;
    
    return true;
}

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
//    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //  Default Settings
    //
    project             = NULL;
    statusEnergy        = 0;
    
    float defaultHeight = 38;
    float paddingTop    = defaultHeight;
    float paddingLeft   = defaultHeight;
    float paddingRight  = defaultHeight;
    float paddingButton = defaultHeight*0.5;
    
    string  sketchName  = "mySketch";
    
    logo.loadImage("images/OFPlay.png");
    logo.setAnchorPercent(0.5, 0.5);

    //  XML Settings
    //
    ofxXmlSettings XML;
    XML.loadFile("settings/projectGeneratorSettings.xml");
    appToRoot = XML.getValue("appToRoot", "../../../../");
    defaultLoc = XML.getValue("defaultNewProjectLocation", "apps/myApps");
    
    //-------------------------------------
    // calculate the bin path (../../../ on osx) and the sketch path (bin -> root - > defaultLoc)
    //-------------------------------------
    // if appToRoot is wrong, we have alot of issues.  all these paths are used in this project:
    //
#ifdef TARGET_OSX
    string binPath = ofFilePath::getAbsolutePath(ofFilePath::join(ofFilePath::getCurrentWorkingDirectory(), "../../../"));
#else
    string binPath = ofFilePath::getCurrentExeDir();
#endif
    
    string ofRoot = ofFilePath::getAbsolutePath(ofFilePath::join(binPath, appToRoot));
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
    
    //  load font and setup the buttons
    //
    font.loadFont("fonts/Inconsolata.otf", 14, true,false,false,0.5,90);
    secondFont.loadFont("fonts/GeosansLight.ttf", 11, true,false,false,0.5,90);
    
    //  Name:
    //
    projectName.setText(sketchName);
    projectName.setPrefix("Name: ");
    projectName.font = &font;
    projectName.setSizeAndShapes(defaultHeight, 3);
    projectName.x = paddingLeft;
    projectName.y = paddingTop;
    projectName.enable();
    
    froebelTextBox *subProjectName = new froebelTextBox();
    *subProjectName = projectName;
    subProjectName->setPrefix("<< ");
    subProjectName->setText("CHANGE THE NAME");
    subProjectName->font = &secondFont;
    subProjectName->bLeftAlign = false;
    subProjectName->setSizeAndShapes(defaultHeight, 1);
    subProjectName->setActiveColors(4, 6);
    subProjectName->setPasiveColors(3, 0);
    subProjectName->width = ofGetWidth() - paddingLeft*2.0;
    projectName.subInfo = subProjectName;
    
    projectPath.setText(sketchPath);
    projectPath.setPrefix("Path: ");
    projectPath.setDivider("/");
    projectPath.font = &font;
    projectPath.setSizeAndShapes(defaultHeight, 3);
    projectPath.x = paddingLeft;
    projectPath.y = projectName.y + projectName.height + paddingButton;
    
    froebelTextBox *subProjectPath = new froebelTextBox();
    *subProjectPath = projectPath;
    subProjectPath->setPrefix("<< ");
    subProjectPath->setText("CHANGE THE DIRECTORY");
    subProjectPath->font = &secondFont;
    subProjectPath->bLeftAlign = false;
    subProjectPath->setSizeAndShapes(defaultHeight, 1);
    subProjectPath->setActiveColors(4, 6);
    subProjectPath->setPasiveColors(3, 0);
    subProjectPath->width = ofGetWidth() - paddingLeft*2.0;
    projectPath.subInfo = subProjectPath;
    
    //  LOAD PLATFORMS
    //
    platformsList.x = paddingLeft;
    platformsList.y = projectPath.y + projectPath.height + paddingButton;
    platformsList.font = &font;
    platformsList.setPrefix("Platform: ");
    platformsList.setDivider(", ");
    platformsList.setSizeAndShapes(38,3);
    platformsList.width = 320;
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
    subPlatformList->setDivider( "<< " );
    subPlatformList->setText("CHANGE THE PLATFORM TARGET");
    subPlatformList->font = &secondFont;
    subPlatformList->bLeftAlign = false;
    subPlatformList->setSizeAndShapes(defaultHeight, 1);
    subPlatformList->setActiveColors(4, 6);
    subPlatformList->setPasiveColors(3, 0);
    subPlatformList->width = ofGetWidth() - paddingLeft*2.0;
    platformsList.subInfo = subPlatformList;

    //  LOAD ADDONS
    //
    addonsList.x = paddingLeft;
    addonsList.y = platformsList.y + platformsList.height + paddingButton;
    addonsList.font = &font;
    addonsList.setPrefix("Addons: ");
    addonsList.setDivider(", ");
    addonsList.setSizeAndShapes(38,3);
    addonsList.width = 320;
    addonsList.height = defaultHeight;
    addonsList.maxWidth = 700;
    addonsList.maxHeight = 200;
    
    ofDirectory addonsFolder(addonsPath);
    addonsFolder.listDir();
    for(int i=0; i < (int)addonsFolder.size();i++){
    	string addonName = addonsFolder.getName(i);
        
    	if(addonName.find("ofx")==0){
            if (isAddonCore(addonName)){
                addonsList.addElement(addonName,false,0);
            } else {
                addonsList.addElement(addonName,false,4);
            }
    	}
    }
    
    froebelTextBox *subAddonsList = new froebelTextBox();
    *subAddonsList = addonsList;
    subAddonsList->setPrefix("<< ");
    subAddonsList->setText("SELECT ADDONS");
    subAddonsList->font = &secondFont;
    subAddonsList->bLeftAlign = false;
    subAddonsList->setSizeAndShapes(defaultHeight, 1);
    subAddonsList->setActiveColors(4, 6);
    subAddonsList->setPasiveColors(3, 0);
    subAddonsList->width = ofGetWidth() - paddingLeft*2.0;
    addonsList.subInfo = subAddonsList;
    
    generateButton.setText( "GENERATE PROJECT" );
    generateButton.font = &font;
    generateButton.setSizeAndShapes(defaultHeight);
    generateButton.x = ofGetWidth() - paddingLeft - 300;
    generateButton.y = ofGetHeight() - defaultHeight - paddingButton;
    generateButton.width = 300;
    
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
    addonsList.reset();
    
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
    for(int i = 0; i < platformsList.elements.size(); i++){
        if ( platformsList.elements[i]->bSelected == true ){
            if (platformsList.elements[i]->getText() == "windows (codeblocks)" ){
                targetsToMake.push_back(OF_TARGET_WINGCC);
            } else if (platformsList.elements[i]->getText() == "windows (visualStudio)"){
                targetsToMake.push_back(OF_TARGET_WINVS);
            } else if (platformsList.elements[i]->getText() == "linux (codeblocks)"){
                targetsToMake.push_back(OF_TARGET_LINUX);
            } else if (platformsList.elements[i]->getText() == "linux64 (codeblocks)"){
                targetsToMake.push_back(OF_TARGET_LINUX64);
            } else if (platformsList.elements[i]->getText() == "osx (xcode)"){
                targetsToMake.push_back(OF_TARGET_OSX);
            } else if (platformsList.elements[i]->getText() == "ios (xcode)"){
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
            
            vector<string> addons = addonsList.getSelected();
            for (int i = 0; i < addons.size(); i++){
                ofAddon addon;
                addon.pathToOF = getOFRelPath(path);
                addon.fromFS(ofFilePath::join(addonsPath, addons[i]),target);
                project->addAddon(addon);
            }
            
            project->save(true);
        }
        
        setStatus("generated: " + projectPath.getText() + "/" + projectName.getText() + " for " + platformsList.getSelected()[i]);
	}
    
    printf("done with project generation \n");
}

//--------------------------------------------------------------
void testApp::update(){
    ofPoint mouse = ofPoint(mouseX, mouseY);
    
    projectName.update();
    projectPath.update();
    platformsList.update();
    addonsList.update();
    
    float diff = ofGetElapsedTimef()- statusSetTime;
    if (diff > 3){
        statusEnergy *= 0.99;;
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(230,230,230);

    projectName.draw();
    projectPath.draw();
    addonsList.draw();
    platformsList.draw();
    
    ofSetColor(255);
    logo.draw(798,506);
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
        
        projectPath.bSelected   = false;
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
