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
    gainMono.setBounds(35, 40, 75, 75);
    gainMono.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainMono.setValue(audioProcessor.monoGain);
    gainMono.setRange(0.f, 4.f,0.01f);
    gainMono.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(gainMono);
    
    
    gainMonoLabel.setBounds(37,100 , 60, 20);
    gainMonoLabel.setText(juce::String((float)(round((20*log10(audioProcessor.monoGain))*10))/10) + "  dB", juce::dontSendNotification);
    gainMonoLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(gainMonoLabel);
    
    gainPoly.addListener(this);
    gainPoly.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainPoly.setBounds(510, 40, 75, 75);
    gainPoly.setValue(audioProcessor.polyGain);
    gainPoly.setRange(0.f, 4.f,0.01f);
    gainPoly.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(gainPoly);
    gainPoly.setLookAndFeel(&Knob);
    
    gainPolyLabel.setBounds(512 ,100 , 60, 20);
    gainPolyLabel.setText(juce::String((float)(round((20*log10(audioProcessor.polyGain))*10))/10) + "  dB", juce::dontSendNotification);
    gainPolyLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(gainPolyLabel);
    
    distMono.addListener(this);
    distMono.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    distMono.setBounds(35, 155, 75, 75);
    distMono.setValue(audioProcessor.monoDist);
    distMono.setRange(1.f, 10.f,0.01f);
    distMono.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(distMono);
    distMono.setLookAndFeel(&Knob);
    
    distMonoLabel.setBounds(37, 215 , 60, 20);
    distMonoLabel.setText(juce::String((float)(round((20*log10(audioProcessor.monoDist))*10))/10) + "  dB", juce::dontSendNotification);
    distMonoLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(distMonoLabel);
    
    distPoly.addListener(this);
    distPoly.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    distPoly.setBounds(510, 155, 75, 75);
    distPoly.setValue(audioProcessor.polyDist);
    distPoly.setRange(1.f, 10.f,0.01f);
    distPoly.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(distPoly);
    distPoly.setLookAndFeel(&Knob);
    
    distPolyLabel.setBounds(512 ,215 , 60, 20);
    distPolyLabel.setText(juce::String((float)(round((20*log10(audioProcessor.polyDist))*10))/10) + "  dB", juce::dontSendNotification);
    distPolyLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(distPolyLabel);
    
    tremMono.addListener(this);
    tremMono.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    tremMono.setBounds(35, 270, 75, 75);
    tremMono.setValue(audioProcessor.monoTremRate);
    tremMono.setRange(.1f, 10.f,0.1f);
    tremMono.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    //addAndMakeVisible(tremMono);
    tremMono.setLookAndFeel(&Knob);
    
    tremMonoLabel.setBounds(37, 330, 60, 20);
    tremMonoLabel.setText(juce::String(audioProcessor.monoTremRate) + "  Hz", juce::dontSendNotification);
    tremMonoLabel.setJustificationType(juce::Justification::right);
   // addAndMakeVisible(tremMonoLabel);
    
    tremPoly.addListener(this);
    tremPoly.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    tremPoly.setBounds(510, 270, 75, 75);
    tremPoly.setValue(audioProcessor.polyTremRate);
    tremPoly.setRange(.1f, 10.f,0.1f);
    tremPoly.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    //addAndMakeVisible(tremPoly);
    tremPoly.setLookAndFeel(&Knob);
    
    tremPolyLabel.setBounds(512, 330, 60, 20);
    tremPolyLabel.setText(juce::String(audioProcessor.polyTremRate) + "  Hz", juce::dontSendNotification);
    tremPolyLabel.setJustificationType(juce::Justification::right);
    //addAndMakeVisible(tremPolyLabel);
    
    threshSlider.addListener(this);
    //thresh.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    threshSlider.setBounds(175, 255, 200, 50);
    threshSlider.setValue(audioProcessor.threshold);
    threshSlider.setRange(0.f, 1.f,0.01f);
    threshSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 200, 25);
    addAndMakeVisible(threshSlider);
    
    threshLabel.setBounds(365 ,268 , 60, 20);
    threshLabel.setText(juce::String(audioProcessor.threshold*100) + "  %", juce::dontSendNotification);
    threshLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(threshLabel);

    releaseKnob.addListener(this);
    //releaseKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    releaseKnob.setBounds(175, 310, 200, 50);
    releaseKnob.setValue(audioProcessor.releaseMS);
    releaseKnob.setRange(10, 2000, 1);
    releaseKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 75, 25);
    addAndMakeVisible(releaseKnob);
    //releaseKnob.setLookAndFeel(&Knob);
    
    releaseLabel.setBounds(365 , 323 , 80, 20);
    releaseLabel.setText(juce::String(round(audioProcessor.releaseMS)) + "  ms", juce::dontSendNotification);
    releaseLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(releaseLabel);
    
    monoTremButton.addListener(this);
    monoTremButton.setBounds(100,258,50,20);
    monoTremButton.setButtonText("Off");
    monoTremButton.setColour(0x1000100, juce::Colours::darkslategrey);
    monoTremButton.setColour(0x1000101, juce::Colours::dodgerblue.brighter(.6));
    monoTremButton.setToggleState(false, juce::dontSendNotification);
    //addAndMakeVisible(monoTremButton);
    
    polyTremButton.addListener(this);
    polyTremButton.setBounds(450,258,50,20);
    polyTremButton.setButtonText("Off");
    polyTremButton.setColour(0x1000100, juce::Colours::darkslategrey);
    polyTremButton.setColour(0x1000101, juce::Colours::dodgerblue.brighter(.6));
    polyTremButton.setToggleState(false, juce::dontSendNotification);
    //addAndMakeVisible(polyTremButton);
    
    sliderAttachments.emplace_back(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"monoGain",gainMono));
    sliderAttachments.emplace_back(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"polyGain",gainPoly));
    sliderAttachments.emplace_back(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"monoDist",distMono));
    sliderAttachments.emplace_back(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"polyDist",distPoly));
    sliderAttachments.emplace_back(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"release",releaseKnob));
    sliderAttachments.emplace_back(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"thresh",threshSlider));
    sliderAttachments.emplace_back(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"tremMono",tremMono));
    sliderAttachments.emplace_back(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.state,"tremPoly",tremPoly));
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
    
    g.drawImage(gainImage, 25, 10, 70,25,0,0,100,40);
    g.drawImage(gainImage, 500, 10, 70,25,0,0,100,40);
    
    //g.drawImageAt(clipImage, 23, 120);
    //g.drawImageAt(clipImage, 498, 120);
    
    g.drawImage(clipImage, 32, 122, 75,30,0,0,100,50);
    g.drawImage(clipImage, 507, 122, 75,30,0,0,100,50);
    
    //g.drawImage(tremImage, 25, 240, 70,25,0,0,100,40);
    //g.drawImage(tremImage, 500, 240, 70,25,0,0,100,40);
    
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
    
    if (slider == &threshSlider){
        audioProcessor.threshold = threshSlider.getValue();
        threshLabel.setText(juce::String(audioProcessor.threshold*100) + "  %", juce::dontSendNotification);
    }
    
    if (slider == &gainMono){
        audioProcessor.monoGain = gainMono.getValue();
        gainMonoLabel.setText(juce::String((round((20*log10(audioProcessor.monoGain))*10))/10) + "  dB", juce::dontSendNotification);
    }
    
    if (slider == &gainPoly){
        audioProcessor.polyGain = gainPoly.getValue();
        gainPolyLabel.setText(juce::String((float)(round((20*log10(audioProcessor.polyGain))*10))/10) + "  dB", juce::dontSendNotification);
    }
    
    if (slider == &distMono){
        audioProcessor.monoDist = distMono.getValue();
        distMonoLabel.setText(juce::String((float)(round((20*log10(audioProcessor.monoDist))*10))/10) + "  dB", juce::dontSendNotification);
    }
    
    if (slider == &distPoly){
        audioProcessor.polyDist = distPoly.getValue();
        distPolyLabel.setText(juce::String((float)(round((20*log10(audioProcessor.polyDist))*10))/10) + "  dB", juce::dontSendNotification);
    }
    
    if (slider == &tremMono){
        audioProcessor.monoTremRate = tremMono.getValue();
        tremMonoLabel.setText(juce::String(audioProcessor.monoTremRate) + "  Hz", juce::dontSendNotification);
    }
    
    if (slider == &tremPoly){
        audioProcessor.polyTremRate = tremPoly.getValue();
        tremPolyLabel.setText(juce::String(audioProcessor.polyTremRate) + "  Hz", juce::dontSendNotification);
    }
    
    if (slider == &releaseKnob){
        audioProcessor.releaseMS = releaseKnob.getValue();
        audioProcessor.releaseDec = floor((audioProcessor.releaseMS/1000)*audioProcessor.Fs);
        releaseLabel.setText(juce::String(round(audioProcessor.releaseMS)) + "  ms", juce::dontSendNotification);
    }
}

void MonoPolySplitAudioProcessorEditor::buttonClicked(juce::Button * button){
    
    if (button == &monoTremButton){
        if(monoTremButton.getToggleState()==true){
            monoTremButton.setToggleState(false, juce::dontSendNotification);
            monoTremButton.setButtonText("Off");
            audioProcessor.monoTremTog = 0;
        }
        else{
            monoTremButton.setToggleState(true, juce::dontSendNotification);
            monoTremButton.setButtonText("On");
            audioProcessor.monoTremTog = 1;
        }
    }
    
    if (button == &polyTremButton){
        if(polyTremButton.getToggleState()==true){
            polyTremButton.setToggleState(false, juce::dontSendNotification);
            polyTremButton.setButtonText("Off");
            audioProcessor.polyTremTog = 0;
        }
        else{
            polyTremButton.setToggleState(true, juce::dontSendNotification);
            polyTremButton.setButtonText("On");
            audioProcessor.polyTremTog = 1;
        }
    }
    
}

