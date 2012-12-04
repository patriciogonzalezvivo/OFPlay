//
//  froebelTextBox.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//  Copyright (c) 2012 http://www.patriciogonzalezvivo.com All rights reserved.
//

#ifndef FROEBELTEXTBOX
#define FROEBELTEXTBOX

#include "ofMain.h"

#include "froebelColor.h"
#include "froebelShape.h"

class froebelTextBox: public ofRectangle {
public:
    froebelTextBox();
    
    void    setSizeAndShapes(float _size, int _endingShape = -1, int _iconShape = -1);
    
    virtual bool checkMousePressed(ofPoint _mouse);
    
    virtual void setPrefix( string _prefix );
    virtual void setDivider( string _deliminater );
    virtual void setText(string _text );
    virtual string getText();
    
    virtual ofRectangle getBoundingBox();
    virtual void update();
    virtual void draw();
    
    froebelColor    fgColor;
    froebelColor    bgColor;
    
    ofTrueTypeFont  *font;
    froebelTextBox  *subInfo;
    
    enum    { STATE_PASIVE, STATE_HOVER, STATE_ACTIVE };
    int     nState;
    int     maxWidth;
    
    bool    bChange;
    bool    bSelected;
    bool    bLeftAlign;
    bool    bFixedSize;
    
protected:
    string  text;
    string  prefix;
    string  deliminater;
    
    froebelShape    endingShape;
    froebelShape    iconShape;
    
    string          displayText;
    
    ofRectangle     textBox;
    
    float   size;
    float   margins;

    bool    bEdge;
    bool    bIcon;
    int     nEdges;
};

#endif
