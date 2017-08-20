#include "SoundEngine.h"
#include "ofUtils.h"

//--------------------------------------------------------------
SoundEngine::SoundEngine()
{

    ControlParameter dry = addParameter("dry",-3.f).displayName("Dry Level (dbFS)").min(-60.f).max(0.f);
    ControlParameter wet = addParameter("wet",-3.f).displayName("Wet Level (dbFS)").min( -60.f).max(0.f);
    ControlParameter time = addParameter("decayTime", 2.0f).displayName("Decay Time (s)").min(0.1f).max(10.f);
    ControlParameter lowDecay = addParameter("lowDecay", 16000.0f).displayName("Decay Lowpass Cutoff (Hz)").min(4000.0f).max(20000.0f);
    ControlParameter hiDecay = addParameter("hiDecay",20.0f).displayName("Decay Highpass Cutoff (Hz)").min(20.f).max(250.f);
    ControlParameter preDelay = addParameter("preDelay", 0.001f).displayName("Pre-delay").min(0.001f).max(0.05f);
    ControlParameter inputLPF = addParameter("inputLPF",18000.f).displayName("Input LPF cutoff (Hz)").min(4000.0f).max(20000.0f);
    ControlParameter inputHPF = addParameter("inputHPF",20.f).displayName("Input HPF cutoff (Hz)").min(20.f).max(250.f);
    ControlParameter density = addParameter("density",0.5f).displayName("Density");
    ControlParameter shape = addParameter("shape",0.5f).displayName("Shape");
    ControlParameter size = addParameter("size",0.5f).displayName("Room Size");
    ControlParameter stereo = addParameter("stereo",0.5f).displayName("Stereo Width");

    reverb = Reverb()
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
}

//--------------------------------------------------------------
SoundEngine::~SoundEngine()
{
    while(!synths.empty()) {
        delete synths.back(), synths.pop_back();
    }
    synths.clear();
}

//--------------------------------------------------------------
int SoundEngine::createSynth(string filename)
{
    Synth *s = new Synth();
    (*s) = Tonic::SynthFactory::createInstance(filename);

    synths.push_back(s);

    string n = ofToString(getNumSynths());
    ControlParameter volume = addParameter("fx1_send"+n, 0.5f).displayName("FX1 send");
    FX1Send.push_back(volume);

    recalculateSynths();

    return (getNumSynths() -1);
}

//--------------------------------------------------------------
void SoundEngine::recalculateSynths()
{
    Generator dry,wet,mix;

    for(int i=0;i < synths.size();i++)
    {
        Synth s = *(synths[i]);
        Generator gen = s.getOutputGen();
        dry = dry + gen;
        wet = wet + (gen*FX1Send[i]);
    }

    mix = dry + (wet >> reverb);

    setOutputGen(mix);
}

//--------------------------------------------------------------
void SoundEngine::setup()
{

}
