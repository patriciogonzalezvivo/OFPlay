//
//  froebelShape.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//  Copyright (c) 2012 http://www.patriciogonzalezvivo.com All rights reserved.
//

#include "froebelShape.h"

froebelShape::froebelShape(){
    color.set(1.0f);
    dstColor.set(1.0f);
    
    damp = 0.1;
    
    size = 40;
    type = 0;
    color = 3;
}

void froebelShape::setShape(int _shapeNum, float _size){
    type = _shapeNum;
    size = _size;
    
    float width = 0;
    float height = 0;
    ofPoint center = ofPoint(0,0);
    shape.clear();
    
    switch (_shapeNum) {
        case 0:
        case 2:
        case 4:
            width = size;
            height = size;
            center.set(width*0.5,height*0.5);
            break;
        case 1:
        case 3:
            width = size*0.5f;
            height = size;
            center.set(0,height*0.5);
            break;
        case 5:
            width = size/1.845f;
            height = size;
            center.set(0,height*0.5);
            break;
        case 6:
        case 7:
            width = size;
            height = size/1.845f;
            center.set(width*0.5,height*0.5);
            break;
        default:
            break;
    }
    
    switch (_shapeNum) {
        case 0:
            shape.arc(center,size*0.4,size*0.4, 0, 360, true, 36);
            break;
        case 1:
            shape.arc(center,size*0.5,size*0.5, -90, 90, true, 20);
            break;
        case 2:
            shape.arc(center,width*0.5,width*0.5, 0, 360, true,3);
            break;
        case 3:
        case 5:
            shape.addVertex(0, 0);
            shape.addVertex(0,height);
            shape.addVertex(width,height*0.5);
            break;
        case 4:
        case 6:
            shape.addVertex(width*0.5,0);
            shape.addVertex(width,height*0.5);
            shape.addVertex(width*0.5,height);
            shape.addVertex(0,height*0.5);
            break;
        case 7:
            shape.addVertex(0,0);
            shape.addVertex(0,height);
            shape.addVertex(width,height);
            break;
    }

    for (int i = 0; i < shape.getVertices().size(); i++) {
        shape.getVertices()[i] -= center;
    }
}

void froebelShape::setColor(int _color, float _damp){
    dstColor = froebelColor(_color);
    damp = _damp;
}

void froebelShape::draw(){
    
    if (dstColor != color){
        color.lerp(dstColor, damp);
    }
    
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(x, y);
    ofSetColor(color);
    
    ofBeginShape();
    for (int i = 0; i < shape.getVertices().size(); i++){
        ofVertex(shape.getVertices()[i]);
    }
    ofEndShape();
    
    ofPopStyle();
    ofPopMatrix();
}

ofFloatColor froebelShape::froebelColor(int _color){
    ofFloatColor color;
    
    switch (_color) {
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
};