//
//  froebelContainer.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/5/12.
//
//

#include "froebelContainer.h"

froebelContainer::froebelContainer(){
    maxHeight       = 200;
    
    totalBoxHeight  = 0;
    totalLenght     = 0;
    offsetY         = 0.0;
    offsetPct       = 0.0;
    damp            = 0.1;
    slider.width    = 20;
    
    bEnable         = false;
    bCheckList      = false;
    bgColor.setFromPalet(0);
}

froebelContainer::~froebelContainer(){
    for(int i = 0; i < elements.size(); i++){
        delete elements[i];
    }
    elements.clear();
}

void froebelContainer::addElement( froebelTextBox *_newElement){
    int lastY = 0;
    if (elements.size() > 0)
        lastY = elements[elements.size()-1]->y +  elements[elements.size()-1]->height;
    
    _newElement->x = x;
    _newElement->y = lastY;
    
    elements.push_back(_newElement);
}

void froebelContainer::clear(){
    for(int i = 0; i < elements.size(); i++){
        delete elements[i];
    }
    elements.clear();
    width = 0;
    height = 0;
}

void froebelContainer::reset(){
    for(int i = 0; i < elements.size(); i++){
        elements[i]->bSelected = false;
    }
}

bool froebelContainer::select(string _value){
    for(int i = 0; i < elements.size(); i++){
        if ( elements[i]->getText() == _value){
            elements[i]->bSelected = true;
            return true;
        }
    }
    return false;
}

vector<string> froebelContainer::getSelected(){
    vector<string> list;
    
    for(int i = 0; i < elements.size(); i++){
        if ( elements[i]->bSelected ){
            list.push_back( elements[i]->getText() );
        }
    }
    
    return list;
}

void froebelContainer::update(){
    bgColor.update();
    
    width = 0;
    totalBoxHeight = 0;
    
    slider.x = x;
    
    bool minWidthChange = false;
    
    for(int i = 0; i < elements.size(); i++){
        
        if (totalBoxHeight < maxHeight)
            totalBoxHeight += elements[i]->height;
        
        float elementWidth = elements[i]->getTextBoundingBox().width + elements[i]->getVerticalMargins();
        
        if (elementWidth > width){
            width = elementWidth;
            minWidthChange = true;
        }
    }
    
    if (minWidthChange){
        for(int i = 0; i < elements.size(); i++){
            elements[i]->minWidth = width;
        }
    }
    
    if (bEnable){
        //  Adjust the size
        //
        if (totalBoxHeight != height)
            height = ofLerp(height, totalBoxHeight, damp);
        
        //  Recalculate the totalLenght of the elements
        //
        totalLenght = 0;
        for(int i = 0; i < elements.size(); i++){
            totalLenght += elements[i]->height;
        }
        
        if (totalLenght > height){
            ofPoint mouse = ofPoint(ofGetMouseX(),ofGetMouseY());
            
            //  Scrolling
            //
            if (inside(mouse)){
                
                offsetPct = ofMap(mouse.y-y, 0,height,0.0,1.0,true);
                
                //  Scrolling on top and button zones
                //
                if ( offsetY > -totalLenght - height*0.5){
                    //                    float newDamp = ( 0.5+abs(offsetPct - 0.5) )*0.01;
                    //
                    //                    if ( offsetPct < 0.5 ){
                    //                        offsetY = ofLerp(offsetY, 0, newDamp);
                    //                    } else {
                    //                        offsetY = ofLerp(offsetY, (-totalLenght + box.height), newDamp);
                    //                    }
                }
                
                //  Slider Scrolling
                //
                slider.y = y + ofMap(offsetY,0,-totalLenght,0,height);
                slider.height = (height/totalLenght)*height;
                if (slider.inside(mouse)){
                    float diff = totalLenght - height;
                    offsetY = -diff * offsetPct;
                }
            }
            
        }
        
        float previusY = 0;
        for(int i = 0; i < elements.size(); i++){
            elements[i]->x = x ;
            elements[i]->y = y + previusY + offsetY;
            elements[i]->width = width;
            elements[i]->update();
            
            previusY += elements[i]->height;
        }
    } else {
        if (totalBoxHeight != height)
            height = ofLerp(height, 0.0, damp);
    }
}

void froebelContainer::draw(){
    
    ofFill();
    ofSetColor(bgColor);
    ofRect( *this );
    
    for(int i = 0; i < elements.size(); i++){
        if (inside( elements[i]->getCenter() )){
            elements[i]->draw();
        }
    }
    
    if ( (totalLenght-5 >= maxHeight) ){
        ofSetColor(bgColor.getFromPalet(2));
        ofRect(slider);
    }
    
}

bool froebelContainer::checkMousePressed(ofPoint _mouse){
    bool rta = false;
    
    if (slider.inside(_mouse)){
        rta = true;
    } else {
        
        int bClickedOn = -1;
        for(int i = 0; i < elements.size(); i++){
            
            if ( inside( elements[i]->getCenter() )){
                if (elements[i]->checkMousePressed(_mouse)){
                    bClickedOn = i;
                }
            }
        }
        
        if (bClickedOn != -1){
            if ( !bCheckList ){
                for(int i = 0; i < elements.size(); i++){
                    if ( i != bClickedOn ){
                        elements[i]->bSelected = false;
                    }
                }
            }
            rta = true;
        }
    }
    
    return rta;
}