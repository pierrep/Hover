//
//  SampleWithReverb.cpp
//  TonicDemo
//
//  Created by Nick Donaldson on 5/5/13.
//

#include "Tonic.h"
#include "ofApp.h"

using namespace Tonic;

class SampleWithReverb : public Synth {

public:

  SampleWithReverb(){
    ControlParameter volume = addParameter("volume", 0.5f).displayName("Volume");
    ControlParameter dry = addParameter("dry",-6.f).displayName("Dry Level (dbFS)").min(-60.f).max(0.f);
    ControlParameter wet = addParameter("wet",-20.f).displayName("Wet Level (dbFS)").min( -60.f).max(0.f);
    ControlParameter time = addParameter("decayTime", 1.0f).displayName("Decay Time (s)").min(0.1f).max(10.f);
    ControlParameter lowDecay = addParameter("lowDecay", 16000.0f).displayName("Decay Lowpass Cutoff (Hz)").min(4000.0f).max(20000.0f);
    ControlParameter hiDecay = addParameter("hiDecay",20.0f).displayName("Decay Highpass Cutoff (Hz)").min(20.f).max(250.f);
    ControlParameter preDelay = addParameter("preDelay", 0.001f).displayName("Pre-delay").min(0.001f).max(0.05f);
    ControlParameter inputLPF = addParameter("inputLPF",18000.f).displayName("Input LPF cutoff (Hz)").min(4000.0f).max(20000.0f);
    ControlParameter inputHPF = addParameter("inputHPF",20.f).displayName("Input HPF cutoff (Hz)").min(20.f).max(250.f);
    ControlParameter density = addParameter("density",0.5f).displayName("Density");
    ControlParameter shape = addParameter("shape",0.5f).displayName("Shape");
    ControlParameter size = addParameter("size",0.5f).displayName("Room Size");
    ControlParameter stereo = addParameter("stereo",0.5f).displayName("Stereo Width");

    //float bpm = 30.f;

    //ControlMetro beat = ControlMetro().bpm(bpm);
    //ControlDelay offbeat = ControlDelay(1.2f).input(beat).delayTime(30.0f/bpm);


	string samplename = ofApp::sampleToLoad;
    cout << "samplename = " << samplename << endl;

	SampleTable buf = loadAudioFile(samplename);
	BufferPlayer player;
    //player.setBuffer(buf).loop(true).trigger(1).playReversed(true);
    player.setBuffer(buf).loop(true).trigger(1);

    Reverb reverb = Reverb()
      .preDelayTime(preDelay)
      .inputLPFCutoff(inputLPF)
      .inputHPFCutoff(inputHPF)
      .decayTime(time)
      .decayLPFCutoff(lowDecay)
      .decayHPFCutoff(hiDecay)
      .stereoWidth(stereo)
      .density(density)
      .roomShape(shape)
      .roomSize(size)
      .dryLevel(ControlDbToLinear().input(dry))
      .wetLevel(ControlDbToLinear().input(wet));

    Generator mix = ((player) >> reverb) * 0.8f  ;
    Generator output = mix * volume.smoothed();
    setOutputGen(output);

  }

};

TONIC_REGISTER_SYNTH(SampleWithReverb);
