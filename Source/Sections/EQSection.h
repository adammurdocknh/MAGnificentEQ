/*
  ==============================================================================

    EQSection.h
    Created: 27 Aug 2021 9:12:04pm
    Author:  Adam Murdock

  ==============================================================================
*/

#pragma once
#include "../Components/AntiAliasFilter.h"
#include "../Components/Biquad.h"
//#include "../Com"

class EQSection
{
  public:
    EQSection();
    float processSample(float sample, int channel);

    void prepareToPlay(double sampleRate, int samplesPerBlock);

    void setBoostAndCut(int param, float value);

    void setHighFreq(int highFreq);

    float mapValues(float value, float oldMax, float oldMin, float newMax,
                    float newMin);

    void setOS(int os);
	
	float smoothValues(float value, float smoothValue);

	
    int os = 4;

  private:
	
//	float subAmp = 0.f;
//	float lowAmp = 0.f;
//	float lmAmp = 0.f;
//	float hmAmp = 0.f;
//	float highAmp = 0.f;
//	float airAmp = 0.f;
	
    static const int numOfParams = 6;
    static const int numOfFilters = 6;
    float params[numOfParams];
	
	float smoothedValue = 0;
	float smoothTime = 8/48000;
    int highFreq = 10000.f;
    double sampleRate = 48000;
    
    float alpha = exp(-log(9)) / (sampleRate * smoothTime);


    float freqBands[numOfFilters] = {10.f, 40.f, 160.f, 650.f, 2500.f, 10000.f};
    Biquad filters[numOfFilters] = {
        Biquad(Biquad::FilterType::PEAK, .4f),
        Biquad(Biquad::FilterType::PEAK, .4f),
        Biquad(Biquad::FilterType::PEAK, .4f),
        Biquad(Biquad::FilterType::PEAK, .4f),
        Biquad(Biquad::FilterType::HSHELF, 1.f),
        Biquad(Biquad::FilterType::HSHELF, 1.f),
    };
	
	
    Biquad lshelf = Biquad(Biquad::FilterType::LSHELF, 1.f);
    Biquad dc1 = Biquad(Biquad::FilterType::HPF, 1.f);
    Biquad dc2 = Biquad(Biquad::FilterType::HPF, 1.f);
    Biquad dc3 = Biquad(Biquad::FilterType::LPF, 1.f);
    AntiAliasFilter aAFilter;
};
