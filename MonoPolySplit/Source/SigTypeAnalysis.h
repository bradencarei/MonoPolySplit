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
class SigTypeAnalysis  : public juce::Component
{
public:
    SigTypeAnalysis();
    ~SigTypeAnalysis() override;

    void bufferPopulate(float sample, int channel);
    bool checkSigType(int channel);

    static constexpr auto fftOrder = 12;                // [1]
    static constexpr auto fftSize  = 1 << fftOrder;     // [2]
    
    void pushNextSampleIntoFifo (float sample) noexcept;
    
    float thresh=0;
    
    void setThresh(float thresh);
    float getThresh();
    int getBufferSize();
    int bufferSize = fftSize;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SigTypeAnalysis)
    \
    float analysisBuffer[2][fftSize];
    int index[2] = {0};
    juce::dsp::FFT fft;
    std::array<float, fftSize> fifo;                    // [4]
    std::array<float, fftSize * 2> fftData;             // [5]
    int fifoIndex = 0;                                  // [6]
    bool nextFFTBlockReady = false;                     // [7]
    float arrayMax=0;
    float threshCalc;
    float bucket = 0;
    

};
