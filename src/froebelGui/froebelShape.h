//
//  froebelShape.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//  http://www.patriciogonzalezvivo.com
//

#ifndef FROEBELSHAPE
#define FROEBELSHAPE

#include "ofMain.h"

#include "froebelColor.h"

class froebelShape : public ofPoint {
public:
    
    froebelShape();
    
    void    setShape(int _shapeNum, float _size);
    
    virtual void    draw();
    
    froebelColor    color;
    
    float           size;
    
    ofPolyline      shape;
protected:
    int             type;
};

#endif
