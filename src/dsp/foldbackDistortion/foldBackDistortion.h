//
//  foldBackDistortion.h
//  emptyExample
//
//  Created by Joshua Batty on 29/09/13.
//
//

#ifndef _OFFOLDBACK_H
#define _OFFOLDBACK_H

#include <math.h>

//--------------------------------------------------------------
class foldbackDistortion
{
public:
    foldbackDistortion()
    {
        threshold = 0.5;
    }
	
    //DECIMATOR
    double foldback(double i, float _threshold);
    
    float threshold;
};

#endif
