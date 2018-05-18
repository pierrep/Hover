//
//  foldBackDistortion.cpp
//  emptyExample
//
//  Created by Joshua Batty on 29/09/13.
//
//

#include "foldBackDistortion.h"


double foldbackDistortion::foldback(double i, float _threshold)
{
    threshold = _threshold;
    
    if (i>threshold || i<-threshold)
    {
        i= fabs(fabs(fmod(i - threshold, threshold*4)) - threshold*2) - threshold;
    }
    return i;
}