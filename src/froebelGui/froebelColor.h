//
//  froebelColor.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/3/12.
//  http://www.patriciogonzalezvivo.com
//

#ifndef FROEBELCOLOR
#define FROEBELCOLOR

#include "ofMain.h"

class froebelColor : public ofFloatColor {
public:
    
    froebelColor();
    
    void    setFromPalet( unsigned int _palletN  );
    ofFloatColor    getFromPalet( unsigned int _palletN );
    
    void    clear();
    void    addState( unsigned int _palletN );
    void    addState( ofFloatColor _color);
    void    linktToColor( ofFloatColor *_color );
    
    void    setState( unsigned int _stateN );
    
    void    update();
    
    float   damp;
private:
    vector<ofFloatColor>   states;
    ofFloatColor    *pointerToColor;
    
    int     dstStateN;
};

#endif
