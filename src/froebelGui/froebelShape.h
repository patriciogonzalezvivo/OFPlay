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

#include "froebelColor.h"

class froebelShape : public ofPoint {
public:
    
    froebelShape();
    
    void    setShape(int _shapeNum, float _size);
    
    void    draw();
    
    ofPolyline      shape;
    
    froebelColor    color;
    
private:
    float           size;
    int             type;
};

#endif
