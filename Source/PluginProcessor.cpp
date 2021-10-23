/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
MAGnificentEQAudioProcessor::MAGnificentEQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
      apvts(*this, nullptr, "Params", createParams())
#endif
{
}

MAGnificentEQAudioProcessor::~MAGnificentEQAudioProcessor() {}

//==============================================================================
const juce::String MAGnificentEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MAGnificentEQAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MAGnificentEQAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MAGnificentEQAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MAGnificentEQAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int MAGnificentEQAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are
              // 0 programs, so this should be at least 1, even if you're not
              // really implementing programs.
}

int MAGnificentEQAudioProcessor::getCurrentProgram() { return 0; }

void MAGnificentEQAudioProcessor::setCurrentProgram(int index) {}

const juce::String MAGnificentEQAudioProcessor::getProgramName(int index)
{
    return {};
}

void MAGnificentEQAudioProcessor::changeProgramName(int index,
                                                    const juce::String &newName)
{
}

//==============================================================================
void MAGnificentEQAudioProcessor::prepareToPlay(double sampleRate,
                                                int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    //    eqSection.prepareToPlay(sampleRate * os, samplesPerBlock);
    //	saturationSection1.prepareToPlay(sampleRate * os, samplesPerBlock);
    //	saturationSection2.prepareToPlay(sampleRate * os, samplesPerBlock)

	eqSection.setOS(os);
    eqSection.prepareToPlay(sampleRate * os, samplesPerBlock * os);
    saturationSection1.prepareToPlay(sampleRate * os, samplesPerBlock * os);
    saturationSection2.prepareToPlay(sampleRate * os, samplesPerBlock * os);

    upsample.prepare(os, quality);
    downsample.prepare(os, quality);
    aAFilter.setQuality(quality);
}

void MAGnificentEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MAGnificentEQAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void MAGnificentEQAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                               juce::MidiBuffer &midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.

    auto inputGain = apvts.getRawParameterValue("INPUT")->load();
    inputGain = juce::Decibels::decibelsToGain(inputGain);

    auto drive = apvts.getRawParameterValue("DRIVE")->load();

    auto subGain = apvts.getRawParameterValue("SUBGAIN")->load();
    auto band1Gain = apvts.getRawParameterValue("BAND1")->load();
    auto band2Gain = apvts.getRawParameterValue("BAND2")->load();
    auto band3Gain = apvts.getRawParameterValue("BAND3")->load();
    auto band4Gain = apvts.getRawParameterValue("BAND4")->load();
    auto airGain = apvts.getRawParameterValue("AIRGAIN")->load();
    auto outputGain = apvts.getRawParameterValue("OUTPUT")->load();
    outputGain = juce::Decibels::decibelsToGain(outputGain);

    inputGain = smoothValue(oldInput, inputGain);
    drive = smoothValue(oldDrive, drive);
    subGain = smoothValue(oldSub, subGain);
    band1Gain = smoothValue(oldBand1Gain, band1Gain);
    band2Gain = smoothValue(oldBand2Gain, band2Gain);
    band3Gain = smoothValue(oldBand3Gain, band3Gain);
    band4Gain = smoothValue(oldBand4Gain, band4Gain);
    airGain = smoothValue(oldAirGain, airGain);
    outputGain = smoothValue(oldOutput, outputGain);
    
    float currentParams[6] = {subGain,   band1Gain, band2Gain,
                              band3Gain, band4Gain, airGain};

    auto limit = juce::Decibels::decibelsToGain(18.f);
    
    saturationSection1.setDistValue(drive * .5);
    saturationSection2.setDistValue(drive * .25);

    for (int i = 0; i < 6; i++)
    {
        eqSection.setBoostAndCut(i, currentParams[i]);
    }

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto *channelData = buffer.getWritePointer(channel);
        upsample.process(channelData, upbuffer, channel,
                         buffer.getNumSamples());
        for (int sample = 0; sample < buffer.getNumSamples() * os; ++sample)
        {
            float input = upbuffer[sample];
            input *= inputGain;
            input = saturationSection1.processSample(input, channel);
            input = eqSection.processSample(input, channel);
            input = saturationSection2.processSample(input, channel);
            input *= outputGain;
            if (juce::Decibels::decibelsToGain(input) >= limit) {
                input = juce::Decibels::decibelsToGain(limit);
            }
            upbuffer[sample] = input;
        }
        downsample.process(upbuffer, channelData, channel,
                           buffer.getNumSamples());
    }
    
    oldInput = inputGain;
    oldDrive = drive;
    oldSub = subGain;
    oldBand1Gain = band1Gain;
    oldBand2Gain = band2Gain;
    oldBand3Gain = band3Gain;
    oldBand4Gain = band4Gain;
    oldAirGain = airGain;
    oldOutput = outputGain;
}

//==============================================================================
bool MAGnificentEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *MAGnificentEQAudioProcessor::createEditor()
{
    return new MAGnificentEQAudioProcessorEditor(*this);
}

//==============================================================================
void MAGnificentEQAudioProcessor::getStateInformation(
    juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MAGnificentEQAudioProcessor::setStateInformation(const void *data,
                                                      int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory
    // block, whose contents will have been created by the getStateInformation()
    // call.
    std::unique_ptr<juce::XmlElement> xmlState(
        getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new MAGnificentEQAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout
MAGnificentEQAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Adding input slider to params vector
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "INPUT", "Input", -12.f, 12.f, 0.f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "DRIVE", "Drive", 0.f, 1.f, 0.f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "SUBGAIN", "Sub Gain", -5.f, 5.f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "BAND1", "Band1", -5.f, 5.f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "BAND2", "Band2", -5.f, 5.f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "BAND3", "Band3", -5.f, 5.f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "BAND4", "Band4", -5.f, 5.f, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "AIRGAIN", "Air Gain", 0.f, 10.f, 0.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "OUTPUT","Output", -18.f, 18.f, 0.f));

    return {params.begin(), params.end()};
}

float MAGnificentEQAudioProcessor::smoothValue(float oldValue, float knobValue) {
    float diff = oldValue - knobValue;
    if (diff <= .001 || diff >= -.001)
    {
        return knobValue;
    }
    else
    {
        return (oldValue + knobValue) * .5f;
    }
    
}
