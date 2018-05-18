/*
 *  phasor.cpp
 *  phasorExample
 *
 *  Created by Joshua Batty on 8/06/12.
 *  Copyright 2012 BMP. All rights reserved.
 *
 */
#include "phasor.h"

void phasor::Range( float fMin, float fMax ){ // Hz
    _dmin = fMin / (SR/2.f);
    _dmax = fMax / (SR/2.f);
}

void phasor::Rate( float rate ){ // cps
    _lfoInc = 2.f * F_PI * (rate / SR);
}

void phasor::Feedback( float fb ){ // 0 -> <1.
    _fb = fb;
}

void phasor::Depth( float depth ){  // 0 -> 1.
    _depth = depth;
}

float phasor::Update( float inSamp ){
    //calculate and update phaser sweep lfo...
    float d  = _dmin + (_dmax-_dmin) * ((sin( _lfoPhase ) +
                                         1.f)/2.f);
    _lfoPhase += _lfoInc;
    if( _lfoPhase >= F_PI * 2.f )
        _lfoPhase -= F_PI * 2.f;
    
    //update filter coeffs
    for( int i=0; i<6; i++ )
        _alps[i].Delay( d );
    
    //calculate output
    float y = 	_alps[0].Update(
                                _alps[1].Update(
                                                _alps[2].Update(
                                                                _alps[3].Update(
                                                                                _alps[4].Update(
                                                                                                _alps[5].Update( inSamp + _zm1 * _fb ))))));
    _zm1 = y;
    
    return inSamp + y * _depth;
}

