//
//  Sampler.cpp
//

#include "Tonic.h"
#include "ofApp.h"

using namespace Tonic;

class Sampler : public Synth {

public:

  Sampler(){
    ControlParameter volume = addParameter("volume", 0.5f).displayName("Volume");

	string samplename = ofApp::sampleToLoad;

	SampleTable buf = loadAudioFile(samplename);
	BufferPlayer player;
    //player.setBuffer(buf).loop(true).trigger(1).playReversed(false);
    player.setBuffer(buf).loop(true).trigger(1);

    Generator output = player * volume.smoothed();
    setOutputGen(output);
  }

};

TONIC_REGISTER_SYNTH(Sampler);
