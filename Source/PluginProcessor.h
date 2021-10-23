/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "Components/AntiAliasFilter.h"
#include "Components/DownSampling2Stage.h"
#include "Components/UpSampling2Stage.h"
#include "Sections/EQSection.h"
#include "Sections/SaturationSection.h"
#include <JuceHeader.h>

//==============================================================================
/**
 */
class MAGnificentEQAudioProcessor : public juce::AudioProcessor {
  public:
    //==============================================================================
    MAGnificentEQAudioProcessor();
    ~MAGnificentEQAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState apvts;

    int os = 4;
    
    float smoothValue(float oldValue, float knobValue);

  private:
    //==============================================================================

    UpSampling2Stage upsample;
    DownSampling2Stage downsample;

//    int os = 4;
    int quality = 3;

	AntiAliasFilter aAFilter;
    EQSection eqSection;
//    float upbuffer[16384] = {0.f};
	float upbuffer[32768] = {0.f};
//	float upbuffer[192000] = {0.f};
	SaturationSection saturationSection1;
	SaturationSection saturationSection2;
	
    float oldInput = 0.f;
    float oldDrive = 0.f;
    float oldSub = 0.f;
    float oldBand1Gain = 0.f;
    float oldBand2Gain = 0.f;
    float oldBand3Gain = 0.f;
    float oldBand4Gain = 0.f;
    float oldAirGain = 0.f;
    float oldOutput = 0.f;

	juce::AudioProcessorValueTreeState::ParameterLayout createParams();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MAGnificentEQAudioProcessor)
};
