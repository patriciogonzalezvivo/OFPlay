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

class textFont: public ofTrueTypeFont {
public:
    textFont(){
    }
    
    float getCharacterWidth(char ch){
        if (ch=='\n')
            return 10.0;
        else {
            if (ch==' ') ch='i';
            if (ch=='\xE0') ch='a'; //ˆ
            if (ch=='\xE1') ch='a'; //‡
            if (ch=='\xE2') ch='a'; //‰
            if (ch=='\xE3') ch='a'; //‹
            if (ch=='\xE4') ch='a'; //Š
            if (ch=='\xE6') ch='a'; //¾
            if (ch=='\xE8') ch='e'; //
            if (ch=='\xE9') ch='e'; //Ž
            if (ch=='\xEA') ch='e'; //
            if (ch=='\xEB') ch='e'; //‘
            if (ch=='\xEC') ch='i'; //“
            if (ch=='\xED') ch='i'; //’
            if (ch=='\xEE') ch='i'; //”
            if (ch=='\xEF') ch='i'; //•
            if (ch=='\xF2') ch='o'; //˜
            if (ch=='\xF3') ch='o'; //—
            if (ch=='\xF4') ch='o'; //™
            if (ch=='\xF5') ch='o'; //›
            if (ch=='\xF6') ch='o'; //š
            if (ch=='\xF9') ch='u'; //
            if (ch=='\xFA') ch='u'; //œ
            if (ch=='\xFB') ch='u'; //ž
            if (ch=='\xFC') ch='u'; //Ÿ
            if (ch=='\xC7') ch='c'; //
            return cps[ch-NUM_CHARACTER_TO_START].setWidth;
        }
    }
};

class TextBlock : public ofRectangle{
public:
    TextBlock();

    void    loadFont(string _fontLocation, float _fontSize, int _dpi = 90);
    void    setAlignment(horizontalAlignment _hAlignment , verticalAlignment _vAlignment = OF_TEXT_ALIGN_TOP);
    
    float   getTextWidth();
    float   getTextHeight();
    
    void    setText(string _inputText);
    void    setText(vector<string> _inputText);
    void    appendText(string _inputText);
    string  getText()const{return rawText;};
    
    void    draw();
    void    draw(float _x, float _y, float _w = -1, float _h = -1);
    
protected:
    void    _loadWords();
    void    _subsChars(string & origString){
        
        static charSubstitution chars[]={ {"Ã ","\xE0"}, {"Ã¡","\xE1"}, {"Ã¢","\xE2"}, {"Ã£","\xE3"}, {"Ã¤","\xE4"}, {"Ã¦","\xE6"}, {"Ã²","\xF2"},{"Ã³","\xF3"}, {"Ã´","\xF4"}, {"Ãµ","\xF5"}, {"Ã¶","\xF6"}, {"Ã¹","\xF9"}, {"Ãº","\xFA"}, {"Ã»","\xFB"}, {"Ã¼","\xFC"}, {"Ã¨","\xE8"}, {"Ã©","\xE9"}, {"Ãª","\xEA"}, {"Ã«","\xEB"}, {"Ã¬","\xEC"}, {"Ã­","\xED"}, {"Ã®","\xEE"}, {"Ã¯","\xEF"}, {"Ã§","\xE7"}, {"Ã‡","\xC7"} };
        
        for(int i=0; i<24; i++){
            while(origString.find(chars[i].character) !=string::npos){
                origString = origString.substr(0,origString.find(chars[i].character)) + chars[i].code + origString.substr(origString.find(chars[i].character)+2);
            }
        };
    }

    void    _trimLineSpaces();
    float   _getWidthOfWords();
    int     _getLinedWords();
    
    int     _wrapTextX(float lineWidth);                 //Returns the number of lines it formed.
    bool    _wrapTextForceLines(int linesN);
    
    void    _forceScale(float _scale);
    
    vector<wordBlock>   words;
    vector<lineBlock>   lines;
    wordBlock           blankSpaceWord;
    textFont            font;
    
    horizontalAlignment hAlignment;
    verticalAlignment   vAlignment;
    
    string              rawText;
};

#endif
