/*
  ==============================================================================

    Tremolo.cpp
    Created: 15 Apr 2021 3:51:16pm
    Author:  Braden Carei

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Tremolo.h"
#include <math.h>

//==============================================================================

Tremolo::Tremolo(){}
Tremolo::~Tremolo(){}
float Tremolo::processSample(float x){
    
    lfo = amp * sin(currentAngle) + dc;
    
    float out = x*lfo;
    currentAngle += angleChange; // curAng = curAng + angCh
    if (currentAngle > pix2){
        currentAngle -= pix2  ;
    }
    
    return out;
    
}

void Tremolo::setRate(float newRate, int  Fs){
    rate = newRate;
    if(prevRate!=rate){
        angleChange = rate * 2.0f * M_PI / (float)Fs;
        lfo = 0.0f;
        currentAngle = 0.0f;
    }
    
    prevRate = rate;
}
