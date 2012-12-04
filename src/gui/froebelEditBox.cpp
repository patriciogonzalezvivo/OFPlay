//
//  froebelEditBox.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//  Copyright (c) 2012 http://www.patriciogonzalezvivo.com All rights reserved.
//

#include "froebelEditBox.h"

froebelEditBox::froebelEditBox(){
    subInfo     = NULL;
    
    setActiveColors(3, 4);
    setPasiveColors(2, 5);
    
    bgColor = bgDstColor = fgColor = fgDstColor = froebelColor(0);
    
    bSelected   = false;
    bLeftAlign  = true;
    bChange     = true;
    
    text = "";
    prefix = "";
    deliminater = "";
    
    bIcon = false;
    bEdge = false;
    
    maxWidth = 600;
    size = 40;
    damp = 0.1;
    
	cursorPosition = 0;
	cursorx = 0;
	cursory = 0;
    bEnabled = false;
	bEditing = false;
    bDrawCursor = false;
	mouseDownInRect = false;
}

void froebelEditBox::enable(){
	if(!bEnabled){
		ofAddListener(ofEvents().mousePressed, this, &froebelEditBox::mousePressed);
		ofAddListener(ofEvents().mouseReleased, this, &froebelEditBox::mouseReleased);
		bEnabled = true;
	}
}

void froebelEditBox::disable(){
	if(bEditing){
		endEditing();
	}
	if(bEnabled){
        ofRemoveListener(ofEvents().mousePressed, this, &froebelEditBox::mousePressed);
		ofRemoveListener(ofEvents().mouseReleased, this, &froebelEditBox::mouseReleased);
		bEnabled = false;
    }
	
}

void froebelEditBox::beginEditing() {
    if(!bEditing){
        ofAddListener(ofEvents().keyPressed, this, &froebelEditBox::keyPressed);
        ofSendMessage(TEXTFIELD_IS_ACTIVE);
        bEditing = true;
        bDrawCursor = true;
        cursory = 0;
        cursorPosition = cursorx = text.size();
        bSelected = true;
    }
}

void froebelEditBox::endEditing() {
    if(bEditing){
        ofRemoveListener(ofEvents().keyPressed, this, &froebelEditBox::keyPressed);
        ofSendMessage(TEXTFIELD_IS_INACTIVE);
        ofNotifyEvent(textChanged, text, this);
        bEditing = false;
        bDrawCursor = false;
        bSelected = false;
        
        for (int i = 0; i < text.size(); i++){
            int which = (int)text[i];
            if ((which >= 48 && which <= 57) ||
                (which >= 65 && which <= 90) ||
                (which >= 97 && which <= 122)){
            } else {
                text[i] = '_';
            }
        }
        
        //  Calculate the size of the text
        //
        ofRectangle textBox = font->getStringBoundingBox( displayText , 0, 0);
        
        if ( textBox.width + size >= width ){
            
            if ( textBox.width + size <= maxWidth ){
                
                //  If it less that the max adjust the shape
                //
                width = size*0.5 + textBox.width + size;
                
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
            width = size*0.5 + textBox.width + size;
            
            if ( textBox.height+size*0.5 > size ) {
                int nEdges = 0;
                while ( textBox.height+size > size*nEdges ) {
                    nEdges++;
                    height = size*nEdges;
                }
            }
        } else {
            width = textBox.width + size;
            height = size;
        }
    }
}

void froebelEditBox::draw(){
    froebelTextBox::draw();
    
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(x+size*0.5, y+size-13);
    
    if(bDrawCursor) {
        
        ofPushStyle();
        float timeFrac = 0.5 * sin(3.0f * ofGetElapsedTimef()) + 0.5;
        
        ofColor col = fgColor; //ofGetStyle().color;
		
        ofSetColor(col.r * timeFrac, col.g * timeFrac, col.b * timeFrac);
        ofSetLineWidth(3.0f);
        
        float cursorPos = font->stringWidth(prefix + text.substr(0,cursorx) )+3;
        if ( bLeftAlign ){
            ofLine(cursorPos, 13.7*cursory+2,cursorPos, 13.7*cursory-font->stringHeight("L")-2);
        } else {
            cursorPos = width - size + 3;
            
            if (bEdge)
                cursorPos -= size*0.5;
            
            if (bIcon)
                cursorPos -= size*0.5;
            
            ofLine(cursorPos, 13.7*cursory+2,cursorPos, 13.7*cursory-font->stringHeight("L")-2);
        }

        ofPopStyle();
    }
    
    ofPopMatrix();
    ofPopStyle();
    
}

void froebelEditBox::mousePressed(ofMouseEventArgs& args){
	mouseDownInRect = inside(args.x, args.y);
}

void froebelEditBox::mouseReleased(ofMouseEventArgs& args){
    
    if( inside(args.x, args.y)) {
        if(!bEditing && mouseDownInRect){
	        beginEditing();
        }
    }
    else if(bEditing){
		endEditing();
	}
}

void froebelEditBox::keyPressed(ofKeyEventArgs& args) {
    
	int key = args.key;
	if (key == OF_KEY_RETURN) {
        endEditing();
        return;
	}
	
	if (key >=32 && key <=126) {
		text.insert(text.begin()+cursorPosition, key);
		cursorPosition++;
	}
	
	
	if (key==OF_KEY_BACKSPACE) {
		if (cursorPosition>0) {
			text.erase(text.begin()+cursorPosition-1);
			--cursorPosition;
		}
	}
	
	if (key==OF_KEY_DEL) {
		if (text.size() > cursorPosition) {
			text.erase(text.begin()+cursorPosition);
		}
	}
	
	if (key==OF_KEY_LEFT){
		if (cursorPosition>0){
			--cursorPosition;
		}
	}
	
	if (key==OF_KEY_RIGHT){
		if (cursorPosition<text.size()){
			++cursorPosition;
		}
	}
	
	//for multiline:
	cursorx = cursory = 0;
	if(text.size() > 0){
		for (int i=0; i<cursorPosition; ++i) {
			if (*(text.begin()+i) == '\n') {
				++cursory;
				cursorx = 0;
			} else {
				cursorx++;
			}
		}
	}
    
    bChange = true;
}