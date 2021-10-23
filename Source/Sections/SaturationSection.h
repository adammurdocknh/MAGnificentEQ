/*
  ==============================================================================

    SaturationSection.h
    Created: 27 Aug 2021 9:12:58pm
    Author:  Adam Murdock

  ==============================================================================
*/

#pragma once
#include "../Components/Biquad.h"
#include <JuceHeader.h>
#include <cmath>
class SaturationSection {
  public:
    SaturationSection();

    float processSample(float sample, int channel);

    void prepareToPlay(double sampleRate, int samplesPerBlock);
	
	void setDistValue(float distValue);

  private:
    float distortion(float sample);
	
	float distValue = .1f;
    
    float freq = 20000.f;

    double sampleRate;

    int samplesPerBlock;
	
	Biquad lpf = Biquad(Biquad::FilterType::LPF,1);
	Biquad dc = Biquad(Biquad::FilterType::HPF,.7);
};
