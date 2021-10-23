/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
MAGnificentEQAudioProcessorEditor::MAGnificentEQAudioProcessorEditor(
    MAGnificentEQAudioProcessor &p)
    : juce::AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(750, 250);
    float xOffset = 240;
	float yOffset = 100;

	juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
    juce::LookAndFeel::setDefaultLookAndFeel(&mainLookAndFeel);
	
   juce::Typeface::Ptr headerTFace = juce::Typeface::createSystemTypefaceFor(
	   BinaryData::HelveticaNowDisplayBlack_ttf,
	   BinaryData::HelveticaNowDisplayBlack_ttfSize);
	
	
	auto headerFont = juce::Font(headerTFace);
	headerFont.setHeight(144.f);
	
	
	headerLabel.setFont(headerFont);
	headerLabel.setText("MAGnificent EQ", juce::NotificationType::dontSendNotification);
	headerLabel.setBounds(10, 10, 700, 100);
	addAndMakeVisible(headerLabel);
	
	inputGainKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    inputGainKnob.setBounds(20, 20 + yOffset, 100, 100);
    inputGainKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    inputGainKnob.setValue(0.f);
    inputGainAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.apvts, "INPUT", inputGainKnob);
    inputGainKnob.setLookAndFeel(&subGainLookAndFeel);
    addAndMakeVisible(inputGainKnob);

    inputLabel.setText("Input", juce::NotificationType::dontSendNotification);
    inputLabel.setBounds(20, 120 + yOffset, 100, 18);
	inputLabel.setJustificationType(juce::Justification::horizontallyCentred);

    addAndMakeVisible(inputLabel);

    driveGainKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveGainKnob.setBounds(140, 20 + yOffset, 100, 100);
    driveGainKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    driveGainKnob.setValue(0.f);
    driveGainAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.apvts, "DRIVE", driveGainKnob);
    driveGainKnob.setLookAndFeel(&subGainLookAndFeel);
    addAndMakeVisible(driveGainKnob);

	driveLabel.setText("Drive", juce::NotificationType::dontSendNotification);
    driveLabel.setBounds(140,120 + yOffset, 100, 18);
    driveLabel.setJustificationType(juce::Justification::horizontallyCentred);
    addAndMakeVisible(driveLabel);
	
	
    subGainKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    subGainKnob.setBounds(20 + xOffset, 20 + yOffset, 50, 50);
    subGainKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    subGainKnob.setValue(0.f);
    subGainAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.apvts, "SUBGAIN", subGainKnob);
//    subGainKnob.setLookAndFeel(&subGainLookAndFeel);
	subGainKnob.setLookAndFeel(&subGainLookAndFeel);
    addAndMakeVisible(subGainKnob);

    subLabel.setText("10hz", juce::NotificationType::dontSendNotification);
    subLabel.setBounds(70 + xOffset, 20 + yOffset, 50, 50);
	subLabel.setJustificationType(juce::Justification::centred);
//    subLabel.setFont(juce::Font(juce::Typeface::createSystemTypefaceFor(
//        BinaryData::RobotoLight_ttf, BinaryData::RobotoLight_ttfSize)));
    //	subLabel.setFont(juce::Font(juce::Typeface::cre))
    addAndMakeVisible(subLabel);

    band1GainKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    band1GainKnob.setBounds(70 + xOffset, 70 + yOffset, 50, 50);
    band1GainKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    band1GainKnob.setValue(0.f);
    band1GainAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.apvts, "BAND1", band1GainKnob);
//    band1GainKnob.setLookAndFeel(&band1GainLookAndFeel);
    band1GainKnob.setLookAndFeel(&band1GainLookAndFeel);
    addAndMakeVisible(band1GainKnob);

    band1Label.setText("40hz", juce::NotificationType::dontSendNotification);
    band1Label.setBounds(120 + xOffset, 70 + yOffset, 50, 50);
	band1Label.setJustificationType(juce::Justification::centred);
//    band1Label.setFont(juce::Font(juce::Typeface::createSystemTypefaceFor(
//        BinaryData::RobotoLight_ttf, BinaryData::RobotoLight_ttfSize)));
    //	subLabel.setFont(juce::Font(juce::Typeface::cre))
    addAndMakeVisible(band1Label);

    band2GainKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    band2GainKnob.setBounds(120 + xOffset, 20 + yOffset, 50, 50);
    band2GainKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    band2GainKnob.setValue(0.f);
    //	band2GainKnob.setSkewFactor(4);
    band2GainAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.apvts, "BAND2", band2GainKnob);
