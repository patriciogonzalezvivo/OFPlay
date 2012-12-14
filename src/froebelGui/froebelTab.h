//
//  froebelTab.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/12/12.
//
//

#ifndef FROEBELTAB
#define FROEBELTAB

#include "ofMain.h"

#include "froebelColor.h"
#include "froebelShapeButton.h"

class froebelTab : public ofRectangle{
public:
    
    froebelTab();
    
    void    addElement(string _title);
    bool    checkMousePressed(ofPoint _mouse);
    
    int     size(){ return buttons.size(); }
    void    setElement( unsigned int _nLine);
    void    setPrev();
    void    setNext();
    
    int     getElementNumber() { return currentElement; }
    string  getElementText() { return text; };
    
    void    update();
    void    draw();

    ofTrueTypeFont  font;
    froebelColor    textColor;
    
private:
    vector<froebelShapeButton>  buttons;
    
    string          text;
    float           buttonSize;
    int             desiredElement;
    int             currentElement;
    int             countDown;
    
};

#endif
