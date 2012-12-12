//
//  froebelTextBox.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//  http://www.patriciogonzalezvivo.com
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
    void    setSubInfo(string _comment);
    
    virtual bool checkMousePressed(ofPoint _mouse);
    
    virtual void setPrefix( string _prefix );
    virtual void setDivider( string _deliminater );
    virtual void setText(string _text );
    
    virtual void        reset();
    virtual string      getText();
    virtual float       getVerticalMargins();
    virtual ofRectangle getTextBoundingBox();
    virtual ofRectangle getBoundingBox();
    
    virtual void update();
    virtual void draw();
    
    ofEvent<string> focusLost;
    
    froebelColor    fgColor;
    froebelColor    bgColor;
    
    ofTrueTypeFont  *font;
    froebelTextBox  *subInfo;
    
    enum    { STATE_PASIVE, STATE_HOVER, STATE_ACTIVE };
    int     nState;
    
    int     minWidth;
    int     maxWidth;
    
    bool    bChange;
    bool    bSelected;
    bool    bLeftAlign;
    bool    bFixedSize;
    bool    bEdge;
    bool    bIcon;
    bool    bFill;
    
protected:
    string  text;
    string  prefix;
    string  deliminater;
    
    froebelShape    endingShape;
    froebelShape    iconShape;
    
    string          displayText;
    
    ofRectangle     textBox;
    
    float           size;
    int             nEdges;
};

#endif
