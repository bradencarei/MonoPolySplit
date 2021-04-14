/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SigTypeAnalysis.h"
#include <math.h>

//==============================================================================
MonoPolySplitAudioProcessorEditor::MonoPolySplitAudioProcessorEditor (MonoPolySplitAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 350);
    startTimer(60);
    gainImage = juce::ImageCache::getFromMemory(BinaryData::Gain_png, BinaryData::Gain_pngSize);
    clipImage = juce::ImageCache::getFromMemory(BinaryData::Clip_png, BinaryData::Clip_pngSize);
    
    gainMono.addListener(this);
    gainMono.setLookAndFeel(&Knob);
    gainMono.setBounds(35, 45, 75, 75);
    gainMono.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainMono.setValue(audioProcessor.monoGain);
    gainMono.setRange(0.0, 1.0,0.01f);
    gainMono.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(gainMono);
    
    
    gainPoly.addListener(this);
    gainPoly.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainPoly.setBounds(510, 45, 75, 75);
    gainPoly.setValue(audioProcessor.polyGain);
    gainPoly.setRange(0.0, 1.0,0.01f);
    gainPoly.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(gainPoly);
    gainPoly.setLookAndFeel(&Knob);
    
    distMono.addListener(this);
    distMono.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    distMono.setBounds(35, 165, 75, 75);
    distMono.setValue(audioProcessor.monoDist);
    distMono.setRange(1.f, 10.f,0.01f);
    distMono.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(distMono);
    distMono.setLookAndFeel(&Knob);
    
    distPoly.addListener(this);
    distPoly.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    distPoly.setBounds(510, 165, 75, 75);
    distPoly.setValue(audioProcessor.polyDist);
    distPoly.setRange(1.f, 10.f,0.01f);
    distPoly.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(distPoly);
    distPoly.setLookAndFeel(&Knob);

    thresh.addListener(this);
    //thresh.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    thresh.setBounds(200, 250, 200, 50);
    thresh.setValue(audioProcessor.sta->getThresh());
    thresh.setRange(0.0, 1.0,0.01f);
    thresh.setTextBoxStyle(juce::Slider::NoTextBox, false, 200, 25);
    addAndMakeVisible(thresh);

    releaseKnob.addListener(this);
    //releaseKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    releaseKnob.setBounds(200, 300, 200, 50);
    releaseKnob.setValue(audioProcessor.releaseMS);
    releaseKnob.setRange(0.f, 2000.f, 1);
    releaseKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(releaseKnob);
    //releaseKnob.setLookAndFeel(&Knob);
    
}

MonoPolySplitAudioProcessorEditor::~MonoPolySplitAudioProcessorEditor()
{
}

//==============================================================================
void MonoPolySplitAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::black);
    g.drawRect(120, 20, 360, 220);
    
    
    
    g.setGradientFill(juce::ColourGradient(juce::Colours::aliceblue, 300, 300, juce::Colours::skyblue.darker(.5), 300, 110, true));
    g.fillRect(121, 21, 358, 218);
    g.setColour (juce::Colours::skyblue.darker(.5));
    g.drawLine(200, 110, 300, 110);
    
    
    
    g.setColour (juce::Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("Monophonic", 140,40,100,40, juce::Justification::centred, 1);
    g.drawFittedText ("Polyphonic", 370,40,100,40, juce::Justification::centred, 1);
    
    
    g.drawEllipse(297.5, 197.5, 5, 5, 1);
    
    
    g.setColour (juce::Colours::orangered);
    juce::Path path;
    path.startNewSubPath(200,110);
    path.quadraticTo(300, 70, 400, 110);
 
    path.closeSubPath();
    juce::PathStrokeType stroke (1.0f);
    g.strokePath(path,stroke, juce::AffineTransform());
    
    g.setColour (juce::Colours::skyblue.darker(.5));
    g.drawLine(200, 110, 400, 110);
    
    g.setColour (juce::Colours::black);
    g.drawLine(300, 200, audioProcessor.arrowX+100, audioProcessor.arrowY);
    
    g.drawImageAt(gainImage, 10, 0);
    g.drawImageAt(gainImage, 485, 0);
    
    g.drawImageAt(clipImage, 20, 120);
    g.drawImageAt(clipImage, 495, 120);
}


void MonoPolySplitAudioProcessorEditor::resized() {
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void MonoPolySplitAudioProcessorEditor::timerCallback()
{
    repaint();
}

void MonoPolySplitAudioProcessorEditor::sliderValueChanged(juce::Slider * slider)
{
    
    if (slider == &thresh){
        audioProcessor.sta->setThresh(thresh.getValue());
    }
    
    if (slider == &gainMono){
        audioProcessor.monoGain = gainMono.getValue();
    }
    
    if (slider == &gainPoly){
        audioProcessor.polyGain = gainPoly.getValue();
    }
    
    if (slider == &distMono){
        audioProcessor.monoDist = distMono.getValue();
    }
    
    if (slider == &gainPoly){
        audioProcessor.polyDist = distPoly.getValue();
    }
    
    if (slider == &releaseKnob){
        audioProcessor.releaseMS = releaseKnob.getValue();
        audioProcessor.releaseDec = floor((audioProcessor.releaseMS/1000)*audioProcessor.Fs);
    }
}
