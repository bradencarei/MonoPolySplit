/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MonoPolySplitAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer, public juce::Slider::Listener
{
public:
    MonoPolySplitAudioProcessorEditor (MonoPolySplitAudioProcessor&);
    ~MonoPolySplitAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override ;
    
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MonoPolySplitAudioProcessor& audioProcessor;
    juce::Slider thresh;
    juce::Slider gainMono;
    juce::Slider gainPoly;
    juce::Slider distMono;
    juce::Slider distPoly;
    juce::Slider attackKnob;
    juce::Slider releaseKnob;
    
    void sliderValueChanged(juce::Slider* slider) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonoPolySplitAudioProcessorEditor)
};
