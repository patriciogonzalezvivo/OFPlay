//
//  froebelTab.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/12/12.
//
//

#include "froebelTab.h"


froebelTab::froebelTab(){
    currentElement = 0;
    desiredElement = 0;
    countDown   = 0;

    text = "";
    buttonSize = 12;
    
    textColor.addState(ofFloatColor(0.0,0.0));
    textColor.addState(5);
    
    set(0,0,ofGetWidth(),34*2.0);
}


void froebelTab::addElement(string _title){
    froebelShapeButton newButton;
    newButton.setShape(0, buttonSize);
    newButton.color.addState(ofFloatColor(0.686274));
    newButton.color.addState(0);
    newButton.color.addState(0);
    newButton.text = _title;
    newButton.font = &font;
    newButton.textColor.set(0.0,0.0);
    buttons.push_back(newButton);
    
    float totalWidth = buttons.size()*2.0*buttonSize;
    for(int i = 0; i < buttons.size(); i++){
        buttons[i].x = - totalWidth*0.5 + buttonSize + buttonSize*i*2.0;
        buttons[i].y = 0;//y + height + buttonSize*0.5;
    }
    
    desiredElement = 1;
    setElement(0);
}

void froebelTab::setPrev(){
    if ( currentElement > 0){
        setElement(currentElement-1);
    }
}

void froebelTab::setNext(){
    setElement(currentElement+1);
}

void froebelTab::setElement( unsigned int _nElement){
    if ( _nElement < size() ){
        
        if (_nElement != desiredElement ){
            textColor.setState(0);
            desiredElement = _nElement;
            countDown = ofGetFrameRate();
            
            for(int i = 0; i < buttons.size(); i++){
                if (i == desiredElement){
                    buttons[i].bEnable = false;
                } else {
                    buttons[i].bEnable = true;
                }
            }
        }
        
    }
}

bool froebelTab::checkMousePressed(ofPoint _mouse){
    bool rta = false;
    
    _mouse.x -= x+width*0.5;
    _mouse.y -= y+height;
    
    for(int i = 0; i < buttons.size(); i++){
        if (buttons[i].checkMousePressed(_mouse)){
            rta = true;
            setElement(i);
            break;
        }
    }
    
    return rta;
}

void froebelTab::update(){
    
    if ( buttons.size() > 0 ){
        textColor.update();
        
        if (countDown == 0){
            text = buttons[desiredElement].text;
            currentElement = desiredElement;
            countDown = -1;
            textColor.setState(1);
        } else if ( countDown > 0){
            countDown--;
        }
        
        for(int i = 0; i < buttons.size(); i++){
            buttons[i].update();
        }
    }
}

void froebelTab::draw(){
    ofPushStyle();
    ofPushMatrix();
    
    ofTranslate(x+width*0.5, y+height*0.5);
    ofSetColor(textColor);
    ofRectangle textBox = font.getStringBoundingBox(text, 0, 0);
    font.drawString(text, -textBox.width*0.5, textBox.height*0.5);
    
    ofTranslate(0, height*0.5);
    ofFill();
    ofSetColor(255);
    for(int i = 0; i < buttons.size(); i++){
        buttons[i].draw();
    }
    
    ofPopMatrix();
    
    ofPushStyle();
}