/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Knob.h"

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
    juce::Label  threshLabel;
    juce::Slider gainMono;
    juce::Label  gainMonoLabel;
    juce::Slider gainPoly;
    juce::Label  gainPolyLabel;
    juce::Slider distMono;
    juce::Label  distMonoLabel;
    juce::Slider distPoly;
    juce::Label  distPolyLabel;
    juce::Slider releaseKnob;
    juce::Label  releaseLabel;
    
    juce::Image gainImage;
    juce::Image clipImage;
    void sliderValueChanged(juce::Slider* slider) override;
    
    Knob Knob;
        
    //juce::LookAndFeel_V3 lookAndFeel3;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonoPolySplitAudioProcessorEditor)
};
