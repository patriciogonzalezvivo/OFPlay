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
    
    switch ( _palletNum ) {
        case 0:
            color = ofColor(220);
            break;
        case 1:
            color = ofColor(4, 49, 7);
            break;
        case 2:
            color = ofColor(220, 202, 185);
            break;
        case 3:
            color = ofColor(201,28,29);//186, 1, 23);
            break;
        case 4:
            color = ofColor(245,170,53);//247, 181, 55);
            break;
        case 5:
            color = ofColor(41,94,151);//64, 79, 122);
            break;
        case 6:
            color = ofColor(35,151,66);//62, 2, 35);
            break;
        case 7:
            color = ofColor(233,88,43);//193, 66, 11);
            break;
        default:
            color = ofColor(255);
            break;
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