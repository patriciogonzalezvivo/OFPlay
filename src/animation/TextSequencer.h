//
//  TextSequencer.h
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#ifndef TEXTSEQUENCER
#define TEXTSEQUENCER

#include "ofMain.h"

#include "TextBlock.h"

#include "froebelColor.h"
#include "froebelShapeButton.h"

#include "ofxXmlSettings.h"

typedef struct {
    string                  text;
    horizontalAlignment     hAlign;
    verticalAlignment       vAlign;
} textPhrase;

class TextSequencer: public ofRectangle {
public:
    TextSequencer();
    
    bool    loadSequence(string _xmlFile);
    
    int     size(){ return script.size(); }
    void    setLine( unsigned int _nLine);
    void    setNextPhrase(textPhrase &_phrase );
    
    void    setPrevLine();
    void    setNextLine();
    
    int     getLineNumber() { return currentLine; }
    string  getLineText(){return (text !=NULL)? text->getText(): ""; };
    
    void    update();
    void    draw();
    
    vector<froebelShapeButton>  buttons;
    
    froebelColor    textColor;
    froebelColor    elemColor;
    
    ofTrueTypeFont  *font;
    bool            bFinish;

protected:
    vector<textPhrase>  script;

    string              rawText;
    
    TextBlock           *text;
    
    verticalAlignment   defaultVertAlign;
    horizontalAlignment defaultHoriAlign;
    
    int                 desiredLine;
    int                 currentLine;
    int                 countDown;
};

#endif
