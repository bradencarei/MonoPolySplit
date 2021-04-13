#ifndef LARGEKNOB_H_INCLUDED
#define LARGEKNOB_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class LargeKnob : public juce::LookAndFeel_V3
{
public:
    juce::Image sprite = juce::ImageCache::getFromMemory(<#const void *imageData#>, <#int dataSize#>);
    
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider);
};

#endif /* LARGEKNOB_H_INCLUDED */
