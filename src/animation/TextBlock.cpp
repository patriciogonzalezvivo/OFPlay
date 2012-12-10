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

#include "TextBlock.h"

TextBlock::TextBlock(){
    font = NULL;
    
    x = 0;
    y = 0;
    width = ofGetWidth();
    height = ofGetHeight();

    hAlignment  =   OF_TEXT_ALIGN_LEFT;
    vAlignment  =   OF_TEXT_ALIGN_TOP; 
}

void TextBlock::linkFont(ofTrueTypeFont *_font){
    font = _font;
    
    //Set up the blank space word
    //
    blankSpaceWord.rawWord = " ";
    blankSpaceWord.width   = font->stringWidth ("x");
    blankSpaceWord.height  = font->stringHeight("i");
}

void TextBlock::setAlignment(horizontalAlignment _hAlignment , verticalAlignment _vAlignment){
    hAlignment = _hAlignment;
    vAlignment = _vAlignment;
};

void TextBlock::setText(string _inputText){
    
    rawText     = _inputText;
    
    //  Process words extractint width in order to arrange the lines in the specify format
    //
    _loadWords();
    
    _wrapTextX(width);
}

//  If the user specify a position and shape it will change the x, y, width and height of the based ofRectangle
//  variables.
//
void TextBlock::draw(float _x, float _y, float _w, float _h){
    if (_h == -1){
        _h = height;

        if (_w == -1)
            _w = width;
    }
    
    set(_x,_y,_w,_h);
    setText(rawText);
    
    draw();
}

void TextBlock::draw(){
    float yAlig = y;
    
    if (vAlignment == OF_TEXT_ALIGN_BOTTOM){
        yAlig = y + height - getTextHeight();
    } else if (vAlignment == OF_TEXT_ALIGN_MIDDLE){
        yAlig = getCenter().y - getTextHeight()*0.5;
    }
    
    if (hAlignment == OF_TEXT_ALIGN_LEFT){
        string  strToDraw;
        int     currentWordID;
        float   drawX;
        float   drawY;
        
        float currX = 0;
        
        if (words.size() > 0){
            for(int l=0;l < lines.size(); l++){
                for(int w=0;w < lines[l].wordsID.size(); w++){
                    
                    currentWordID = lines[l].wordsID[w];
                    
                    drawX = x + currX;
                    drawY = yAlig + (font->getLineHeight() * (l + 1));
                    
                    ofPushMatrix();
                    font->drawString(words[currentWordID].rawWord.c_str(), drawX, drawY);
                    currX += words[currentWordID].width;
                    ofPopMatrix();
                    
                }
                currX = 0;
            }
        }
    } else if (hAlignment == OF_TEXT_ALIGN_RIGHT){
        string  strToDraw;
        int     currentWordID;
        float   drawX;
        float   drawY;
        
        float currX = 0;
        
        if (words.size() > 0) {
            
            for(int l=0;l < lines.size(); l++){
                for(int w=lines[l].wordsID.size() - 1; w >= 0; w--){
                    
                    currentWordID = lines[l].wordsID[w];
                    
                    drawX = -currX - words[currentWordID].width;
                    drawY = font->getLineHeight() * (l + 1);
                    
                    ofPushMatrix();
                    
                    //Move to top left point using pre-scaled co-ordinates
                    ofTranslate(x + width, yAlig, 0.0f);
                    
                    font->drawString(words[currentWordID].rawWord.c_str(), drawX, drawY);
                    currX += words[currentWordID].width;
                    
                    ofPopMatrix();
                    
                }
                currX = 0;
                
            }
        }
    } else if (hAlignment == OF_TEXT_ALIGN_JUSTIFIED){
        string  strToDraw;
        int     currentWordID;
        float   drawX;
        float   drawY;
        int     spacesN;
        float   nonSpaceWordWidth;
        float   pixelsPerSpace;
        
        float currX = 0;
        
        if (words.size() > 0) {
            for(int l=0;l < lines.size(); l++){
                //Find number of spaces and width of other words;
                spacesN = 0;
                nonSpaceWordWidth = 0;
                
                for(int w = 0; w < lines[l].wordsID.size(); w++){
                    currentWordID = lines[l].wordsID[w];
                    if (words[currentWordID].rawWord == " ") spacesN++;
                    else nonSpaceWordWidth += words[currentWordID].width;
                }
                
                pixelsPerSpace = (width - x - nonSpaceWordWidth) / spacesN;
                
                for(int w=0;w < lines[l].wordsID.size(); w++){
                    currentWordID = lines[l].wordsID[w];
                    
                    drawX = currX;
                    drawY = font->getLineHeight() * (l + 1);
                    
                    ofPushMatrix();
                    //Move to top left point using pre-scaled co-ordinates
                    ofTranslate(x, yAlig, 0.0f);
                    
                    if (words[currentWordID].rawWord != " ") {
                        font->drawString(words[currentWordID].rawWord.c_str(), drawX, drawY);
                        currX += words[currentWordID].width;
                    } else {
                        currX += pixelsPerSpace;
                    }
                    ofPopMatrix();
                    
                }
                currX = 0;
                
            }
        }
    } else if (hAlignment == OF_TEXT_ALIGN_CENTER ){
        string  strToDraw;
        int     currentWordID;
        float   drawX;
        float   drawY;
        float   lineWidth;
        
        float currX = 0;
        
        if (words.size() > 0) {
            for(int l=0;l < lines.size(); l++){
                
                //Get the length of the line.
                lineWidth = 0;
                for(int w=0;w < lines[l].wordsID.size(); w++){
                    currentWordID = lines[l].wordsID[w];
                    lineWidth += words[currentWordID].width;
                }
                
                for(int w=0;w < lines[l].wordsID.size(); w++){
                    currentWordID = lines[l].wordsID[w];
                    
                    drawX = -(lineWidth / 2) + currX;
                    drawY = font->getLineHeight() * (l + 1);
                    
                    ofPushMatrix();
                    //Move to central point using pre-scaled co-ordinates
                    ofTranslate(getCenter().x, yAlig, 0.0f);
                    font->drawString(words[currentWordID].rawWord.c_str(), drawX, drawY);
                    currX += words[currentWordID].width;
                    ofPopMatrix();
                    
                }
                currX = 0;
            }
        }
    }
}

