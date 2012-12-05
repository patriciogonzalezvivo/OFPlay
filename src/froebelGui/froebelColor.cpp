//
//  froebelColor.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/3/12.
//  Copyright (c) 2012 http://www.patriciogonzalezvivo.com All rights reserved.
//

#include "froebelColor.h"

froebelColor::froebelColor(){
    dstStateN   = -1;
    damp        = 0.1;
    pointerToColor = NULL;
    r = 1.0;
    g = 1.0;
    b = 1.0;
    a = 1.0;
}

void froebelColor::setFromPalet(unsigned int _palletN){
    set(getFromPalet(_palletN));
}

ofFloatColor froebelColor::getFromPalet( unsigned int _palletNum ){
    ofFloatColor color;
    
    ofColor palette[8];
    
//    palette[0] = ofColor(220);
//    palette[1] = ofColor(4, 49, 7);
//    palette[2] = ofColor(220, 202, 185);
//    palette[3] = ofColor(201,28,29);
//    palette[4] = ofColor(245,170,53);
//    palette[5] = ofColor(41,94,151);
//    palette[6] = ofColor(35,151,66);
//    palette[7] = ofColor(233,88,43);
    
    palette[0] = ofColor(220);
    palette[1] = ofColor(4, 49, 7);
    palette[2] = ofColor(220, 202, 185);
    palette[3] = ofColor(186, 1, 23);
    palette[4] = ofColor(247, 181, 55);
    palette[5] = ofColor(64, 79, 122);
    palette[6] = ofColor(62, 2, 35);
    palette[7] = ofColor(193, 66, 11);
    
    if (_palletNum < 8){
        color = palette[_palletNum];
    } else {
        color = ofColor(255);
    }
    
    return color;
}

void froebelColor::clear(){
    states.clear();
    
    pointerToColor = NULL;
}

void froebelColor::addState( ofFloatColor _color ){
    states.push_back(_color);
}

void froebelColor::addState( unsigned int _pallet ){
    ofFloatColor newColor;
    newColor.set(getFromPalet( _pallet ));
    states.push_back(newColor);
}

void froebelColor::setState( unsigned int _stateN ){
    if ( (_stateN < states.size()) && ( _stateN != dstStateN ) ){
        dstStateN = _stateN;
    }
}

void froebelColor::linktToColor( ofFloatColor *_color ){
    pointerToColor = _color;
}

void froebelColor::update(){
    
    if (pointerToColor != NULL){
        lerp( *pointerToColor, damp );
    } else if ( dstStateN != -1 ){
        if ( (*this) != states[ dstStateN ] ){
            lerp( states[ dstStateN ], damp);
        }
        return;
    }
    
}