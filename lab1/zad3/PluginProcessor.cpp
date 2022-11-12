/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Zad3AudioProcessor::Zad3AudioProcessor()
{
    addParameter(gain = new juce::AudioParameterFloat("gain", "Gain", 0.0f, 1.0f, 0.0f));
}

Zad3AudioProcessor::~Zad3AudioProcessor()
{
}

//==============================================================================
const juce::String Zad3AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Zad3AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Zad3AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Zad3AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Zad3AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Zad3AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Zad3AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Zad3AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Zad3AudioProcessor::getProgramName (int index)
{
    return {};
}

void Zad3AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Zad3AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Zad3AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Zad3AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void Zad3AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto currentGain = *gain * 1;
    if (currentGain == previousGain)
    {
        buffer.applyGain(currentGain);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(),
            previousGain, currentGain);
        previousGain = currentGain;
    }


    auto* channelData = buffer.getWritePointer(0, 0);
    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float randomSample = ((juce::Random::getSystemRandom().nextFloat() * 2) - 1) * currentGain;
        buffer.addSample(0, sample, randomSample);
        buffer.addSample(1, sample, randomSample);
    }
}

//==============================================================================
bool Zad3AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Zad3AudioProcessor::createEditor()
{
    return new Zad3AudioProcessorEditor (*this);
}

//==============================================================================
void Zad3AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Zad3AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Zad3AudioProcessor();
}
