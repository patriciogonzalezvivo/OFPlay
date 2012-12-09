//
//  froebelListBox.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//  http://www.patriciogonzalezvivo.com
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
    size            = 40;
    nState          = 0;
    
    containerBox.bCheckList = true;
    
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

void froebelListBox::addElement(string _value, bool _defVal, int _iconShape, int _edgeCoorner){
    
    //  Create new element
    //
    froebelTextBox *newElement = new froebelTextBox();
    newElement->font = font;
    newElement->setSizeAndShapes(size,_edgeCoorner,_iconShape);
    newElement->setText(_value);
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
    containerBox.size = size;
    containerBox.addElement(newElement);
    
    bChange = true;
}

ofRectangle froebelListBox::getBoundingBox(){
    ofRectangle rta;
    
    rta.set(*this);
    
    if (subInfo != NULL)
        rta.growToInclude(*subInfo);
    
    rta.growToInclude(containerBox);
    
    for (int i = 0; i < containerBox.elements.size(); i++){
        
        if ((containerBox.elements[i]->bSelected) &&
            (containerBox.inside(containerBox.elements[i]->getCenter())) ){
                rta.growToInclude( containerBox.elements[i]->getBoundingBox() );
        }
        
    }
    
    return rta;
}

void froebelListBox::reset(){
    froebelTextBox::reset();
    containerBox.reset();
}

bool froebelListBox::select(string _value){
    return bChange = containerBox.select(_value);
}

string froebelListBox::getSelectedAsString(){
    string list;
    
    for (int i = 0; i < containerBox.elements.size(); i++){
        if ( containerBox.elements[i]->bSelected ){
            
            if (list.length() > 0)
                list += deliminater;
            
            list += containerBox.elements[i]->getText();
        }
    }
    
    return list;
}

bool froebelListBox::checkMousePressed(ofPoint _mouse){
    
    if ( !bSelected ){ 
        
        return froebelTextBox::checkMousePressed(_mouse);
        
    } else {
        
        if ( containerBox.checkMousePressed(_mouse)){
            text = getSelectedAsString();
            bChange = true;
            bSelected = true;
            return true;
        } else {
            if (!inside(_mouse)){
                string rta = getText();
                ofNotifyEvent(focusLost, rta);
            } else {
                bSelected = !bSelected;
            }
        }
    }
}

void froebelListBox::update(){
    if ( bChange ){
        minWidth = containerBox.width + size * 0.5;
    }
    
    froebelTextBox::update();
    
    containerBox.x = x;
    containerBox.y = y + height;
    containerBox.bEnable = bSelected;
    containerBox.update();
    
}

void froebelListBox::draw(){
    
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