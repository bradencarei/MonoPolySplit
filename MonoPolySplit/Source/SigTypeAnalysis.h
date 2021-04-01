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

    static constexpr auto fftOrder = 10;                // [1]
    static constexpr auto fftSize  = 8192;     // [2]
    
    void pushNextSampleIntoFifo (float sample) noexcept;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SigTypeAnalysis)
    
    float analysisBuffer[2][16384];
    int index = 0;
    juce::dsp::FFT fft;
    std::array<float, fftSize> fifo;                    // [4]
    std::array<float, fftSize * 2> fftData;             // [5]
    int fifoIndex = 0;                                  // [6]
    bool nextFFTBlockReady = false;                     // [7]

};
