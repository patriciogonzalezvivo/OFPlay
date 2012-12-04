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
    vector<string>  getSelected();
    string  getSelectedAsString();
    
    bool checkMousePressed(ofPoint _mouse);
    
    void    clear();
    void    reset();
    
    void    update();
    void    draw();
    
    ofFloatColor    backgroundColor;
    
    vector<froebelTextBox*> elements;
    
    int     maxHeight;
    
protected:
    ofRectangle     box;
    
    float           mouseOffSet;
    
};

#endif
