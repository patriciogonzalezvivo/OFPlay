//
//  froebelShape.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//  Copyright (c) 2012 http://www.patriciogonzalezvivo.com All rights reserved.
//

#ifndef FROEBELSHAPE
#define FROEBELSHAPE

#include "ofMain.h"

class froebelShape : public ofPoint {
public:
    
    froebelShape();
    
    void    setShape(int _shapeNum, float _size);
    void    setColor(int _color, float _damping = 0.1);
    
    void    draw();
    
    ofPolyline      shape;
    ofFloatColor    dstColor;
    
private:
    ofFloatColor    froebelColor(int _color);
    ofFloatColor    color;
    
    float           damp;
    float           size;
    int             type;
};

#endif
