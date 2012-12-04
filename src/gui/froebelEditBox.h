//
//  froebelEditBox.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//  Copyright (c) 2012 http://www.patriciogonzalezvivo.com All rights reserved.
//

#ifndef FROEBELEDITBOX
#define FROEBELEDITBOX

#define TEXTFIELD_IS_ACTIVE "textfieldIsActive"
#define TEXTFIELD_IS_INACTIVE "textfieldIsInactive"

#include "ofMain.h"
#include "froebelTextBox.h"

class froebelEditBox : public froebelTextBox {
public:
    froebelEditBox();
    
    void    enable();
	void    disable();
    
	void    beginEditing();
	void    endEditing();
    
    void    draw();
    
protected:
    ofEvent<string> textChanged;
    
	void    keyPressed(ofKeyEventArgs &a);
    void    mousePressed(ofMouseEventArgs& args);
    void    mouseReleased(ofMouseEventArgs& args);
    
    int     cursorPosition;
    int		cursorx, cursory;
    
    bool 	bEnabled;
	bool	bEditing;
    bool    bDrawCursor;
    
    bool	mouseDownInRect;
};

#endif
