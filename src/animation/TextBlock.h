/***********************************************************************

 Copyright (c) 2009, Luke Malcolm, www.lukemalcolm.com

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 ***********************************************************************/

#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include "ofMain.h"
#include <iterator>

enum horizontalAlignment {
    OF_TEXT_ALIGN_LEFT,
    OF_TEXT_ALIGN_RIGHT,
    OF_TEXT_ALIGN_JUSTIFIED,
    OF_TEXT_ALIGN_CENTER
};

enum verticalAlignment {
    OF_TEXT_ALIGN_TOP,
    OF_TEXT_ALIGN_BOTTOM,
    OF_TEXT_ALIGN_MIDDLE
};

typedef struct{
    string character;
    string code;
} charSubstitution;

typedef struct {
    string      rawWord;
    float       width;
    float       height;
} wordBlock;

typedef struct {
    vector<int> wordsID;
    float       width;
    float       height;
} lineBlock;

class TextBlock : public ofRectangle{
public:
    TextBlock();

    void    linkFont(ofTrueTypeFont *_font);
    
    void    setAlignment(horizontalAlignment _hAlignment , verticalAlignment _vAlignment = OF_TEXT_ALIGN_TOP);
    
    float   getTextWidth();
    float   getTextHeight();
    
    void    setText(string _inputText);
    string  getText()const{return rawText;};
    
    void    draw();
    void    draw(float _x, float _y, float _w = -1, float _h = -1);
    
protected:
    void    _loadWords();

    void    _trimLineSpaces();
    float   _getWidthOfWords();
    int     _getLinedWords();
    
    int     _wrapTextX(float lineWidth);                 //Returns the number of lines it formed.
    bool    _wrapTextForceLines(int linesN);
    
    void    _forceScale(float _scale);
    
    vector<wordBlock>   words;
    vector<lineBlock>   lines;
    wordBlock           blankSpaceWord;
    ofTrueTypeFont      *font;
    
    horizontalAlignment hAlignment;
    verticalAlignment   vAlignment;
    
    string              rawText;
};

#endif
