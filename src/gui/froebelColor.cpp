//
//  froebelColor.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/3/12.
//  Copyright (c) 2012 http://www.patriciogonzalezvivo.com All rights reserved.
//

#include "froebelColor.h"

froebelColor::froebelColor(){
    
}

ofFloatColor froebelColor::getColorFromPalet( int _palletNum ){
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
            color = ofColor(0);
            break;
    }
    
    return color;
}

void    addState( int _pallet );
void    addState( ofFloatColor *_color );