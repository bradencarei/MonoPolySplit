/*
  ==============================================================================

    Distortion.h
    Created: 11 Apr 2021 1:56:15pm
    Author:  Braden Carei

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Distortion  : public juce::Component
{
public:
    Distortion();
    ~Distortion() override;



private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Distortion)
};
