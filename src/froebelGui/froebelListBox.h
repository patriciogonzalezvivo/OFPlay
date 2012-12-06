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
#include "froebelContainer.h"
#include "froebelFolderElement.h"

class froebelListBox : public froebelTextBox {
public:
    
    froebelListBox();
    
    void    addElement(string _value, bool _defVal = false, int _iconShape = -1, int _edgeCoorner = -1);
    bool    select(string _value);
    string  getSelectedAsString();
    virtual ofRectangle getBoundingBox();
    bool    checkMousePressed(ofPoint _mouse);
    
    void    update();
    void    draw();
    
    ofEvent<string> focusLost;
    
    froebelContainer containerBox;
    
protected:
    
};

#endif
