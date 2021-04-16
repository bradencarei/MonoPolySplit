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
    
    //Populate the buffer to be pushed into fifo array
    analysisBuffer[channel][index[channel]] = sample;
    index[channel]++;
    
}

bool SigTypeAnalysis::checkSigType(int channel){
    int count = 0;
    arrayMax = 0;
    int maxIndex = 0;
    index[channel] = 0;
    
    //Push samples into fifo
    for (auto i = 0; i < bufferSiz; ++i){
        pushNextSampleIntoFifo (analysisBuffer[channel][i]);
        }
    
    //Perform fft
    fft.performFrequencyOnlyForwardTransform (fftData.data());
    for (auto p = 40; p < bufferSiz/3; ++p){
        //save comparison variable
        bucket = fftData[p];
        //Check to see if comparison variable is greater than max and if so set newmax
        if(arrayMax < bucket){
            arrayMax = bucket;
            maxIndex = p;
        }
        
    }
        
    //Calculate the threshold in percentage of max
    threshCalc = arrayMax*(analysisThresh);
    
    //Necessary for fft to analyze next array
    nextFFTBlockReady = false;
    for(auto t = 0; t<bufferSiz/3;t++)
    {
        
        //check for freq domain peaks greater than percentage of max
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
