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
    size            = 40;
    nState          = 0;
    
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
    conteinerBox.x = x;
    conteinerBox.y = y + height;
    conteinerBox.width = width-( bEdge? size: 0);
    conteinerBox.height = 0;
    conteinerBox.addElement(newElement);
    
    bChange = true;
}

ofRectangle froebelListBox::getBoundingBox(){
    ofRectangle rta;
    
    rta.set(*this);
    
    if (subInfo != NULL)
        rta.growToInclude(*subInfo);
    
    rta.growToInclude(conteinerBox);
    
    return rta;
}

bool froebelListBox::select(string _value){
    return bChange = conteinerBox.select(_value);
}


string froebelListBox::getSelectedAsString(){
    string list;
    
    for (int i = 0; i < conteinerBox.elements.size(); i++){
        if ( conteinerBox.elements[i]->bSelected ){
            
            if (list.length() > 0)
                list += deliminater;
            
            list += conteinerBox.elements[i]->getText();
        }
    }
    
    return list;
}

bool froebelListBox::checkMousePressed(ofPoint _mouse){
    
    if ( !bSelected ){ 
        
        return froebelTextBox::checkMousePressed(_mouse);
        
    } else {
        
        if ( conteinerBox.checkMousePressed(_mouse)){
            text = getSelectedAsString();
            bChange = true;
            bSelected = true;
            return true;
        }
    }
}

void froebelListBox::update(){
    if ( bChange ){
        minWidth = conteinerBox.width + size * 0.5;
    }
    
    froebelTextBox::update();
    
    conteinerBox.x = x;
    conteinerBox.y = y + height;
    conteinerBox.bEnable = bSelected;
    conteinerBox.update();
    
}

void froebelListBox::draw(){
    
    if (bEdge)
        endingShape.color.set(bgColor);
    
    if (bIcon)
        iconShape.color.set(fgColor);
    
    
    //  Render Elements
    //
    if ( bSelected ){
        conteinerBox.draw();
    }
    
    froebelTextBox::draw();
}