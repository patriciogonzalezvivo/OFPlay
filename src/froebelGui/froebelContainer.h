//
//  froebelContainer.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/5/12.
//
//

#ifndef FROEBELCONTAINER
#define FROEBELCONTAINER

#include "ofMain.h"
#include "froebelTextBox.h"

class froebelContainer : public ofRectangle {
public:
    
    froebelContainer();
    
    virtual bool checkMousePressed(ofPoint _mouse);
    
    void update();
    void draw();
    
};

#endif
