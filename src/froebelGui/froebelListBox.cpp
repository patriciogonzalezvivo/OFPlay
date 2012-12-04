//
//  froebelListBox.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//  Copyright (c) 2012 http://www.patriciogonzalezvivo.com All rights reserved.
//

#include "froebelListBox.h"

froebelListBox::froebelListBox(){
    subInfo     = NULL;
    
    bLeftAlign  = true;
    bChange     = true;
    bEdge       = false;
    bIcon       = false;
    bSelected   = false;
    bFixedSize  = false;
    
    text        = "";
    prefix      = "";
    deliminater = "";
    
    maxWidth    = 600;
    size        = 40;
    nState      = 0;
    damp        = 0.1;
    
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
    
    backgroundColor.setFromPalet(0);
}

void froebelListBox::addElement(string _value, bool _defVal, int _iconShape){
    
    int lastY = 0;
    if (elements.size() > 0)
        lastY = elements[elements.size()-1]->y +  elements[elements.size()-1]->height;
        
    //  Add element to the vector
    //
    froebelTextBox *newElement = new froebelTextBox();
    newElement->x = x;
    newElement->y = lastY;
    
    if ( _iconShape == -1 )
        newElement->setSizeAndShapes(size,3);
    else
        newElement->setSizeAndShapes(size,3,_iconShape);
        
    newElement->setText(_value);
    newElement->setPrefix(" ");
    newElement->font = font;
    newElement->bSelected = _defVal;
    newElement->bFixedSize = true;
    
    newElement->fgColor.clear();
    newElement->bgColor.clear();
    
    //  STATE_PASSIVE
    //
    newElement->fgColor.addState(5);
    newElement->bgColor.addState(0);//addStateAsPointer(&backgroundColor);
    
    //  STATE_HOVER
    //
    newElement->fgColor.addState(7);
    newElement->bgColor.addState(2);
    
    //  STATE_ACTIVE
    //
    newElement->fgColor.addState(4);
    newElement->bgColor.addState(7);
    
    elements.push_back(newElement);
    
    //  recalculate the bounding box
    //
    box.x = x;
    box.y = y + size;
    box.width = width - size*0.5;
    box.height = 0;
    totalBoxHeight = 0;
    
    if (bEdge)
        box.width -= size*0.5;
    
    for(int i = 0; i < elements.size(); i++){
        if (totalBoxHeight < maxHeight)
            totalBoxHeight += elements[i]->height;
    }
    
}

void froebelListBox::clear(){
    for(int i = 0; i < elements.size(); i++){
        delete elements[i];
    }
    elements.clear();
}

void froebelListBox::reset(){
    for(int i = 0; i < elements.size(); i++){
        elements[i]->bSelected = false;
    }
}

bool froebelListBox::select(string _value){
    for(int i = 0; i < elements.size(); i++){
        if ( elements[i]->getText() == _value){
            elements[i]->bSelected = true;
            bChange = true;
            return true;
        }
    }
    return false;
}

ofRectangle froebelListBox::getBoundingBox(){
    ofRectangle rta;
    
    rta.set(*this);
    
    if (subInfo != NULL)
        rta.growToInclude(*subInfo);
    
    rta.growToInclude(box);
    
    return rta;
}

vector<string> froebelListBox::getSelected(){
    vector<string> list;
    
    for(int i = 0; i < elements.size(); i++){
        if ( elements[i]->bSelected ){
            list.push_back( elements[i]->getText() );
        }
    }
    
    return list;
}

string froebelListBox::getSelectedAsString(){
    string list;
    
    for (int i = 0; i < elements.size(); i++){
        if ( elements[i]->bSelected ){
            
            if (list.length() > 0)
                list += deliminater;
            
            list += elements[i]->getText();
        }
    }
    
    return list;
}

bool froebelListBox::checkMousePressed(ofPoint _mouse){
    
    if ( !bSelected ){ 
        
        return froebelTextBox::checkMousePressed(_mouse);
        
    } else {
        
        if ( box.inside(_mouse) ){
            
            for(int i = 0; i < elements.size(); i++){
                if (box.inside( elements[i]->getCenter() )){
                    if (elements[i]->checkMousePressed(_mouse)){
                        text = getSelectedAsString();
                        bChange = true;
                        bSelected = true;
                        return true;
                    }
                }
            }
            
            return false;
        }
    }
}

void froebelListBox::update(){
    if ( bChange ){
        totalBoxHeight = 0;
        
        for(int i = 0; i < elements.size(); i++){
            if (totalBoxHeight < maxHeight)
                totalBoxHeight += elements[i]->height;
        }
    }
    
    froebelTextBox::update();
    backgroundColor.update();
    
    if (bSelected){
        //  recalculate the bounding box
        //
        box.x = x;
        box.y = y + size;
        
        if (totalBoxHeight != box.height)
            box.height = ofLerp(box.height, totalBoxHeight, damp);
    
        float totalLenght = 0;
        float offSetY = 0;
        for(int i = 0; i < elements.size(); i++){
            totalLenght += elements[i]->height;
        }
        
        if (totalLenght > box.height){
            if (box.inside(ofGetMouseX(),ofGetMouseY())){
                float pct = ofMap(ofGetMouseY()- box.y, size*0.5, box.height, 0,1,true);
                float diff = totalLenght - box.height;
                mouseOffSet = ofLerp( mouseOffSet, -diff * pct, damp*0.5);
            }
        }
        
        for(int i = 0; i < elements.size(); i++){
            elements[i]->x = box.x;
            elements[i]->y = box.y + offSetY + mouseOffSet;
            elements[i]->width = box.width + size*0.5;
            elements[i]->update();
            
            offSetY += elements[i]->height;
        }
    } else {
        if (totalBoxHeight != box.height)
            box.height = ofLerp(box.height, 0, damp*0.5);
    }
}


void froebelListBox::draw(){
    
    if (bEdge)
        endingShape.color.set(bgColor);
    
    if (bIcon)
        iconShape.color.set(fgColor);
    
    
    //  Render Elements
    //
    if ( bSelected ){
        ofFill();
        ofSetColor(backgroundColor);
        ofRect(box);
        
        for(int i = 0; i < elements.size(); i++){
            
            if (box.inside( elements[i]->getCenter() )){
                elements[i]->draw();
            }
        }
    }
    
    froebelTextBox::draw();
}