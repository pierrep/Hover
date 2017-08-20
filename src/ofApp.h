#pragma once

#include "ofMain.h"
#include "InteractivePad.h"
#include "ofxTonic.h"
#include "ofxMidi.h"
#include "DeclickedFLoat.h"
#include "ofxDSP.h"
#include "SoundEngine.h"

#ifdef USE_LEAP_MOTION
#include "ofxLeapMotion.h"
#endif

using namespace Tonic;


struct ParameterSlider{
  ControlParameter param;
  bool isActive;
  ParameterSlider(ControlParameter paramArg)
    :param(paramArg),
    isActive(false){}
};

//typedef triangleWinFunctor grainPlayerWin;
//typedef gaussianWinFunctor grainPlayerWin;
//typedef hannWinFunctor grainPlayerWin;


class ofApp : public ofBaseApp, public ofxMidiListener {

	public:
	    ~ofApp();
		void setup();
		void update();
		void draw();
        void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void onPadEvent(PadEventArgs &e);
		void audioOut(float* output, int bufferSize, int nChannels);
		void newMidiMessage(ofxMidiMessage& midiMessage);
		void setupAudio();
		void setupMidi();
		void saveSet();

		void loadLayoutXML();

        ofSoundStream soundStream;
        ofxMidiIn midiIn;
        bool bMidiExists;
        float masterPan;
        ofSoundDeclickedFloat masterVolume;
        float outLeft,outRight;

        SoundEngine engine;

        vector<InteractivePad *> pads;
        vector<float> auxsend1;

        /* parameter sliders */
        vector<vector<ParameterSlider> > sliders;
        ofRectangle sliderArea;
        int currentPad;
        int selectedPad;

        ofXml xml;
        int numPads;
        int numSynths;
        int numSamplers;
        int numGrainpitchers;

        float speed;
        float rate;
        float overlaps;
        float grainLength;

        /** leap motion */
        #ifdef USE_LEAP_MOTION
        ofxLeapMotion leap;
        vector <ofxLeapMotionSimpleHand> simpleHands;
        #endif
        vector <int> fingersFound;
        float leapx, leapy;
        bool bLeapInited;

        /** layout **/
        int marginSpacer;

        /** FX **/
        ofxCrush crushFX;
        float crushFXvol;
        float crushFXamount;

        static string sampleToLoad;
};
