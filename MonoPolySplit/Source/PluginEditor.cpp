/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SigTypeAnalysis.h"

//==============================================================================
MonoPolySplitAudioProcessorEditor::MonoPolySplitAudioProcessorEditor (MonoPolySplitAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 350);
    startTimer(60);
    
    thresh.addListener(this);
    //thresh.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    thresh.setBounds(200, 250, 200, 50);
    thresh.setValue(audioProcessor.sta->getThresh());
    thresh.setRange(0.0, 1.0,0.01f);
    thresh.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 25);
    addAndMakeVisible(thresh);
    
    gainMono.addListener(this);
    gainMono.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainMono.setBounds(25, 25, 75, 75);
    gainMono.setValue(audioProcessor.monoGain);
    gainMono.setRange(0.0, 1.0,0.01f);
    gainMono.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    addAndMakeVisible(gainMono);
    
    gainPoly.addListener(this);
    gainPoly.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainPoly.setBounds(500, 25, 75, 75);
    gainPoly.setValue(audioProcessor.polyGain);
    gainPoly.setRange(0.0, 1.0,0.01f);
    gainPoly.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    addAndMakeVisible(gainPoly);
    
    distMono.addListener(this);
    distMono.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    distMono.setBounds(25, 125, 75, 75);
    distMono.setValue(audioProcessor.monoDist);
    distMono.setRange(0.0, 1.0,0.01f);
    distMono.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    addAndMakeVisible(distMono);
    
    distPoly.addListener(this);
    distPoly.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    distPoly.setBounds(500, 125, 75, 75);
    distPoly.setValue(audioProcessor.polyDist);
    distPoly.setRange(0.0, 1.0,0.01f);
    distPoly.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 25);
    addAndMakeVisible(distPoly);
    
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
    
    
    g.setGradientFill(juce::ColourGradient(juce::Colours::palegoldenrod, 200, 300, juce::Colours::yellow.brighter(.4), 200, 110, true));
    g.fillRect(121, 21, 358, 218);
    g.setColour (juce::Colours::yellow.brighter(.4));
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
    
    g.setColour (juce::Colours::yellow.brighter(.4));
    g.drawLine(200, 110, 400, 110);
    
    g.setColour (juce::Colours::black);
    g.drawLine(300, 200, audioProcessor.arrowX+100, audioProcessor.arrowY);
    
     
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
}
