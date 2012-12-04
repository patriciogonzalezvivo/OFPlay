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
    
    set(1.0f);
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
            color = ofColor(186, 1, 23);
            break;
        case 4:
            color = ofColor(247, 181, 55);
            break;
        case 5:
            color = ofColor(64, 79, 122);
            break;
        case 6:
            color = ofColor(62, 2, 35);
            break;
        case 7:
            color = ofColor(193, 66, 11);
            break;
        default:
            color = ofColor(255);
            break;
    }
    
    return color;
}

void froebelColor::clear(){
//    for (int i = 0; i < states.size(); i++) {
//        delete states[i];
//    }
//    
    states.clear();
}

void froebelColor::addState( unsigned int _pallet ){
    ofFloatColor newColor;
    newColor = getFromPalet( _pallet );
    states.push_back(newColor);
}

//void froebelColor::addStateAsPointer( ofFloatColor *_color ){
//    states.push_back(_color);
//}

void froebelColor::setState( unsigned int _stateN ){
    if ( (_stateN < states.size()) && ( _stateN != dstStateN ) ){
        dstStateN = _stateN;
    }
}

void froebelColor::update(){
    
    if ( dstStateN != -1 ){
        if ( (*this) != states[ dstStateN ] ){
            lerp( states[ dstStateN ], damp);
        }
        return;
    }
    
}