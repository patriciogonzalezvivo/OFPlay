//
//  froebelDirElement.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/5/12.
//  http://www.patriciogonzalezvivo.com
//

#include "froebelFolderElement.h"

#include "Utils.h"

froebelFolderElement::froebelFolderElement(){
    subInfo     = NULL;
    father      = NULL;
    
    bLeftAlign  = true;
    bChange     = true;
    bEdge       = false;
    bIcon       = false;
    bSelected   = false;
    bFixedSize  = false;
    
    text        = "";
    prefix      = "";
    deliminater = "/";
    rootPath    = "";
    
    minWidth        = 0;
    maxWidth        = 600;
    size            = 40;
    nState          = 0;
    
    containerBox.bCheckList = false;
    
    //  STATE_PASSIVE
    //
    fgColor.addState(2);
    bgColor.addState(5);
    
    //  STATE_HOVER
    //
    fgColor.addState(4);
    bgColor.addState(5);
    
    //  STATE_ACTIVE
    //
    fgColor.addState(3);
    bgColor.addState(4);
}

string froebelFolderElement::getText(){
    string rta = text;
    
    for( int i = 0; i < containerBox.elements.size(); i++ ){
        if ( containerBox.elements[i]->bSelected ){
            rta += containerBox.elements[i]->getText();
            break;
        }
    }
    
    return rta;
}

ofRectangle froebelFolderElement::getBoundingBox(){
    ofRectangle rta;
    
    rta.set(*this);
    
    if (subInfo != NULL)
        rta.growToInclude(*subInfo);
    
    if (bSelected){
        rta.growToInclude(containerBox);
    
        for (int i = 0; i < containerBox.elements.size(); i++){
            if (inside(containerBox.elements[i]->getCenter())){
                rta.growToInclude( containerBox.elements[i]->getBoundingBox() );
            }
        }
    }
    
    return rta;
}

bool froebelFolderElement::checkMousePressed(ofPoint _mouse){
    bool rta = false;
    
    if ( !bSelected ){
        
        if ( inside(_mouse) ){
            
            //  Every time is clicked it re-populate it content
            //
            containerBox.clear();
            ofDirectory folder(rootPath+text);
            folder.listDir();
            for(int i=0; i < (int)folder.size();i++){
                
                if (folder.getFile(i).isDirectory()){
                    
                    string name = folder.getName(i);
                    string path = rootPath+text+name;
                    
                    if (isAddonCore(name) ||
                        name == "build" ||
                        name == "bin" ||
                        name == "src" ||
                        name == "lib" ||
                        name == "libs" ||
                        name == "include" ||
                        name == "scripts" ||
                        name == "doc" ||
                        name == "docs"){
                        
                      //    Skip core addons folder because don't have a example inside
                      //    and src libs folders
                        
                    } else if (  isProjectFolder( path ) ){
                        froebelTextBox *newElement = new froebelTextBox();
                        newElement->font = font;
                        newElement->setSizeAndShapes(size,3,5);
                        newElement->setText(folder.getName(i));
                        
                        newElement->fgColor.clear();
                        newElement->fgColor.addState(5);
                        newElement->fgColor.addState(7);
                        newElement->fgColor.addState(4);
                        newElement->bgColor.clear();
                        newElement->bgColor.addState(0);
                        newElement->bgColor.addState(2);
                        newElement->bgColor.addState(7);
                        
                        newElement->update();
                    
                        containerBox.addElement(newElement);
                        
                    } else {
                        froebelFolderElement *newFolder = new froebelFolderElement();
                        newFolder->font = font;
                        newFolder->setSizeAndShapes(size);
                        newFolder->setText( folder.getName(i) + "/");
                        newFolder->rootPath = rootPath + text ;
                        newFolder->fgColor.clear();
                        newFolder->fgColor.addState(5);
                        newFolder->fgColor.addState(7);
                        newFolder->fgColor.addState(4);
                        newFolder->bgColor.clear();
                        newFolder->bgColor.addState(0);
                        newFolder->bgColor.addState(2);
                        newFolder->bgColor.addState(7);
                        newFolder->containerBox.size = size;
                        
                        newFolder->father = &containerBox;
                        
                        newFolder->update();
                        
                        containerBox.addElement(newFolder);
                    }
                }
            }
            
            bSelected = true;
            bChange = true;
            rta = true;
        }
        
    } else {
    
        if ( containerBox.checkMousePressed(_mouse) ){
            rta = true;
        }
    }
    
    return rta;
}

void froebelFolderElement::update(){
    froebelTextBox::update();
    
    containerBox.x = father->x + father->width;
    containerBox.y = father->y;
    containerBox.maxHeight = father->maxHeight;
    containerBox.bEnable = bSelected;
    
    containerBox.update();
}

void froebelFolderElement::draw(){
    
    if (bEdge)
        endingShape.color.set(bgColor);
    
    if (bIcon)
        iconShape.color.set(fgColor);
    
    
    //  Render Elements
    //
    if ( bSelected ){
        containerBox.draw();
    }
    
    froebelTextBox::draw();
}