//    band2GainKnob.setLookAndFeel(&band2GainLookAndFeel);
	band2GainKnob.setLookAndFeel(&band2GainLookAndFeel);
    addAndMakeVisible(band2GainKnob);

    band2Label.setText("160hz", juce::NotificationType::dontSendNotification);
    band2Label.setBounds(170 + xOffset, 20 + yOffset, 50, 50);
	band2Label.setJustificationType(juce::Justification::centred);
//    band2Label.setFont(juce::Font(juce::Typeface::createSystemTypefaceFor(
//        BinaryData::RobotoLight_ttf, BinaryData::RobotoLight_ttfSize)));
    //	subLabel.setFont(juce::Font(juce::Typeface::cre))
    addAndMakeVisible(band2Label);

    band3GainKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    band3GainKnob.setBounds(170 + xOffset, 70 + yOffset, 50, 50);
    band3GainKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    band3GainKnob.setValue(0.f);

    //	band3GainKnob.setSkewFactor(4);
    band3GainAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.apvts, "BAND3", band3GainKnob);
//    band3GainKnob.setLookAndFeel(&band3GainLookAndFeel);
	band3GainKnob.setLookAndFeel(&band3GainLookAndFeel);
    addAndMakeVisible(band3GainKnob);

    band3Label.setText("650hz", juce::NotificationType::dontSendNotification);
    band3Label.setBounds(220 + xOffset, 70 + yOffset, 50, 50);
	band3Label.setJustificationType(juce::Justification::centred);
//    band3Label.setFont(juce::Font(juce::Typeface::createSystemTypefaceFor(
//        BinaryData::RobotoLight_ttf, BinaryData::RobotoLight_ttfSize)));
    //	subLabel.setFont(juce::Font(juce::Typeface::cre))
    addAndMakeVisible(band3Label);

    band4GainKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    band4GainKnob.setBounds(220 + xOffset, 20 + yOffset, 50, 50);
    band4GainKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    band4GainKnob.setValue(0.f);
    //	band4GainKnob.setSkewFactor(4);
    //	band4GainKnob.setSkewFactorFromMidPoint(.5f);
    band4GainAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.apvts, "BAND4", band4GainKnob);
//    band4GainKnob.setLookAndFeel(&band4GainLookAndFeel);
	band4GainKnob.setLookAndFeel(&band4GainLookAndFeel);
    addAndMakeVisible(band4GainKnob);

    band4Label.setText("2.5khz", juce::NotificationType::dontSendNotification);
    band4Label.setBounds(270 + xOffset, 20 + yOffset, 50, 50);
//    band4Label.setFont(juce::Font(juce::Typeface::createSystemTypefaceFor(
//        BinaryData::RobotoLight_ttf, BinaryData::RobotoLight_ttfSize)));
	band4Label.setJustificationType(juce::Justification::centred);
    //	subLabel.setFont(juce::Font(juce::Typeface::cre))
    addAndMakeVisible(band4Label);

    airGainKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    airGainKnob.setBounds(270 + xOffset, 70 + yOffset, 50, 50);
    airGainKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    airGainKnob.setValue(0.f);
    airGainAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.apvts, "AIRGAIN", airGainKnob);
    airGainKnob.setLookAndFeel(&airGainLookAndFeel);
    addAndMakeVisible(airGainKnob);

    airGainLabel.setText("AIR GAIN",
                         juce::NotificationType::dontSendNotification);
    airGainLabel.setBounds(320 + xOffset, 70 + yOffset, 50, 50);
	airGainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(airGainLabel);
    
	outputGainKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outputGainKnob.setBounds(390 + xOffset, 20 + yOffset, 100, 100);
    outputGainKnob.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    outputGainKnob.setValue(0.f);
    outputGainAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            audioProcessor.apvts, "OUTPUT", outputGainKnob);
    outputGainKnob.setLookAndFeel(&subGainLookAndFeel);
	addAndMakeVisible(outputGainKnob);
    
    outputGainLabel.setText("Output", juce::NotificationType::dontSendNotification);
    outputGainLabel.setBounds(390 + xOffset, 120 + yOffset, 100, 18);
    outputGainLabel.setJustificationType(juce::Justification::horizontallyCentred);
    
    addAndMakeVisible(outputGainLabel);
	
}

MAGnificentEQAudioProcessorEditor::~MAGnificentEQAudioProcessorEditor() {}

//==============================================================================
void MAGnificentEQAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with
    // a solid colour)
    g.fillAll(juce::Colour(32, 120, 220));
    g.setColour(juce::Colours::white);
}

void MAGnificentEQAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
