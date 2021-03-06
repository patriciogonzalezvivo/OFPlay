//
//  froebelDirElement.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/5/12.
//  http://www.patriciogonzalezvivo.com
//

#ifndef FROEBELFOLDERELEMENT
#define FROEBELFOLDERELEMENT

#include "ofMain.h"
#include "froebelTextBox.h"
#include "froebelContainer.h"

class froebelFolderElement : public froebelTextBox {
public:
    
    froebelFolderElement();
    
    void                reset();
    virtual string      getText();
    virtual ofRectangle getBoundingBox();
    
    virtual bool    checkMousePressed(ofPoint _mouse);
    
    virtual void    update();
    virtual void    draw();
    
    froebelContainer *father;
    froebelContainer containerBox;
    
    string  rootPath;
    
protected:
    
};
#endif
