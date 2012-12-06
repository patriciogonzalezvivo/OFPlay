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
    ~froebelContainer();
    
    void            addElement( froebelTextBox *_newElement);
    bool            select(string _value);
    vector<string>  getSelected();
    void            clear();
    void            reset();
    
    virtual bool    checkMousePressed(ofPoint _mouse);
    
    vector<froebelTextBox*> elements;
    
    void        update();
    void        draw();
    
    froebelColor bgColor;
    ofRectangle  slider;
    
    int         size;
    int         maxHeight;
    bool        bEnable;
    bool        bCheckList;
    
private:
    float       offsetY;
    float       damp;
    float       totalBoxHeight;
    float       totalLenght;
};

#endif
