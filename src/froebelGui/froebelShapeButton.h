//
//  froebelShapeButton.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/7/12.
//  http://www.patriciogonzalezvivo.com
//

#ifndef FROEBELSHAPEBUTTON
#define FROEBELSHAPEBUTTON

#include "ofMain.h"
#include "froebelShape.h"

class froebelShapeButton : public froebelShape {
public:
    
    froebelShapeButton();

    virtual bool    checkMousePressed(ofPoint _mouse);
    
    virtual void    update();
    virtual void    draw();
    
    ofEvent<string> clicked;
    
    string          text;
    froebelColor    textColor;
    ofTrueTypeFont  *font;
    
    enum    { STATE_DISABLE, STATE_ENABLE, STATE_HOVER };
    int             nState;
    bool            bEnable;
};

#endif
