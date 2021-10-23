/*
  ==============================================================================

    EQSection.cpp
    Created: 27 Aug 2021 9:12:04pm
    Author:  Adam Murdock

  ==============================================================================
*/

#include "EQSection.h"

EQSection::EQSection()
{
    for (int i = 0; i < numOfFilters; i++)
    {
        filters[i].setFreq(freqBands[i] / os);
        filters[i].setAmpdB(0.f);
    }
    dc1.setFreq(10.f);
    dc2.setFreq(20.f);
    aAFilter.setQuality(4);
}

float EQSection::processSample(float sample, int channel)
{
    for (int i = 0; i < numOfFilters; i++)
    {
        sample = filters[i].processSample(sample, channel);
    }
    //	sample = dc2.processSample(sample, channel);
    //	sample = aAFilter.process(sample, channel);
    return sample;
}

void EQSection::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    for (int i = 0; i < numOfFilters; i++)
    {
        filters[i].setFs(sampleRate);
    }
    this->alpha = exp(-log(9)) / (sampleRate * smoothTime);
}

void EQSection::setBoostAndCut(int param, float value)
{
	params[param] = value;
    if (param >= 0 && param <= 4)
    {
        if (params[param] > 0)
        {
            filters[param].setAmpdB(params[param] * 3);
        }
        if (params[param] < 0)
        {
            filters[param].setAmpdB(params[param]);
        }
        if (params[param] == 0)
        {
            filters[param].setAmpdB(0.f);
        }
    }
    if (param == 5)
    {
        if (params[param] < 0)
        {
            
        }
        if (params[param] > 10)
        {
            filters[param].setAmpdB(20.f);
        }
        else
        {
            filters[param].setAmpdB(params[param] * 2);
        }
    }
}

void EQSection::setHighFreq(int highFreq)
{
    if (highFreq == 0)
    {
        filters[numOfFilters - 1].setFreq(2500.f);
    }
    if (highFreq == 1)
    {
        filters[numOfFilters - 1].setFreq(5000.f);
    }
    if (highFreq == 2)
    {
        filters[numOfFilters - 1].setFreq(10000.f);
    }
    if (highFreq == 3)
    {
        filters[numOfFilters - 1].setFreq(20000.f);
    }
    if (highFreq == 4)
    {
        filters[numOfFilters - 1].setFreq(40000.f);
    }
}
float EQSection::mapValues(float value, float oldMax, float oldMin,
                           float newMax, float newMin)
{
    float oldSpan = oldMax - oldMin;
    float newSpan = newMax - newMin;

    float valScaled = value / oldSpan;
    return abs(newMin) + (valScaled * newSpan);
}

void EQSection::setOS(int os) { this->os = os; }

float EQSection::smoothValues(float value, float smoothValue)
{
    //	alpha * smoothedVal + (1- alpha) * knobValue;
    this->smoothedValue = (alpha * smoothedValue) + (1 - alpha) * value;
    return smoothedValue;
}
