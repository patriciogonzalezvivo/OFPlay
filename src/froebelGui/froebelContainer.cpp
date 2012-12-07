//
//  froebelContainer.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/5/12.
//  http://www.patriciogonzalezvivo.com
//

#include "froebelContainer.h"

froebelContainer::froebelContainer(){
    maxHeight       = 200;
    
    totalBoxHeight  = 0;
    totalLenght     = 0;
    offsetY         = 0.0;
    damp            = 0.1;
    size            = 30;
    slider.width    = 5;
    
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
    
    adjustShape();
}

void froebelContainer::adjustShape(){
    width = 0;
    totalBoxHeight = 0;
    bool minWidthChange = false;
    for(int i = 0; i < elements.size(); i++){
        
        if (totalBoxHeight < maxHeight)
            totalBoxHeight += elements[i]->height;
        
        float elementWidth = elements[i]->getTextBoundingBox().width;
        
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
        elements[i]->reset();
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

    slider.x = x;
    
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
            if ( inside(mouse) ){
    
//                if ( bCheckList || (getSelected().size() == 0) ){
                
                    float offsetPct = ofMap(mouse.y-y, 0,height,0.0,1.0,true);
                    float diff = totalLenght - height;
                    offsetY = ofLerp(offsetY, -diff * offsetPct, damp);
                    
                    //  Slider Scrolling
                    //
                    slider.y = y + ofMap(offsetY,0,-totalLenght,0,height);
                    slider.height = (height/totalLenght)*height;
//                }
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