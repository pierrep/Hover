//
//  StereoDelaySynth.cpp
//  TonicDemo
//
//  Created by Morgan Packard on 3/13/13.

//

#ifndef TonicDemo_StereoDelaySynth_cpp
#define TonicDemo_StereoDelaySynth_cpp

#include "Tonic.h"

using namespace Tonic;

class StereoDelaySynth : public Synth {

public:
  StereoDelaySynth(){

    ControlParameter volume = addParameter("volume", 0.5f).displayName("Volume");
 ControlMetro metro = ControlMetro().bpm(80);
    ControlGenerator freq = ControlRandom().trigger(metro).min(0).max(1);

    Generator tone = SquareWave().freq(
                                     freq * 0.25 + 100
                                     + 400
                                     ) * SineWave().freq(10);

    ADSR env = ADSR()
    .attack(0.01)
    .decay( 0.4 )
    .sustain(0)
    .release(0)
    .doesSustain(false)
    .trigger(metro);

    StereoDelay delay = StereoDelay(3.0f,3.0f)
    .delayTimeLeft( 0.5 + SineWave().freq(0.2) * 0.01)
    .delayTimeRight(0.55 + SineWave().freq(0.23) * 0.01)
    .feedback(0.3)
    .dryLevel(0.8)
    .wetLevel(0.2);

    Generator filterFreq = (SineWave().freq(0.01) + 1) * 200 + 225;

    LPF24 filter = LPF24().Q(2).cutoff( filterFreq );

    Generator mix = (( tone * env ) >> filter >> delay) * 0.2;

    Generator output = mix * volume.smoothed();
    setOutputGen(output);

  }


};

TONIC_REGISTER_SYNTH(StereoDelaySynth)

#endif
