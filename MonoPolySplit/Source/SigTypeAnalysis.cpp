/*
  ==============================================================================

    SigTypeAnalysis.cpp
    Created: 31 Mar 2021 7:56:13pm
    Author:  Braden Carei

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SigTypeAnalysis.h"
#include "PluginProcessor.h"

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
    
    analysisBuffer[channel][index[channel]] = sample;
    index[channel]++;
    
}

bool SigTypeAnalysis::checkSigType(int channel){
    int count = 0;
    arrayMax = 0;
    int maxIndex = 0;
    index[channel] = 0;
    for (auto i = 0; i < bufferSiz; ++i){
        pushNextSampleIntoFifo (analysisBuffer[channel][i]);
        }
    fft.performFrequencyOnlyForwardTransform (fftData.data());
    for (auto p = 40; p < bufferSiz/3; ++p){
        bucket = fftData[p];
        if(arrayMax < bucket){
            arrayMax = bucket;
            maxIndex = p;
        }
        
    }
        
    threshCalc = arrayMax*(analysisThresh);
    nextFFTBlockReady = false;
    for(auto t = maxIndex; t<bufferSiz/3;t++)
    {
        
        if(fftData[t] > threshCalc){
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

void SigTypeAnalysis::setThresh(float threshold)
{
    analysisThresh = threshold;
}
float SigTypeAnalysis::getThresh()
{
    return analysisThresh;
}


int SigTypeAnalysis::getBufferSize()
{
    return bufferSiz;
}
