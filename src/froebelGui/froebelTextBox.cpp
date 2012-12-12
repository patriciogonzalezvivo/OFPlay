//
//  froebelTextBox.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//  http://www.patriciogonzalezvivo.com
//

#include "froebelTextBox.h"

froebelTextBox::froebelTextBox(){
    subInfo     = NULL;
    
    bFill       = true;
    bLeftAlign  = true;
    bChange     = true;
    bSelected   = false;
    bFixedSize  = false;
    bEdge       = false;
    bIcon       = false;
    
    text        = "";
    prefix      = "";
    deliminater = "";
    
    minWidth    = 0;
    maxWidth    = 600;
    size        = 40;
    nState      = 0;
    
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
    fgColor.addState(5);
    bgColor.addState(4);
}

void froebelTextBox::setSizeAndShapes(float _size, int _endingShape, int _iconShape){
    size = _size;
    height = size;
    
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

void froebelTextBox::setSubInfo(string _comment){
    if (subInfo == NULL){
        subInfo = new froebelTextBox();
        subInfo->x = x;
        subInfo->y = y;
        subInfo->width = ofGetWidth() - size;
        subInfo->height = size;
        subInfo->font = font;
        subInfo->bFixedSize = true;
        subInfo->bLeftAlign = false;
        subInfo->setPrefix("<< ");
        subInfo->setText( _comment );
        subInfo->setSizeAndShapes(size,1);
        subInfo->fgColor.clear();
        subInfo->fgColor.addState(5);
        subInfo->fgColor.addState(7);
        subInfo->fgColor.addState(5);
        subInfo->bgColor.clear();
        subInfo->bgColor.addState(0);
        subInfo->bgColor.addState(2);
        subInfo->bgColor.addState(0);
    } else {
        subInfo->setText( _comment );
    }
}

void froebelTextBox::setPrefix( string _prefix ){
    prefix = _prefix;
    bChange = true;
}

void froebelTextBox::setDivider( string _deliminater ){
    deliminater = _deliminater;
    bChange = true;
}

void froebelTextBox::setText(string _text ){
    text = _text;
    bChange = true;
}

void froebelTextBox::reset(){
    bSelected = false;
    bChange = true;
}

string froebelTextBox::getText(){
    return text;
}

ofRectangle froebelTextBox::getBoundingBox(){
    ofRectangle rta;
    
    rta.set(*this);
    
    if (subInfo != NULL)
        rta.growToInclude(*subInfo);
    
    return rta;
}

float froebelTextBox::getVerticalMargins(){
    float margins = size;
    
    if ( bEdge )
        margins += size*0.5;
    
    if ( bIcon)
        margins += size*0.5;
    
    return margins;
}

ofRectangle froebelTextBox::getTextBoundingBox(){
    return textBox;
}

void froebelTextBox::update(){
    //  Update dependences
    //
    if (subInfo != NULL){
        subInfo->x = x;
        subInfo->y = y;
        subInfo->update();
    }
    
    //  Update STATE
    //
    if( bSelected ){
        nState = STATE_ACTIVE;
    } else {
        if (inside(ofGetMouseX(), ofGetMouseY()) ){
            nState= STATE_HOVER;
        } else {
            nState= STATE_PASIVE;
        }
    }
    
    //  Update Colors
    //
    fgColor.setState(nState);
    bgColor.setState(nState);
    fgColor.update();
    bgColor.update();
    
    if (bEdge)
        endingShape.color.set(bgColor);
    
    if (bIcon)
        iconShape.color.set(fgColor);
    
    //  Update Shape
    //
    if (bChange && font != NULL){
        //  Compose text
        //
        displayText = prefix + text;
        nEdges = 1;
        
        //  Calculate the size of the text
        //
        textBox = font->getStringBoundingBox( displayText , 0, 0);
        textBox.width += getVerticalMargins();
        
        bool doReScale = true;
        
        if ( bFixedSize){
            if (textBox.width < width)
                doReScale = false;
        } 
        
        if (doReScale){
            
            if ( textBox.width <= maxWidth ){
            
                //  If it less that the max adjust the shape
                //
                if (textBox.width <= minWidth){
                    width = minWidth;
                } else {
                    width = textBox.width;
                }
                
                height = size;
                
            } else {
                
                //  other wise break
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
                
                //  Apply the breaking
                //
                textBox = font->getStringBoundingBox( displayText , 0, 0);
                textBox.width + getVerticalMargins();
                width = textBox.width;
                height = size;
                if ( textBox.height+size*0.5 > size ) {
                    while ( textBox.height+size > size*nEdges ) {
                        nEdges++;
                        height = size*nEdges;
                    }
                }
                
            }
        }
        
        bChange = false;
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
    
    if(bFill)
        ofFill();
    else
        ofNoFill();
    
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
    
    if (font != NULL){
        
        ofSetColor(fgColor);
        if (bLeftAlign){
            font->drawString(displayText, size*0.5, size-13);
        } else {
            font->drawString(displayText, width-(-size*0.5)-textBox.width, size-13);
        }
        
    }
    
    ofPopMatrix();
    ofPopStyle();
    
}

bool froebelTextBox::checkMousePressed(ofPoint _mouse){
    if (inside(_mouse)){
        bSelected = !bSelected;
        nState = STATE_ACTIVE;
        return true;
    }
    return false;
}