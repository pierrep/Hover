
#pragma once

#include "ofxMSAInteractiveObject.h"
#include "PadEvent.h"

#define		IDLE_COLOR		0xFFFFFF
#define		OVER_COLOR		0xEEEEEE
#define		DOWN_COLOR		0xFF0000


class InteractivePad : public ofxMSAInteractiveObject {
public:

    enum PadType{TOGGLE,BUTTON};
    enum AudioType{SAMPLER,GRAINPITCHER,SYNTH};

    InteractivePad() {};

    InteractivePad(string _name, int _index, int _instrumentId, PadType _padType = TOGGLE, AudioType _audio = SAMPLER, string _filename="", string _samplename="", float _volume=0.5f) {
        name = _name;
        shortname = ofToUpper(name.substr(0,10));
        filename = _filename;
        samplename = _samplename;
        padType = _padType;
        audioType = _audio;
        bIsOn = false;
        bSelected = false;
        index = _index;
        instrumentId = _instrumentId;
        font.load("fonts/Roboto-Bold.ttf",12,true,false);
        volume = _volume;
    }

	void setup() {
		enableMouseEvents();
		enableKeyEvents();
	}

	void update() {
       // if(isMousePressed()) ;
	}

	void draw() {
		ofPushStyle();
            if(bIsOn) ofSetColor(255,0,0);
            else if(isMouseOver()) ofSetHexColor(OVER_COLOR);
            else ofSetHexColor(IDLE_COLOR);

            ofDrawRectRounded(x, y, width, height,10);
            if(bSelected) {
                ofSetColor(100,200,200,100);
                ofDrawRectRounded(x, y, width, height,10);
            }
            ofSetColor(0,0,0);


            font.drawString(shortname, x + width/2.0f - font.stringWidth(shortname)/2.0f , y + height/2.0f + font.stringHeight(shortname)/2.0f);
            //ofDrawBitmapString(name, x + width/2.0f - name.size() , y + height/2.0f - name.size());
		ofPopStyle();
	}

	virtual void onRollOver(int x, int y) {
		//printf("Thumbnail::onRollOver(x: %i, y: %i)\n", x, y);
	}

	virtual void onRollOut() {

	}

	virtual void onMouseMove(int x, int y){

	}

	virtual void onDragOver(int x, int y, int button) {

	}

	virtual void onDragOutside(int x, int y, int button) {

	}

	virtual void onPress(int x, int y, int button) {
        if(padType == TOGGLE) {
            bIsOn = !bIsOn;
        }
        if(padType == BUTTON) {
            bIsOn = true;
        }

        newEvent.index = index;
        newEvent.state = bIsOn;
        ofNotifyEvent(PadEventArgs::padEvent,newEvent, this);
	}

	virtual void onRelease(int x, int y, int button) {
        if(padType == BUTTON) {
            bIsOn = false;
            newEvent.index = index;
            newEvent.state = bIsOn;
            ofNotifyEvent(PadEventArgs::padEvent,newEvent, this);
        }

	}

	virtual void onReleaseOutside(int x, int y, int button) {
        if(padType == BUTTON) {
            bIsOn = false;
            newEvent.index = index;
            newEvent.state = bIsOn;
            ofNotifyEvent(PadEventArgs::padEvent,newEvent, this);
        }
	}

	virtual void keyPressed(int key) {

	}

	virtual void keyReleased(int key) {

	}

	void exit() {

	}


    string      name;
    string      shortname;
    string      filename;
    string      samplename;
    int         index;
    int         instrumentId;
    PadType     padType;
    AudioType   audioType;
    bool        bIsOn;
    bool        bSelected;
    PadEventArgs newEvent;
    ofTrueTypeFont  font;
    float       volume;
};

