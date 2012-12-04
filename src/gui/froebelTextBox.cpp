//
//  froebelTextBox.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//  Copyright (c) 2012 http://www.patriciogonzalezvivo.com All rights reserved.
//

#include "froebelTextBox.h"

froebelTextBox::froebelTextBox(){
    subInfo     = NULL;
    
    setActiveColors(3, 4);
    setPasiveColors(2, 5);
    
    bgColor = bgDstColor = fgColor = fgDstColor = froebelColor(0);
    
    bSelected   = false;
    bLeftAlign  = true;
    bChange     = true;
    
    bEdge = false;
    bIcon = false;
    
    text = "";
    prefix = "";
    deliminater = "";
    
    maxWidth = 600;
    size = 40;
    damp = 0.1;
}

void froebelTextBox::setActiveColors(int _fg, int _bg){
    fgActiveColor = froebelColor(_fg);
    bgActiveColor = froebelColor(_bg);
}

void froebelTextBox::setPasiveColors(int _fg, int _bg){
    fgPasiveColor = froebelColor(_fg);
    bgPasiveColor = froebelColor(_bg);
}

void froebelTextBox::setSizeAndShapes(float _size, int _endingShape, int _iconShape){
    size = _size;
    height = size;
    width = size;
    
    if (_endingShape != -1){
        bEdge = true;
        endingShape.setShape(_endingShape, _size);
    }
    
    
    if (_iconShape != -1){
        bIcon = true;
        iconShape.setShape(_iconShape, _size*0.5);
    }
    
    bChange = true;
}

void froebelTextBox::setText(string _text ){
    text = _text;
    bChange = true;
}

string froebelTextBox::getText(){
    return text;
}

void froebelTextBox::setPrefix( string _prefix ){
    prefix = _prefix;
    bChange = true;
}

void froebelTextBox::setDivider( string _deliminater ){
    deliminater = _deliminater;
    bChange = true;
}

bool froebelTextBox::checkMousePressed(ofPoint _mouse){
    if (inside(_mouse)){
        bSelected = !bSelected;
        return true;
    }
    return false;
}

void froebelTextBox::update(){
    //  Update dependences
    //
    if (subInfo != NULL)
        subInfo->update();
    
    
    //  Selection and Hover colors
    //
    if( bSelected ){
        bgDstColor = bgActiveColor;
        fgDstColor = fgActiveColor;
        
        if (bEdge)
            endingShape.dstColor = bgActiveColor;
        
        if (bIcon)
            iconShape.dstColor = fgActiveColor;
        
    } else {
        if (inside(ofGetMouseX(), ofGetMouseY()) ){
            fgDstColor = bgActiveColor;
            
            if (bIcon)
                iconShape.dstColor = bgActiveColor;
            
        } else {
            fgDstColor = fgPasiveColor;
            
            if (bIcon)
                iconShape.dstColor = fgPasiveColor;
        }
        bgDstColor = bgPasiveColor;
        
        if (bEdge)
            endingShape.dstColor = bgPasiveColor;
    }
    
    if (bgColor != bgDstColor){
        bgColor.lerp(bgDstColor, damp);
    }
    
    if (fgColor != fgDstColor){
        fgColor.lerp(fgDstColor, damp);
    }
    
    if (bChange){
        //  Compose text
        //
        displayText = prefix + text;
        nEdges = 1;
        
        //  Calculate the size of the text
        //
        textBox = font->getStringBoundingBox( displayText , 0, 0);
        margins = size;
        
        if ( bEdge )
            margins += size*0.5;
        
        if ( bIcon)
            margins += size*0.5;
        
        if ( textBox.width + margins >= width ){
            if ( textBox.width + margins <= maxWidth ){
                
                //  If it less that the max adjust the shape
                //
                width = textBox.width + margins;
                
            } else {
                
                //  other wise break it baby
                //
                string _newText = displayText;
                vector < string > breakUp;
                breakUp = ofSplitString(_newText, deliminater);
                
                ofPoint pos;
                pos.set(0,0);
                
                displayText = "";
                
                for (int i = 0; i < breakUp.size(); i++){
                    string text = breakUp[i];
                    if (i != breakUp.size() -1) text += deliminater;
                    
                    ofRectangle rect = font->getStringBoundingBox(text, pos.x, pos.y);
                    
                    if ((pos.x + rect.width) > maxWidth ){
                        displayText += "\n";
                        displayText += text;
                        pos.x = rect.width;
                    } else {
                        displayText+= text;
                        pos.x += rect.width;
                    }
                }
            }
            
            textBox = font->getStringBoundingBox( displayText , 0, 0);
            width = textBox.width + margins ;
            
            if ( textBox.height+size*0.5 > size ) {
                while ( textBox.height+size > size*nEdges ) {
                    nEdges++;
                    height = size*nEdges;
                }
            }
            
            bChange = false;
        }
    }
}

void froebelTextBox::draw(){
    
    //  If it have a sub textBox, draw it first
    //
    if (subInfo != NULL)
        subInfo->draw();
    
    //  Render
    //
    ofRectangle smallBox;
    smallBox.set(*this);
    smallBox.width = width;
    smallBox.height = height ;

    if ( bEdge ){
         smallBox.width -= size*0.5;
    }
    
    ofPushMatrix();
    ofPushStyle();
    
    ofFill();
    ofSetColor(bgColor);
    ofRect(smallBox);
    ofTranslate(x, y);
    
    if ( bEdge ){
        for(int i = 0; i < nEdges; i++){
            endingShape.set(smallBox.width,size*0.5+size*i);
            endingShape.draw();
        }
    }
    
    if ( bIcon ){
        iconShape.set( width-size*0.5 ,size*0.5);
        iconShape.draw();
    }
    
    ofSetColor(fgColor);
    
    if (bLeftAlign){
        font->drawString(displayText, size*0.5, size-13);
    } else {
        font->drawString(displayText, width-(margins-size*0.5)-textBox.width, size-13);
    }
        
    ofPopMatrix();
    ofPopStyle();
    
}

ofFloatColor froebelTextBox::froebelColor(int _color){
    ofFloatColor color;
    
    switch (_color) {
        case 0:
            color = ofColor(220);
            break;
        case 1:
            color = ofColor(4, 49, 7);
            break;
        case 2:
            color = ofColor(220, 202, 185);
            break;
        case 3:
            color = ofColor(186, 1, 23);
            break;
        case 4:
            color = ofColor(247, 181, 55);
            break;
        case 5:
            color = ofColor(64, 79, 122);
            break;
        case 6:
            color = ofColor(62, 2, 35);
            break;
        case 7:
            color = ofColor(193, 66, 11);
            break;
        default:
            color = ofColor(0);
            break;
    }
    
    return color;
};