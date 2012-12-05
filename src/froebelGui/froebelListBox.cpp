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
    
    minWidth        = 0;
    maxWidth        = 600;
    maxHeight       = 200;
    
    totalBoxHeight  = 0;
    totalLenght     = 0;
    offsetY         = 0.0;
    offsetPct       = 0.0;
    size            = 40;
    nState          = 0;
    damp            = 0.1;
    
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

void froebelListBox::addElement(string _value, bool _defVal, int _iconShape, int _edgeCoorner){
    int lastY = 0;
    if (elements.size() > 0)
        lastY = elements[elements.size()-1]->y +  elements[elements.size()-1]->height;
        
    //  Add element to the vector
    //
    froebelTextBox *newElement = new froebelTextBox();
    newElement->x = x;
    newElement->y = lastY;
    newElement->font = font;
    newElement->setSizeAndShapes(size,_edgeCoorner,_iconShape);
    newElement->setText(_value);
    newElement->setPrefix("  ");
    newElement->bSelected = _defVal;
    
    //  Defaul Element Colors
    //
    newElement->fgColor.clear();
    newElement->fgColor.addState(5);
    newElement->fgColor.addState(7);
    newElement->fgColor.addState(4);
    newElement->bgColor.clear();
    newElement->bgColor.addState(0);
    newElement->bgColor.addState(2);
    newElement->bgColor.addState(7);
    
    newElement->update();
    
    elements.push_back(newElement);
    
    //  recalculate the container box
    //
    conteinerBox.x = x;
    conteinerBox.y = y + height;
    conteinerBox.width = width-( bEdge? size: 0);
    conteinerBox.height = 0;
    
    bChange = true;
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
    
    rta.growToInclude(conteinerBox);
    
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
        
        if ( conteinerBox.inside(_mouse) ){
            
            if (slider.inside(_mouse)){
                return true;
            } else {
                for(int i = 0; i < elements.size(); i++){
                    if (conteinerBox.inside( elements[i]->getCenter() )){
                        if (elements[i]->checkMousePressed(_mouse)){
                            text = getSelectedAsString();
                            bChange = true;
                            bSelected = true;
                            return true;
                        }
                    }
                }
            }
        }
        
    }
        
    return false;
}

void froebelListBox::updateContainerBoxSize(){
    conteinerBox.x = x;
    conteinerBox.y = y + height;
    conteinerBox.width = 0;
    totalBoxHeight = 0;
    
    slider.x = conteinerBox.x;
    slider.width = size*0.5;
    
    bool minWidthChange = false;
    
    for(int i = 0; i < elements.size(); i++){
        
        if (totalBoxHeight < maxHeight)
            totalBoxHeight += elements[i]->height;
        
        float elementWidth = elements[i]->getTextBoundingBox().width;
        
        if (elementWidth > conteinerBox.width){
            conteinerBox.width = elementWidth;
            minWidthChange = true;
        }
    }
    
    if (minWidthChange){
        for(int i = 0; i < elements.size(); i++){
            elements[i]->minWidth = conteinerBox.width;
        }
    }
}

void froebelListBox::update(){
    if ( bChange ){
        conteinerBox.x = x;
        conteinerBox.y = y + height;
        conteinerBox.update();
        
        minWidth = conteinerBox.width + size;
    }
    
    froebelTextBox::update();
    backgroundColor.update();
    
    if (bSelected){
        //  Adjust the size of the box that contain the 
        //
        if (totalBoxHeight != conteinerBox.height)
            conteinerBox.height = ofLerp(conteinerBox.height, totalBoxHeight, damp);
        
        //  Recalculate the totalLenght of the elements
        //
        totalLenght = 0;
        for(int i = 0; i < elements.size(); i++){
            totalLenght += elements[i]->height;
        }
        
        if (totalLenght > conteinerBox.height){
            ofPoint mouse = ofPoint(ofGetMouseX(),ofGetMouseY());
            
            //  Scrolling
            //
            if (conteinerBox.inside(mouse)){
                
                offsetPct = ofMap(mouse.y-conteinerBox.y, size*0.5,conteinerBox.height,0.0,1.0,true);
                
                //  Scrolling on top and button zones 
                //
                if ( offsetY > -totalLenght-conteinerBox.height*0.5){
//                    float newDamp = ( 0.5+abs(offsetPct - 0.5) )*0.01;
//                    
//                    if ( offsetPct < 0.5 ){
//                        offsetY = ofLerp(offsetY, 0, newDamp);
//                    } else {
//                        offsetY = ofLerp(offsetY, (-totalLenght + box.height), newDamp);
//                    }
                    float diff = totalLenght - conteinerBox.height;
                    offsetY = ofLerp(offsetY,-diff * offsetPct,0.01);
                }
                
                //  Slider Scrolling
                //
                slider.y = y + height + ofMap(offsetY,0,-totalLenght,0,conteinerBox.height);
                slider.height = (conteinerBox.height/totalLenght)*conteinerBox.height;
                if (slider.inside(mouse)){
                    float diff = totalLenght - conteinerBox.height;
                    offsetY = -diff * offsetPct;
                }
            }
            
        }
        
        float previusY = 0;
        for(int i = 0; i < elements.size(); i++){
            elements[i]->x = conteinerBox.x ;
            elements[i]->y = conteinerBox.y + previusY + offsetY;
            elements[i]->width = conteinerBox.width + size*0.5;
            elements[i]->update();
            
            previusY += elements[i]->height;
        }
    } else {
        if (totalBoxHeight != conteinerBox.height)
            conteinerBox.height = ofLerp(conteinerBox.height, 0, damp*0.5);
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
        ofRect(conteinerBox);
        
        for(int i = 0; i < elements.size(); i++){
            
            if (conteinerBox.inside( elements[i]->getCenter() )){
                elements[i]->draw();
            }
        }
    }
    
    if ((totalLenght-5 >= maxHeight) && bSelected ){
        ofSetColor(bgColor.getFromPalet(2));
        ofRect(slider);
    }
    
    froebelTextBox::draw();
}