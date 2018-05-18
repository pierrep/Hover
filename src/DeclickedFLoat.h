#pragma once

/** ofSoundDeclickedFloat

 Declick a changing float value by using a DECLICKLEN sample ramp (around 1.5ms at 44.1kHz).
 You must call rebuildRampIfNecessary before processing every audio block, in order to apply incoming
 value changes.
 Also, you must call frameTick() for every audio frame (sample) to advance the ramp.

 @author damian
 */

 #define DECLICKLEN 256

class ofSoundDeclickedFloat
{
public:

	ofSoundDeclickedFloat( float startValue=0.0f ) { rampPos = 0; current = startValue; setValue( startValue ); }

	/// Return the current value, declicked
	float getDeclickedValue() { return current; }
	/// Return the raw value (the target for declicking)
	float getRawValue() { return target; }
	/// Set a new value + rebuild ramp
	inline void setValue( float newValue ) { if ( !isnan(newValue)&&finite(newValue) ) { target = newValue; rampNeedsRebuild = true; } }

	/// Rebuild the ramp, if necessary. Call before processing a block of samples.
	void rebuildRampIfNecessary() { if ( rampNeedsRebuild ) rebuildRamp(); rampNeedsRebuild = false; }

	/// Update, to be called for every frame
	inline void frameTick() { current = ramp[rampPos]; ramp[rampPos] = target; rampPos = (rampPos+1)%DECLICKLEN; }

	/// operator overloading
	ofSoundDeclickedFloat& operator=( float new_value ) { setValue( new_value ); return *this; }
	ofSoundDeclickedFloat& operator+=( float adjustment ) { setValue( target+adjustment ); return *this; }
	ofSoundDeclickedFloat& operator-=( float adjustment ) { setValue( target-adjustment ); return *this; }

private:

	inline void rebuildRamp()
	{
        float v=current; float step= (target-current)/(DECLICKLEN-1);
        for ( int i=0; i<DECLICKLEN; i++ ) {
            ramp[(i+rampPos)%DECLICKLEN] = v; v += step;
        }
    }

	float current;
	float target;

	bool rampNeedsRebuild;
	int rampPos;
	float ramp[DECLICKLEN];
};
