#include "Knob.h"

void Knob::drawRotarySlider (juce::Graphics& g,
                                      int x,
                                      int y,
                                      int width,
                                      int height,
                                      float sliderPos,
                                      const float rotaryStartAngle,
                                      const float rotaryEndAngle,
                                      juce::Slider& slider)
{
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    
    juce::AffineTransform rotator;
    g.drawImageTransformed(sprite, rotator.rotated(angle, (float)(sprite.getWidth()/2), (float)(sprite.getHeight()/2)));
    
}
