//
//  TextSequencer.cpp
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "TextSequencer.h"

TextSequencer::TextSequencer(){
    currentLine = 0;
    desiredLine = 0;
    countDown   = 0;
    
    text        = NULL;
    textColor.addState(ofFloatColor(0.0,0.0));
    textColor.addState(5);
    bFinish  = false;
}

bool TextSequencer::loadSequence(string _xmlFile){
    bool    success = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_xmlFile)){
        
        script.clear();
        buttons.clear();
        
        float buttonSize = 20;
        
        XML.pushTag("textSequence");
        
        string defVerAlign = XML.getValue("default:vAlign", "MIDDLE");
        if (defVerAlign == "TOP"){
            defaultVertAlign = OF_TEXT_ALIGN_TOP;
        } else if ( defVerAlign == "BOTTOM"){
            defaultVertAlign = OF_TEXT_ALIGN_BOTTOM;
        } else if ( defVerAlign == "MIDDLE"){
            defaultVertAlign = OF_TEXT_ALIGN_MIDDLE;
        }
        
        string defHorAlign = XML.getValue("default:hAlign", "CENTER");
        if (defHorAlign == "LEFT"){
            defaultHoriAlign = OF_TEXT_ALIGN_LEFT;
        } else if ( defHorAlign == "RIGHT"){
            defaultHoriAlign = OF_TEXT_ALIGN_RIGHT;
        } else if ( defHorAlign == "JUSTIFIED"){
            defaultHoriAlign = OF_TEXT_ALIGN_JUSTIFIED;
        } else if ( defHorAlign == "CENTER"){
            defaultHoriAlign = OF_TEXT_ALIGN_CENTER;
        }
        
        int totalLines = XML.getNumTags("phrase");
        for(int i = 0; i < totalLines; i++){
            XML.pushTag("phrase",i);
            textPhrase newPhrase;
            
            //  Text Phrase
            //
            newPhrase.text  = XML.getValue("text", "NO TEXT FOUND");
            
            string alignment = XML.getValue("hAlign", defHorAlign);
            if (alignment == "LEFT"){
                newPhrase.hAlign = OF_TEXT_ALIGN_LEFT;
            } else if ( alignment == "RIGHT"){
                newPhrase.hAlign = OF_TEXT_ALIGN_RIGHT;
            } else if ( alignment == "JUSTIFIED"){
                newPhrase.hAlign = OF_TEXT_ALIGN_JUSTIFIED;
            } else if ( alignment == "CENTER"){
                newPhrase.hAlign = OF_TEXT_ALIGN_CENTER;
            }
            
            alignment = XML.getValue("vAlign", defVerAlign);
            if (alignment == "TOP"){
                newPhrase.vAlign = OF_TEXT_ALIGN_TOP;
            } else if ( alignment == "BOTTOM"){
                newPhrase.vAlign = OF_TEXT_ALIGN_BOTTOM;
            } else if ( alignment == "MIDDLE"){
                newPhrase.vAlign = OF_TEXT_ALIGN_MIDDLE;
            } 
            
            script.push_back(newPhrase);
            
            froebelShapeButton newButton;
            newButton.setShape(0, buttonSize);
            newButton.color.addState(2);
            newButton.color.addState(0);
            newButton.color.addState(7);
            newButton.textColor.set(0.0,0.0);
            buttons.push_back(newButton);
            
            XML.popTag();
        }
        
        XML.popTag();
        
        
        float totalWidth = buttons.size()*2.0*buttonSize;
        for(int i = 0; i < buttons.size(); i++){
            
            buttons[i].x = x + width*0.5 - totalWidth*0.5 + buttonSize + buttonSize*i*2.0;
            buttons[i].y = y + height + buttonSize*0.5;
        }
        
        bFinish = false;
        setLine(0);
        
    } else {
        ofLog(OF_LOG_ERROR, "File " + ofToDataPath(_xmlFile) + " could not be opened" );
    }

    return success;
}

void TextSequencer::setNextPhrase(textPhrase &_phrase ){

    delete  text;
    text = new TextBlock();
    
    if (text != NULL){
        text->set(x,y,width,height);
        text->linkFont(font);
        text->setText( _phrase.text );
        text->setAlignment(_phrase.hAlign, _phrase.vAlign);
    }
}

void TextSequencer::setPrevLine(){
    if ( currentLine > 0){
        setLine(currentLine-1);
    }
}

void TextSequencer::setNextLine(){
    setLine(currentLine+1);
}

void TextSequencer::setLine( unsigned int _nLine){
    if ( _nLine < size()){
        textColor.setState(0);
        desiredLine = _nLine;
        countDown = ofGetFrameRate();
        
        for(int i = 0; i < buttons.size(); i++){
            if (i == desiredLine){
                buttons[i].bEnable = false;
            } else {
                buttons[i].bEnable = true;
            }
        }
    } else {
        bFinish = true;
    }
}

void TextSequencer::update(){
    textColor.update();
    
    if (countDown == 0){
        setNextPhrase( script[desiredLine] );
        currentLine = desiredLine;
        countDown = -1;
        textColor.setState(1);
    } else if ( countDown > 0){
        countDown--;
    }
    
    for(int i = 0; i < buttons.size(); i++){
        buttons[i].update();
    }
}

void TextSequencer::draw(){
    ofPushStyle();
    if ( script.size() > 0 ){
        if (text != NULL){
            ofSetColor(textColor);
            text->draw();
        }
    }
    
    ofFill();
    ofSetColor(255);
    for(int i = 0; i < buttons.size(); i++){
        buttons[i].draw();
    }
    
    ofPushStyle();
}