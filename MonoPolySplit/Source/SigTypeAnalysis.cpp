/*
  ==============================================================================

    SigTypeAnalysis.cpp
    Created: 31 Mar 2021 7:56:13pm
    Author:  Braden Carei

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SigTypeAnalysis.h"

//==============================================================================
SigTypeAnalysis::SigTypeAnalysis() : fft (fftOrder)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
}

SigTypeAnalysis::~SigTypeAnalysis()
{
    
    
    
}

void SigTypeAnalysis::bufferPopulate(float sample, int channel){
    
    analysisBuffer[channel][index] = sample;
    index++;
    
}
bool SigTypeAnalysis::checkSigType(int channel){
    int count = 0;
    index = 0;
    for (auto i = 0; i < 16384; ++i)
        pushNextSampleIntoFifo (analysisBuffer[channel][i]);
    
    fft.performFrequencyOnlyForwardTransform (fftData.data());
    
    for(auto i = 0; i<16384;i++)
    {
        if(fftData[i] > 15){
            count++;
        }
        
        if(count > 1)
            return false;
        
    }
    
    return true;
}

void SigTypeAnalysis::pushNextSampleIntoFifo (float sample) noexcept
    {
        // if the fifo contains enough data, set a flag to say
        // that the next line should now be rendered..
        if (fifoIndex == fftSize)       // [8]
        {
            if (! nextFFTBlockReady)    // [9]
            {
                std::fill (fftData.begin(), fftData.end(), 0.0f);
                std::copy (fifo.begin(), fifo.end(), fftData.begin());
                nextFFTBlockReady = true;
            }
 
            fifoIndex = 0;
        }
 
        fifo[(size_t) fifoIndex++] = sample; // [9]
    }
