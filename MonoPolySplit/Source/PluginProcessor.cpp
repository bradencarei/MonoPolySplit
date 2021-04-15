/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SigTypeAnalysis.h"
#include <math.h>



//==============================================================================
MonoPolySplitAudioProcessor::MonoPolySplitAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
      AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), state(*this, nullptr, "PlugParams",
                               createParameterLayout())
#endif
{
    sta = new SigTypeAnalysis;
}

MonoPolySplitAudioProcessor::~MonoPolySplitAudioProcessor()
{
    delete sta;
}

juce::AudioProcessorValueTreeState::ParameterLayout MonoPolySplitAudioProcessor::createParameterLayout(){
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back( std::make_unique<juce::AudioParameterFloat> ("monoGain","Monophonic Gain",0.f,1.f,.01f) );
    params.push_back( std::make_unique<juce::AudioParameterFloat> ("polyGain","Polyphonic Gain",0.f,1.f,.01f) );
    params.push_back( std::make_unique<juce::AudioParameterFloat> ("monoDist","Monophonic Distortion",0.f,1.f,.01f) );
    params.push_back( std::make_unique<juce::AudioParameterFloat> ("polyDist","Polyphonic Distortion",0.f,1.f,.01f) );
    params.push_back( std::make_unique<juce::AudioParameterFloat> ("release","Release",0.f,2.f,1.f) );
    params.push_back( std::make_unique<juce::AudioParameterFloat> ("thresh","Threshold",0, 2000, 1) );
    
    return {params.begin() , params.end() };
    
}

//==============================================================================
const juce::String MonoPolySplitAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MonoPolySplitAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MonoPolySplitAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MonoPolySplitAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MonoPolySplitAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MonoPolySplitAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MonoPolySplitAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MonoPolySplitAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MonoPolySplitAudioProcessor::getProgramName (int index)
{
    return {};
}

void MonoPolySplitAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MonoPolySplitAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    Fs = sampleRate;
    thresh = *state.getRawParameterValue("thresh");
    monoGain = *state.getRawParameterValue("monoGain");
    polyGain = *state.getRawParameterValue("polyGain");
    monoDist = *state.getRawParameterValue("monoDist");
    polyDist = *state.getRawParameterValue("polyDist");
    releaseMS = *state.getRawParameterValue("release");
}

void MonoPolySplitAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MonoPolySplitAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MonoPolySplitAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        
        
        
        
        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            float x = buffer.getReadPointer(channel)[n];
            monoDistortion.setDrive(monoDist);
            polyDistortion.setDrive(polyDist);
            
            attackFS = floor((attackMS/1000)*Fs);
            releaseFS = floor((releaseMS/1000)*Fs);
            
        
            if(count[channel]<sta->getBufferSize())
                sta->bufferPopulate(buffer.getReadPointer(channel)[n], channel);

        // ..do something to the data...
            
            else{
                count[channel] = 0;
                stateAn = sta->checkSigType(channel);
            }
            
            
            if(stateAn != prevState || releaseDec>0)
            {
                stateAn = prevState;
                releaseDec = releaseDec - 1;;
            }
            
            else
            {
                releaseDec = releaseFS;
            }
            
                
            xMono = monoDistortion.processSample(x);
            xMono = xMono * monoGain;
            xPoly = polyDistortion.processSample(x);
            xPoly = xPoly * polyGain;
            
            
            count[channel]++;
            if(stateAn == true){
                    arrowX = 100;
                    arrowY = 95;
                    buffer.getWritePointer(channel)[n]= (releaseDec/releaseFS)*xMono + ((releaseFS-releaseDec)/releaseFS)*xPoly;
                }
                
            else{
                    arrowX = 300;
                    arrowY = 95;
                    buffer.getWritePointer(channel)[n]= (releaseDec/releaseFS)*xPoly + ((releaseFS-releaseDec)/releaseFS)*xMono;
                }
            
            prevState = stateAn;
            
        }
    }
}

//==============================================================================
bool MonoPolySplitAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MonoPolySplitAudioProcessor::createEditor()
{
    return new MonoPolySplitAudioProcessorEditor (*this);
}

//==============================================================================
void MonoPolySplitAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto currentState = state.copyState();
    std::unique_ptr<juce::XmlElement> xml (currentState.createXml());
    copyXmlToBinary(*xml, destData);
}

void MonoPolySplitAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xml ( getXmlFromBinary(data, sizeInBytes));
    if (xml && xml->hasTagName(state.state.getType())){
        state.replaceState(juce::ValueTree::fromXml(*xml));}
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MonoPolySplitAudioProcessor();
}
