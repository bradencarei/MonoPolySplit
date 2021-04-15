/*
  ==============================================================================

    Tremolo.cpp
    Created: 15 Apr 2021 3:51:16pm
    Author:  Braden Carei

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Tremolo.h"

//==============================================================================
float Tremolo::processSample(float x, float Fs){
    
    lfo = amp * sin(currentAngle) + dc;
    
    
    currentAngle += angleChange; // curAng = curAng + angCh
    if (currentAngle > pix2){
        currentAngle -= pix2;
    }
    
    return  x * lfo;
    
}

void Tremolo::setRate(float newRate, float Fs){
    rate = newRate;
    angleChange = rate * 2.0f * M_PI / Fs;
}
