/*
  ==============================================================================

    Tremolo.h
    Created: 15 Apr 2021 3:51:16pm
    Author:  Braden Carei

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Tremolo {
public:

    float processSample(float x, float Fs);
    
    void setRate(float rate,float Fs);
    

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Tremolo)
    
    float rate = 1.f; // [0.1 - 10]
    float amp = 1/4;
    float dc = 1.f - amp;
    
    float lfo = 0.f;
    
    float currentAngle = 0.0f;
    float angleChange = 0.0f;
    float pix2 = 2.0f * M_PI;
};
