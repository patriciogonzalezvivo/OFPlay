//
//  froebelColor.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/3/12.
//  Copyright (c) 2012 http://www.patriciogonzalezvivo.com All rights reserved.
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
//    void    addStateAsPointer( ofFloatColor *_color );
    
    void    setState( unsigned int _stateN );
    
    void    update();
    
    float   damp;
private:
    vector<ofFloatColor>   states;
    
    int     dstStateN;
};

#endif
