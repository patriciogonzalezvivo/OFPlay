//
//  froebelDirElement.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/5/12.
//
//

#include "froebelFolderElement.h"

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


void froebelFolderElement::addElement(string _value, bool _defVal, int _iconShape, int _edgeCoorner){
    
    //  Create new element
    //
    froebelTextBox *newElement = new froebelTextBox();
    newElement->font = font;
    newElement->setSizeAndShapes(size,_edgeCoorner,_iconShape);
    newElement->setText(_value);
    newElement->setPrefix("  ");
    newElement->bSelected = _defVal;
    
    newElement->fgColor.clear();
    newElement->fgColor.addState(5);
    newElement->fgColor.addState(7);
    newElement->fgColor.addState(4);
    newElement->bgColor.clear();
    newElement->bgColor.addState(0);
    newElement->bgColor.addState(2);
    newElement->bgColor.addState(7);
    
    newElement->update();
    
    //  recalculate the container box
    //
    containerBox.x = x;
    containerBox.y = y + height;
    containerBox.width = width-( bEdge? size: 0);
    containerBox.height = 0;
    containerBox.addElement(newElement);
    
    bChange = true;
}

string froebelFolderElement::getText(){
    string rta = rootPath + text;
    
    for( int i = 0; i < containerBox.elements.size(); i++ ){
        if ( containerBox.elements[i]->bSelected ){
            rootPath += containerBox.elements[i]->getText();
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
        
        if ( froebelTextBox::checkMousePressed(_mouse) ){
            
            //  Every time is clicked it re-populate it content
            //
            containerBox.clear();
            ofDirectory folder(rootPath+text);
            folder.listDir();
            for(int i=0; i < (int)folder.size();i++){
                if (folder.getFiles()[i].isDirectory()){
                    
                    froebelFolderElement *newFolder = new froebelFolderElement();
                    newFolder->font = font;
                    newFolder->setSizeAndShapes(size);
                    newFolder->setText( folder.getName(i) + "/");
                    newFolder->setPrefix("  ");
                    newFolder->rootPath = rootPath + text ;
                    newFolder->fgColor.clear();
                    newFolder->fgColor.addState(5);
                    newFolder->fgColor.addState(7);
                    newFolder->fgColor.addState(4);
                    newFolder->bgColor.clear();
                    newFolder->bgColor.addState(0);
                    newFolder->bgColor.addState(2);
                    newFolder->bgColor.addState(7);
                    
                    newFolder->father = &containerBox;
                    
                    newFolder->update();
                    
                    containerBox.addElement(newFolder);
                }
            }
            
            rta = true;
        }
        
    }
//    else {
//        
//        if ( containerBox.checkMousePressed(_mouse) ){
//            
//            bChange = true;
//            bSelected = true;
//            rta = true;
//        } 
//    }
    
//    if (!rta){
        if ( containerBox.checkMousePressed(_mouse) ){
            cout << "YES" << endl;
            rta = true;
        }
//    }
    
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