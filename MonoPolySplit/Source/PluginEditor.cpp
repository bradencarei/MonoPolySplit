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
    threshImage = juce::ImageCache::getFromMemory(BinaryData::Thresh_png, BinaryData::Thresh_pngSize);
    releaseImage = juce::ImageCache::getFromMemory(BinaryData::Release_png, BinaryData::Release_pngSize);
    tremImage = juce::ImageCache::getFromMemory(BinaryData::Trem_png, BinaryData::Trem_pngSize);
    
    gainMono.addListener(this);
    gainMono.setLookAndFeel(&Knob);
    gainMono.setBounds(35, 45, 75, 75);
    gainMono.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainMono.setValue(audioProcessor.monoGain);
    gainMono.setRange(0.f, 1.f,0.01f);
    gainMono.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(gainMono);
    
    
    gainPoly.addListener(this);
    gainPoly.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainPoly.setBounds(510, 45, 75, 75);
    gainPoly.setValue(audioProcessor.polyGain);
    gainPoly.setRange(0.f, 1.f,0.01f);
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
    
    tremMono.addListener(this);
    tremMono.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    tremMono.setBounds(35, 285, 75, 75);
    tremMono.setValue(audioProcessor.monoTremRate);
    tremMono.setRange(.1f, 10.f,0.1f);
    tremMono.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(tremMono);
    tremMono.setLookAndFeel(&Knob);
    
    tremPoly.addListener(this);
    tremPoly.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    tremPoly.setBounds(510, 285, 75, 75);
    tremPoly.setValue(audioProcessor.polyTremRate);
    tremPoly.setRange(.1f, 10.f,0.1f);
    tremPoly.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(tremPoly);
    tremPoly.setLookAndFeel(&Knob);
    
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
    thresh.setBounds(200, 255, 200, 50);
    thresh.setValue(audioProcessor.sta->getThresh());
    thresh.setRange(0.f, 1.f,0.01f);
    thresh.setTextBoxStyle(juce::Slider::NoTextBox, false, 200, 25);
    addAndMakeVisible(thresh);

    releaseKnob.addListener(this);
    //releaseKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    releaseKnob.setBounds(200, 310, 200, 50);
    releaseKnob.setValue(audioProcessor.releaseMS);
    releaseKnob.setRange(0, 2000.f, 1);
    releaseKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(releaseKnob);
    //releaseKnob.setLookAndFeel(&Knob);
    
    monoTremButton.addListener(this);
    monoTremButton.setBounds(100,258,50,20);
    monoTremButton.setButtonText("Off");
    monoTremButton.setColour(0x1000100, juce::Colours::darkslategrey);
    monoTremButton.setColour(0x1000101, juce::Colours::dodgerblue.brighter(.6));
    monoTremButton.setToggleState(false, juce::dontSendNotification);
    addAndMakeVisible(monoTremButton);
    
    polyTremButton.addListener(this);
    polyTremButton.setBounds(450,258,50,20);
    polyTremButton.setButtonText("Off");
    polyTremButton.setColour(0x1000100, juce::Colours::darkslategrey);
    polyTremButton.setColour(0x1000101, juce::Colours::dodgerblue.brighter(.6));
    polyTremButton.setToggleState(false, juce::dontSendNotification);
    addAndMakeVisible(polyTremButton);
    
    sliderAttachments.emplace_back(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"monoGain",gainMono));
    sliderAttachments.emplace_back(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"polyGain",gainPoly));
    sliderAttachments.emplace_back(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"monoDist",distMono));
    sliderAttachments.emplace_back(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"polyDist",distPoly));
    sliderAttachments.emplace_back(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"release",releaseKnob));
    sliderAttachments.emplace_back(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"thresh",thresh));
}

MonoPolySplitAudioProcessorEditor::~MonoPolySplitAudioProcessorEditor()
{
}

//==============================================================================
void MonoPolySplitAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::dimgrey);
    g.setColour (juce::Colours::black);
    g.drawRect(120, 20, 360, 220);
    
    
    
    g.setGradientFill(juce::ColourGradient(juce::Colours::aliceblue, 300, 300, juce::Colours::dodgerblue.brighter(.35), 300, 110, true));
    g.fillRect(121, 21, 358, 218);
    g.setColour (juce::Colours::lightcyan.darker(.5));
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
    
    g.setColour (juce::Colours::dodgerblue.brighter(.35));
    g.drawLine(200, 110, 400, 110);
    
    g.setColour (juce::Colours::black);
    g.drawLine(300, 200, audioProcessor.arrowX+100, audioProcessor.arrowY);
    
    //g.drawImageAt(gainImage, 10, 5);
    //g.drawImageAt(gainImage, 485, 5);
    
    g.drawImage(gainImage, 25, 15, 70,25,0,0,100,40);
    g.drawImage(gainImage, 500, 15, 70,25,0,0,100,40);
    
    //g.drawImageAt(clipImage, 23, 120);
    //g.drawImageAt(clipImage, 498, 120);
    
    g.drawImage(clipImage, 32, 132, 75,30,0,0,100,50);
    g.drawImage(clipImage, 507, 132, 75,30,0,0,100,50);
    
    g.drawImage(tremImage, 25, 255, 70,25,0,0,100,40);
    g.drawImage(tremImage, 500, 255, 70,25,0,0,100,40);
    
   // g.drawImageAt(threshImage, 120, 235);
    g.drawImage(threshImage, 258, 240, 85,30,0,0,130,50);
    //g.drawImageAt(releaseImage, 120, 295);
    g.drawImage(releaseImage, 250, 295, 100,30,0,0,155,50);//, <#int sourceX#>, <#int sourceY#>, <#int sourceWidth#>, <#int sourceHeight#>)
  
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
    
    if (slider == &tremMono){
        audioProcessor.monoTremRate = tremMono.getValue();
    }
    
    if (slider == &tremPoly){
        audioProcessor.polyTremRate = tremPoly.getValue();
    }
    
    if (slider == &releaseKnob){
        audioProcessor.releaseMS = releaseKnob.getValue();
        audioProcessor.releaseDec = floor((audioProcessor.releaseMS/1000)*audioProcessor.Fs);
    }
}

void MonoPolySplitAudioProcessorEditor::buttonClicked(juce::Button * button){
    
    if (button == &monoTremButton){
        if(monoTremButton.getToggleState()==true){
            monoTremButton.setToggleState(false, juce::dontSendNotification);
            monoTremButton.setButtonText("Off");
        }
        else{
            monoTremButton.setToggleState(true, juce::dontSendNotification);
            monoTremButton.setButtonText("On");
        }
    }
    
    if (button == &polyTremButton){
        if(polyTremButton.getToggleState()==true){
            polyTremButton.setToggleState(false, juce::dontSendNotification);
            polyTremButton.setButtonText("Off");
        }
        else{
            polyTremButton.setToggleState(true, juce::dontSendNotification);
            polyTremButton.setButtonText("On");
        }
    }
    
}

