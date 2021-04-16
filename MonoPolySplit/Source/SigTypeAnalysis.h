/*
  ==============================================================================

    SigTypeAnalysis.h
    Created: 31 Mar 2021 7:56:13pm
    Author:  Braden Carei

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class SigTypeAnalysis
{
public:
    SigTypeAnalysis();
    ~SigTypeAnalysis();

    void bufferPopulate(float sample, int channel);
    bool checkSigType(int channel);

    static constexpr auto fftOrder = 13;                // [1]
    static constexpr auto fftSize  = 1 << fftOrder;     // [2]
    
    void pushNextSampleIntoFifo (float sample) noexcept;
    
    
    
    void setThresh(float thresh);
    float getThresh();
    int getBufferSize();
    int bufferSiz = fftSize;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SigTypeAnalysis)
    \
    float analysisBuffer[2][fftSize];
    float analysisThresh=1.f;
    int index[2] = {0};
    juce::dsp::FFT fft;
    std::array<float, fftSize> fifo;                    // [4]
    std::array<float, fftSize * 2> fftData;             // [5]
    int fifoIndex = 0;                                  // [6]
    bool nextFFTBlockReady = false;                     // [7]
    float arrayMax=0.f;
    float threshCalc=0.f;
    float bucket = 0.f;
    

};
