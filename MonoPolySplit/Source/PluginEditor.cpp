/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MonoPolySplitAudioProcessorEditor::MonoPolySplitAudioProcessorEditor (MonoPolySplitAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    startTimer(60);
    
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
    g.drawRect(20, 20, 360, 220);
    
    g.setGradientFill(juce::ColourGradient(juce::Colours::palegoldenrod, 200, 300, juce::Colours::yellow.brighter(.4), 200, 110, true));
    g.fillRect(21, 21, 358, 218);
    g.setColour (juce::Colours::yellow.brighter(.4));
    g.drawLine(100, 110, 300, 110);
    
    
    
    g.setColour (juce::Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("Monophonic", 40,40,100,40, juce::Justification::centred, 1);
    g.drawFittedText ("Polyphonic", 270,40,100,40, juce::Justification::centred, 1);
    
    
    g.drawEllipse(197.5, 197.5, 5, 5, 1);
    
    g.setColour (juce::Colours::orangered);
    juce::Path path;
    path.startNewSubPath(100,110);
    path.quadraticTo(200, 70, 300, 110);
    path.closeSubPath();
    juce::PathStrokeType stroke (1.0f);
    g.strokePath(path,stroke, juce::AffineTransform());
    
    g.setColour (juce::Colours::yellow.brighter(.4));
    g.drawLine(100, 110, 300, 110);
    g.setColour (juce::Colours::black);
    g.drawLine(200, 200, audioProcessor.arrowX, audioProcessor.arrowY);
    
     
}


void MonoPolySplitAudioProcessorEditor::resized() {
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void MonoPolySplitAudioProcessorEditor::timerCallback()
{
    repaint();
}


