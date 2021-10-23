/*
  ==============================================================================

    SaturationSection.cpp
    Created: 27 Aug 2021 9:12:58pm
    Author:  Adam Murdock

  ==============================================================================
*/

#include "SaturationSection.h"
SaturationSection::SaturationSection() { lpf.setFreq(freq); };

float SaturationSection::processSample(float sample, int channel) {
    sample = distortion(sample);
//	sample = distortion(sample);
    lpf.processSample(sample, channel);
    if (juce::Decibels::gainToDecibels(sample) > 24) {
        sample = juce::Decibels::gainToDecibels(24);
    }
    return sample;
};

void SaturationSection::prepareToPlay(double sampleRate, int samplesPerBlock) {
    this->sampleRate = sampleRate;
    this->samplesPerBlock = samplesPerBlock;
    lpf.setFs(sampleRate);
}

void SaturationSection::setDistValue(float distValue) {
//    if (distValue <= 0) {
//        this->distValue = 0;
//    };
//    if (distValue >= 1) {
//        this->distValue = .3f;
//    };
//    if (distValue > 0 && distValue < 1) {
//        this->distValue = distValue / 3;
//    };
    this->distValue = distValue / 3;
    lpf.setFreq(freq - (750 * distValue));
};

float SaturationSection::distortion(float sample) {
    sample = (sample - distValue * std::pow(sample, 3));
    return sample;
};
