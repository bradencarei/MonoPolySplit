/*
  ==============================================================================

    Knob.h
    Created: 13 Apr 2021 5:48:15pm
    Author:  Braden Carei

  ==============================================================================
*/
#ifndef KNOB_H_INCLUDED
#define KNOB_H_INCLUDED

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Knob  : public juce::LookAndFeel_V3
{
public:
    juce::Image sprite = juce::ImageCache::getFromMemory(BinaryData::Knob_png, BinaryData::Knob_pngSize);
    
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider);
};



#endif /* KNOB_H_INCLUDED */
