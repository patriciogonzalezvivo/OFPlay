//
//  froebelContainer.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/5/12.
//
//

#include "froebelContainer.h"

froebelContainer::froebelContainer(){
    
}

void froebelContainer::update(){
    
}

void froebelListBox::updateContainerBoxSize(){
    
    conteinerBox.width = 0;
    totalBoxHeight = 0;
    
    slider.x = conteinerBox.x;
    slider.width = size*0.5;
    
    bool minWidthChange = false;
    
    for(int i = 0; i < elements.size(); i++){
        
        if (totalBoxHeight < maxHeight)
            totalBoxHeight += elements[i]->height;
        
        float elementWidth = elements[i]->getTextBoundingBox().width;
        
        if (elementWidth > conteinerBox.width){
            conteinerBox.width = elementWidth;
            minWidthChange = true;
        }
    }
    
    if (minWidthChange){
        for(int i = 0; i < elements.size(); i++){
            elements[i]->minWidth = conteinerBox.width;
        }
    }
}