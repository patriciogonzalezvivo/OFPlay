//
//  froebelListBox.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//  Copyright (c) 2012 http://www.patriciogonzalezvivo.com All rights reserved.
//

#ifndef FROEBELLISTBOX
#define FROEBELLISTBOX

#include "ofMain.h"
#include "froebelTextBox.h"

class froebelListBox : public froebelTextBox {
public:
    
    froebelListBox();
    
    void    addElement(string _value, bool _defVal = false, int _iconShape = -1);
    bool    select(string _value);
    
    ofRectangle getBoundingBox();
    vector<string>  getSelected();
    string  getSelectedAsString();
    
    bool    checkMousePressed(ofPoint _mouse);
    
    void    clear();
    void    reset();
    
    void    update();
    void    draw();
    
    froebelColor    backgroundColor;
    
    vector<froebelTextBox*> elements;
    
    float   offsetY;
    float   offsetPct;
    float   damp;
    int     maxHeight;
    
protected:
    ofRectangle box;
    ofRectangle slider;
    
    float       totalBoxHeight;
    float       totalLenght;
};

#endif
