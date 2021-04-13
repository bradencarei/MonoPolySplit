/*
  ==============================================================================

    Distortion.cpp
    Created: 11 Apr 2021 1:56:15pm
    Author:  Braden Carei

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Distortion.h"

//==============================================================================
float Distortion::processSample(float x){
    
    smoothDrive = alpha * smoothDrive + (1.f-alpha) * drive;
    
    float y = (2.f/M_PI) * atan(smoothDrive * x);
    
    return y;
    
}

void Distortion::setDrive(float newDrive){
    if (newDrive <= 10.f && newDrive >= 1.f){
        drive = newDrive;
    }
}

void Distortion::setFs(float newFs){
    Fs = newFs;
    alpha = exp(-log(9)/(Fs*smoothTime));
}


