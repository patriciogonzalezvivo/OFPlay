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
    
    setActiveColors(3, 4);
    setPasiveColors(2, 5);
    
    bgColor = bgDstColor = fgColor = fgDstColor = froebelColor(0);
    
    backgroundColor = ofColor(230.f);
    
    bSelected   = false;
    bLeftAlign  = true;
    bChange     = false;
    
    bEdge       = false;
    bIcon       = false;
    
    text        = "";
    prefix      = "";
    deliminater = "";
    
    maxWidth = 300;
    maxHeight = 200;
    size = 40;
    damp = 0.1;
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
    newElement->font = font;
    newElement->bSelected = _defVal;
    newElement->bgPasiveColor = backgroundColor;
    newElement->bgActiveColor = froebelColor(7);
    newElement->fgPasiveColor = froebelColor(5);
    newElement->fgActiveColor = froebelColor(4);
    
    elements.push_back(newElement);
    
    //  recalculate the bounding box
    //
    box.x = x;
    box.y = y + size;
    box.width = width;
    box.height = 0;

    if (bEdge)
        box.width -= size*0.5;
    
    for(int i = 0; i < elements.size(); i++){
        if (box.height < maxHeight)
            box.height += elements[i]->height;
    }
    
    box.height += size*0.5;
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
                        return true;
                    }
                }
            }
            
            return false;
        }
    }
}

void froebelListBox::update(){
    
    froebelTextBox::update();
    
    if (bSelected){
        //  recalculate the bounding box
        //
        box.x = x;
        box.y = y + size;
    
        float totalLenght = 0;
        float offSetY = 0;
        for(int i = 0; i < elements.size(); i++){
            totalLenght += elements[i]->height;
        }
        
        if (totalLenght > box.height){
            if (box.inside(ofGetMouseX(),ofGetMouseY())){
                float pct = ofMap(ofGetMouseY()- box.y, size*0.5, box.height, 0,1,true);
                float diff = totalLenght - box.height;
                mouseOffSet = ofLerp( mouseOffSet, -diff * pct, damp);
            }
        }
        
        for(int i = 0; i < elements.size(); i++){
            elements[i]->x = box.x;
            elements[i]->y = box.y + offSetY + mouseOffSet;
            elements[i]->width = box.width;
            elements[i]->update();
            
            offSetY += elements[i]->height;
        }
    }
}


void froebelListBox::draw(){
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