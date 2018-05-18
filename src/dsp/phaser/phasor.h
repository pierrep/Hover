/*
 *  phasor.h
 *  phasorExample
 *
 *  Created by Joshua Batty on 8/06/12.
 *  Copyright 2012 BMP. All rights reserved.
 *
 */

#ifndef _OFPHASOR_H
#define _OFPHASOR_H

#include <math.h>

#define SR (44100.f)  //sample rate
#define F_PI (3.14159f)

//--------------------------------------------------------------
class phasor
{
	public:
        phasor()
            : _fb( .197f )
            , _lfoPhase( 0.f )
            , _depth( 1.f )
            , _zm1( 0.f )
		{
            Range( 440.f, 1600.f );
            Rate( .05f );
		}
	
		//PHASOR
        void Range( float fMin, float fMax );
        void Rate( float rate );
        void Feedback( float fb );
        void Depth( float depth );
        float Update( float inSamp );

        
    private:
        class AllpassDelay{
        public:
            AllpassDelay()
            : _a1( 0.f )
            , _zm1( 0.f )
            {}
            
            void Delay( float delay ){ //sample delay time
                _a1 = (1.f - delay) / (1.f + delay);
            }
            
            float Update( float inSamp ){
                float y = inSamp * -_a1 + _zm1;
                _zm1 = y * _a1 + inSamp;
                
                return y;
            }
        private:
            float _a1, _zm1;
        };
        
        AllpassDelay _alps[6];
        
        float _dmin, _dmax; //range
        float _fb; //feedback
        float _lfoPhase;
        float _lfoInc;
        float _depth;
        
        float _zm1;
};

#endif
