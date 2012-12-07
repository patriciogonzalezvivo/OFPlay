//
//  froebelShapeButton.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/7/12.
//  http://www.patriciogonzalezvivo.com
//

#include "froebelShapeButton.h"

froebelShapeButton::froebelShapeButton(){
    size = 40;
    type = 0;
    color.set(1.0f);
    
    text = "";
    textColor.set(0.0);
    font = NULL;
    
    bEnable = true;
}

void froebelShapeButton::update(){
    ofPoint mouse(ofGetMouseX()-x, ofGetMouseY()-y);
    
    if ( bEnable ){
        if ( shape.inside(mouse) ){
            nState = STATE_HOVER;
        } else {
            nState = STATE_ENABLE;
        }
    } else {
        nState = STATE_DISABLE;
    }

    color.setState(nState);
    textColor.setState(nState);
}

void froebelShapeButton::draw(){
    froebelShape::draw();
    textColor.update();
    
    if (font != NULL){
        
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(x, y);
        ofSetColor(textColor);
        
        ofRectangle textBoundingBox = font->getStringBoundingBox( text , 0, 0);
        font->drawString(text, -textBoundingBox.width*0.5, textBoundingBox.height*0.5);
        
        ofPopStyle();
        ofPopMatrix();
    }
}

bool froebelShapeButton::checkMousePressed(ofPoint _mouse){
    if ( bEnable ){
        _mouse.x -= x;
        _mouse.y -= y;
        
        if ( shape.inside(_mouse)){
            ofNotifyEvent(clicked, text);
            return true;
        }
    }
    return false;
}