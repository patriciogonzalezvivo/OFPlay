//
//  SlideSequencer.h
//
//  Created by Patricio Gonzalez Vivo on 4/1/12.
//  Copyright (c) 2012 http://PatricioGonzalezVivo.com All rights reserved.
//

#ifndef SLIDESEQUENCER
#define SLIDESEQUENCER

#include "ofMain.h"

#include "TextBlock.h"

#include "froebelColor.h"
#include "froebelShapeButton.h"

#include "ofxXmlSettings.h"

typedef struct {
    string                  image;
    vector<string>          text;
    horizontalAlignment     hAlign;
    verticalAlignment       vAlign;
} Slide;

class SlideSequencer: public ofRectangle {
public:
    SlideSequencer();
    
    bool    loadSequence(string _xmlFile);
    
    int     size(){ return slides.size(); }
    void    setLine( unsigned int _nLine);
    void    setNextPhrase(Slide &_slide );
    
    void    setPrevLine();
    void    setNextLine();
    
    int     getLineNumber() { return currentLine; }
    
    void    update();
    void    draw();
    
    vector<froebelShapeButton>  buttons;
    
    bool            bFinish;

protected:
    vector<Slide>   slides;
    
    ofImage             image;
    string              imagePath;
    
    TextBlock           *text;
    froebelColor    textColor;
    
    verticalAlignment   defaultVertAlign;
    horizontalAlignment defaultHoriAlign;

    int             desiredLine;
    int             currentLine;
    int             countDown;
};

#endif
