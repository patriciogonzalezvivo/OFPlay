//
//  SlideSequencer.cpp
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#include "SlideSequencer.h"

SlideSequencer::SlideSequencer(){
    currentLine = 0;
    desiredLine = 0;
    countDown   = 0;
    
    set(0,0,ofGetWidth(),ofGetHeight());
    
    imagePath = " ";
    
    text    = NULL;
    textColor.addState(ofFloatColor(0.0,0.0));
    textColor.addState(5);
    
    bFinish  = false;
}

bool SlideSequencer::loadSequence(string _xmlFile){
    bool    success = false;
    
    ofxXmlSettings XML;
    if (XML.loadFile(_xmlFile)){
        
        slides.clear();
        buttons.clear();
        
        float buttonSize = 20;
        
        XML.pushTag("sequence");
        
        //  DEFAULT VALUES
        //
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
        
        int totalSlides = XML.getNumTags("slide");
        for(int i = 0; i < totalSlides; i++){
            XML.pushTag("slide",i);
            Slide newSlide;
            
            //  Image
            //
            newSlide.image = XML.getValue("image", "image.png");
            
            //  Text
            //
            int totalTextLines = XML.getNumTags("text");
            for (int i = 0; i < totalTextLines; i++){
                newSlide.text.push_back( XML.getValue("text", "NO TEXT FOUND", i) );
            }
            string alignment = XML.getValue("hAlign", defHorAlign);
            if (alignment == "LEFT"){
                newSlide.hAlign = OF_TEXT_ALIGN_LEFT;
            } else if ( alignment == "RIGHT"){
                newSlide.hAlign = OF_TEXT_ALIGN_RIGHT;
            } else if ( alignment == "JUSTIFIED"){
                newSlide.hAlign = OF_TEXT_ALIGN_JUSTIFIED;
            } else if ( alignment == "CENTER"){
                newSlide.hAlign = OF_TEXT_ALIGN_CENTER;
            }
            alignment = XML.getValue("vAlign", defVerAlign);
            if (alignment == "TOP"){
                newSlide.vAlign = OF_TEXT_ALIGN_TOP;
            } else if ( alignment == "BOTTOM"){
                newSlide.vAlign = OF_TEXT_ALIGN_BOTTOM;
            } else if ( alignment == "MIDDLE"){
                newSlide.vAlign = OF_TEXT_ALIGN_MIDDLE;
            }
            
            slides.push_back(newSlide);
            
            //  Buttons
            //
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
            buttons[i].y = 0;//y + height + buttonSize*0.5;
        }
        
        bFinish = false;
        setLine(0);
        
    } else {
        ofLog(OF_LOG_ERROR, "File " + ofToDataPath(_xmlFile) + " could not be opened" );
    }

    return success;
}

void SlideSequencer::setNextPhrase(Slide &_slide ){
    
    if (imagePath != _slide.image){
        imagePath = _slide.image;
        image.loadImage(imagePath);
    }
    
    if (text != NULL)
        delete  text;
    
    text = new TextBlock();
    
    if (text != NULL){
        float left = x+width*0.5-image.getWidth()*0.5;
        float top = y+height*0.5+image.getHeight()*0.5;
        
        text->set(left*0.5,top,width-left,height-top);
        text->loadFont("Inconsolata.otf", 15);
        text->setText( _slide.text );
        text->setAlignment( _slide.hAlign, _slide.vAlign);
    }
    
    image.setAnchorPercent(0.5, 0.5);
    image.update();
}

void SlideSequencer::setPrevLine(){
    if ( currentLine > 0){
        setLine(currentLine-1);
    }
}

void SlideSequencer::setNextLine(){
    setLine(currentLine+1);
}

void SlideSequencer::setLine( unsigned int _nLine){
    if ( _nLine < size() ){
        
        if (_nLine != desiredLine ){
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
        }
        
    } else {
        bFinish = true;
    }
}

void SlideSequencer::update(){
    
    if ( slides.size() > 0 ){
        textColor.update();
        
        if (countDown == 0){
            setNextPhrase( slides[desiredLine] );
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
}

void SlideSequencer::draw(){
    ofPushStyle();
    if ( slides.size() > 0 ){
        image.draw(x+width*0.5, y+height*0.5);
        
        if (text != NULL){
            ofSetColor(textColor);
            text->draw();
        }
    }
    
    ofPushMatrix();
    ofTranslate(0, y+height);
    ofFill();
    ofSetColor(255);
    for(int i = 0; i < buttons.size(); i++){
        buttons[i].draw();
    }
    
    ofPopMatrix();
    
    ofPushStyle();
}