/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SigTypeAnalysis.h"
#include "Distortion.h"
#include "Tremolo.h"
//==============================================================================
/**
*/
class MonoPolySplitAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MonoPolySplitAudioProcessor();
    ~MonoPolySplitAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    float arrowX = 200;
    float arrowY = 70;
    
    float monoGain = 1.f;
    float polyGain = 1.f;
    float monoDist = 1.f;
    float polyDist = 1.f;
    float threshold = 0.f;
    float attackMS = 0.f;
    float releaseMS = 0.f;
    
    float attackFS;
    float releaseFS=0.f;
    float xMono=0.f;
    float xPoly=0.f;
    float polyTremRate=1.f;
    float monoTremRate=1.f;
    int polyTremTog = 0;
    int monoTremTog = 0;
    double Fs;
    float releaseDec=0.f;
    SigTypeAnalysis sta;
    bool stateAn = true;
    bool outputState = true;
    float caseMult = 0.f;
    
    juce::AudioProcessorValueTreeState state;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
private:
    
    bool prevState;
    
    
    int count[2] = {0};
    Distortion polyDistortion;
    Distortion monoDistortion;
    
    
    
    Tremolo polyTremolo;
    Tremolo monoTremolo;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonoPolySplitAudioProcessor)
};
