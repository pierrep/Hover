//
//  FilteredNoise.h
//  TonicDemo
//
//  Created by Morgan Packard on 2/24/13.

//

/*
Because of the fancy TONIC_REGISTER_SYNTH macro, and because this synth has the same interface
as Synth it's not necessary to create a header file. You can create an instance of this synth using
SynthFactory::createInstance(SYNTH_NAME), where SYNTH_NAME is the name supplied to TONIC_REGISTER_SYNTH.
*/

#ifndef TonicDemo_FilteredNoise_h
#define TonicDemo_FilteredNoise_h

#include "Tonic.h"

using namespace Tonic;

class FilteredNoiseSynth : public Synth{
public:
  FilteredNoiseSynth(){
    vector<float> midiNums;
    midiNums.push_back(-24);
    midiNums.push_back(0);
    midiNums.push_back(3);
    midiNums.push_back(7);
    midiNums.push_back(10);

    const float cutoffMult = 100;

    PinkNoise noise = PinkNoise();

    ControlParameter volume = addParameter("volume", 0.5f).displayName("Volume");

    ControlGenerator cutoffCtrl = addParameter("cutoff", 0.5);

    Generator q_v = addParameter("Q", 5.0f).min(0.0f).max(10.0f).smoothed();
//
//    ControlGenerator lowBasFreq =   ControlFloor().input( midiNums.at(0) + 12 + cutoffCtrl * cutoffMult ) >> ControlMidiToFreq();
//    ControlGenerator fmAmt = ControlValue(0.1);
//    Generator cutoffSlowSwell = (SineWave().freq(0.1) + 1.0f) * 500;
//    cutoffSlowSwell = FixedValue(500);
//    Generator lpfCutoff = 300 + (cutoffSlowSwell + 0.5 * SineWave().freq(10) * cutoffSlowSwell);
//    LPF12 filter = LPF12().cutoff(lpfCutoff);
//
//    ControlGenerator toothyBassRandomAmp = ControlRandom()
//        .min(-0.1)
//        .max(1)
//        .trigger(
//            ControlMetro().bpm( ControlRandom().min(10).max(30) )
//        );
//
//    Generator toothyBassSwell = ( (toothyBassRandomAmp * toothyBassRandomAmp * toothyBassRandomAmp * 5) >> ControlPrinter().message("toothyBassSwell %f") ).smoothed(10);
//
//    Generator lowToothyBass =
//      SineWave().freq(  lowBasFreq ) * SineWave().freq(2000);
//
//    lowToothyBass = filter.input( lowToothyBass );
//    lowToothyBass = lowToothyBass * toothyBassSwell;

    Adder sumOfFilters;

    for(int i = 0; i < midiNums.size(); i++){
      Generator tremelo = (SineWave().freq( randomFloat(0.1, 0.3) ) + 1.5) * 0.3;
      Generator cutoff = ControlMidiToFreq().input( ControlFloor().input( midiNums.at(i) + cutoffCtrl * cutoffMult  )).smoothed().length(0.01);
      HPF24 filter = HPF24().Q( q_v*2.0 ).cutoff( cutoff ).normalizesGain(true);
      sumOfFilters.input((noise >> filter) * tremelo)*50;
    }

    //FBCombFilter combfilter = FBCombFilter().input(sumOfFilters).delayTime(SineWave().freq(0.1)*5.0).scaleFactor(5);

    Generator toneWithDelay = StereoDelay(0.5, 0.75).input(sumOfFilters).wetLevel(0.5).feedback(0.3);
    Generator mix = toneWithDelay*2;

    Generator output = mix * volume.smoothed();
    setOutputGen(output);


  }
};

TONIC_REGISTER_SYNTH(FilteredNoiseSynth);

#endif
