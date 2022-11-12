/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OverdriveAudioProcessor::OverdriveAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
}

OverdriveAudioProcessor::~OverdriveAudioProcessor()
{
}

//==============================================================================
const juce::String OverdriveAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OverdriveAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool OverdriveAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool OverdriveAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double OverdriveAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OverdriveAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int OverdriveAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OverdriveAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String OverdriveAudioProcessor::getProgramName(int index)
{
    return {};
}

void OverdriveAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void OverdriveAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void OverdriveAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OverdriveAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void OverdriveAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto inChNum = getTotalNumInputChannels();
    auto outChNum = getTotalNumOutputChannels();
    auto smpNum = buffer.getNumSamples();
    if (turnedOn)
    {
        for (int channel = 0; channel < outChNum; ++channel)
        {
            if (channel > inChNum) buffer.clear(channel, 0, smpNum);
            else
            {
                auto* channelData = buffer.getWritePointer(channel, 0);
                for (int smp = 0; smp < smpNum; ++smp)
                {
                    sample = channelData[smp] * steps; 
                    orginalSample = channelData[smp];
                    channelData[smp] = countOverdrive(sample);
                }
            }
        }
    }

}

//==============================================================================
bool OverdriveAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OverdriveAudioProcessor::createEditor()
{
    return new OverdriveAudioProcessorEditor(*this);
}

//==============================================================================
void OverdriveAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OverdriveAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

float OverdriveAudioProcessor::countOverdrive(float currentSample)
{    
    if (currentSample < static_cast<float>(-2) / 3)             //statuc_cast na float jest kluczowe, if widzi int a nie float bez tego
    {                                                           
        currentSample = -1 * wetdryMix / 100;                   // wet
        currentSample += orginalSample * (1 - wetdryMix / 100); //dry
    }
    else if (currentSample < static_cast<float>(-1) / 3 && currentSample >= static_cast<float>(-2) / 3)
    {
        currentSample = (3 - pow((2 + 3 * currentSample),2)) / 3 * wetdryMix / 100;
        currentSample += orginalSample * (1 - wetdryMix / 100); 
    }
    else if (currentSample <= static_cast<float>(1) / 3 && currentSample >= static_cast<float>(-1) / 3)
    {
        currentSample = 2 * currentSample * wetdryMix / 100; 
        currentSample += (orginalSample) * (1 - wetdryMix / 100);
    }
    else if (currentSample <= static_cast<float>(2) / 3 && currentSample > static_cast<float>(1) / 3)
    {
        
        currentSample = (3 - pow((2 - 3 * currentSample), 2)) / 3 * wetdryMix / 100;
        currentSample += orginalSample * (1 - wetdryMix / 100);
    }
    else if (currentSample > static_cast<float>(2) / 3)
    {
        currentSample = 1 * wetdryMix / 100;
        currentSample += orginalSample * (1 - wetdryMix / 100);
    }

    return currentSample;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OverdriveAudioProcessor();
}
