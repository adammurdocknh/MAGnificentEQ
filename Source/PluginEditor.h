/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "PluginProcessor.h"

#include "LookAndFeel/BlackKnobLookAndFeel.h"
#include "LookAndFeel/BlueKnobLookAndFeel.h"
#include "LookAndFeel/GreenKnobLookAndFeel.h"
#include "LookAndFeel/MainLookAndFeel.h"
#include "LookAndFeel/OrangeKnobLookAndFeel.h"
#include "LookAndFeel/RedKnobLookAndFeel.h"
#include "LookAndFeel/YellowKnobLookAndFeel.h"
#include "LookAndFeel/VioletKnobLookAndFeel.h"
#include <JuceHeader.h>

//==============================================================================
/**
 */
class MAGnificentEQAudioProcessorEditor : public juce::AudioProcessorEditor
{
  public:
    MAGnificentEQAudioProcessorEditor(MAGnificentEQAudioProcessor &);
    ~MAGnificentEQAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

  private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MAGnificentEQAudioProcessor &audioProcessor;

	juce::Label headerLabel;
    juce::Label inputLabel;
    juce::Label driveLabel;
    juce::Label subLabel;
    juce::Label band1Label;
    juce::Label band2Label;
    juce::Label band3Label;
    juce::Label band4Label;
    juce::Label airGainLabel;
    juce::Label airFreqLabel;
	juce::Label outputGainLabel;

    juce::Slider inputGainKnob;
    juce::Slider driveGainKnob;
    juce::Slider subGainKnob;
    juce::Slider band1GainKnob;
    juce::Slider band2GainKnob;
    juce::Slider band3GainKnob;
    juce::Slider band4GainKnob;
    juce::Slider airGainKnob;
    juce::Slider airFreqKnob;
	juce::Slider outputGainKnob;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        inputGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        driveGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        subGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        band1GainAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        band2GainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        band3GainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        band4GainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        airGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        airFreqAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
		  outputGainAttachment;

    MainLookAndFeel mainLookAndFeel;
    BlackKnobLookAndFeel subGainLookAndFeel;
    BlueKnobLookAndFeel band1GainLookAndFeel;
    GreenKnobLookAndFeel band2GainLookAndFeel;
    RedKnobLookAndFeel band3GainLookAndFeel;
    OrangeKnobLookAndFeel band4GainLookAndFeel;
    YellowKnobLookAndFeel airGainLookAndFeel;
	VioletKnobLookAndFeel violetKnobLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        MAGnificentEQAudioProcessorEditor)
};