void TextBlock::_trimLineSpaces(){
        if (words.size() > 0) {
            
            //  Now delete all leading or ending spaces on each line
            //
            for(int l=0;l < lines.size(); l++){
                
                // Delete the first word if it is a blank
                //    
                if (lines[l].wordsID.size() > 0){
                    if (words[lines[l].wordsID[0]].rawWord == " ")   lines[l].wordsID.erase(lines[l].wordsID.begin());
                }

                // Delete the last word if it is a blank
                //
                if (lines[l].wordsID.size() > 0){
                    if (words[lines[l].wordsID[lines[l].wordsID.size() - 1]].rawWord == " ") lines[l].wordsID.erase(lines[l].wordsID.end() - 1);
                }
            }
        }

}


void TextBlock::_loadWords(){
    
    istringstream iss(rawText);
    
    vector<string> tokens;
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter<vector<string> >(tokens));
    
    words.clear();
    wordBlock tmpWord;
    for(int i = 0; i < tokens.size(); i++){
        tmpWord.rawWord = tokens.at(i);
        tmpWord.width   = font->stringWidth(tmpWord.rawWord);
        tmpWord.height  = font->stringHeight(tmpWord.rawWord);
        words.push_back(tmpWord);
        
        //  add spaces into the words vector if it is not the last word.
        //
        if (i != tokens.size()) 
            words.push_back(blankSpaceWord);
    }
    
    for(int i=0;i < words.size(); i++){
        ofLog(OF_LOG_VERBOSE, "Loaded word: %i, %s\n", i, words[i].rawWord.c_str());
    }
    
    
}

int TextBlock::_getLinedWords(){

    int wordCount = 0;

    if (words.size() > 0) {
        for(int l=0;l < lines.size(); l++){
            wordCount += lines[l].wordsID.size();
        }
        return wordCount;
    }
    else return 0;
}

bool TextBlock::_wrapTextForceLines(int linesN){
    
    if (words.size() > 0) {
        
        if (linesN > words.size()) linesN = words.size();
        
        float lineWidth = _getWidthOfWords() * (1.1f / (float)linesN);
        
        int curLines = 0;
        bool bGotLines;
        
        //  keep increasing the line width until we get the desired number of lines.
        //
        while (!bGotLines){
            curLines = _wrapTextX(lineWidth);
            if (curLines == linesN) return true;
            if (curLines > linesN) return false;
            lineWidth-=10;
        }
    }
}

int TextBlock::_wrapTextX(float lineWidth){

    if (words.size() > 0) {
        
        lines.clear();
        float       runningWidth = 0.0f;
        bool        newLine = true;
        lineBlock   tmpLine;
        tmpLine.wordsID.clear();
        int         activeLine = 0;

        for(int i = 0; i < words.size(); i++){
            
            //  Add words to each line until it fills the total amount of width
            //  available
            //
            runningWidth += words[i].width;

            if ((runningWidth <= lineWidth)){
                newLine = false;
            } else {
                newLine = true;
                lines.push_back(tmpLine);
                tmpLine.wordsID.clear();
                runningWidth = 0.0f + words[i].width;;
                activeLine++;
            }
            
            //  Store in the line the id of the words
            //
            tmpLine.wordsID.push_back(i);
        }

        //Push in the final line.
        lines.push_back(tmpLine);
        _trimLineSpaces(); //Trim the leading and trailing spaces.

    }

    return lines.size();

}

float TextBlock::_getWidthOfWords(){
    float widthTotal = 0.0f;

    if (words.size() > 0) {
        for(int i=0;i < words.size(); i++) {
            widthTotal += words[i].width;
        }
        return widthTotal;
    } else {
        return 0.0f;
    }

}

float TextBlock::getTextWidth(){
    int   currentWordID;

    float currX = 0.0f;
    float maxWidth  = 0.0f;

    if (words.size() > 0) {
        for(int l=0;l < lines.size(); l++){
            for(int w=0;w < lines[l].wordsID.size(); w++){
                currentWordID = lines[l].wordsID[w];
                currX += words[currentWordID].width;
            }
            maxWidth = MAX(maxWidth, currX);
            currX = 0.0f;
        }
        return maxWidth ;
    }
    else return 0;
}

float TextBlock::getTextHeight(){
    if (words.size() > 0) {
        return font->getLineHeight() * lines.size();
    }
    else return 0;
}