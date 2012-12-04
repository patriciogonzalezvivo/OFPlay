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

class froebelColor {
public:
    
    froebelColor();
    
    ofFloatColor    getColorFromPalet( int _palletNum );
    
    void    addState( int _pallet );
    void    addState( ofFloatColor *_color );
    
    ofFloatColor    color;
private:
    vector<ofFloatColor*>   dstColors;
    
};

#endif
