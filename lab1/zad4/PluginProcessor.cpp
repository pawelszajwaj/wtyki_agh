/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Zad4AudioProcessor::Zad4AudioProcessor()
{
    addParameter(gain = new juce::AudioParameterFloat("gain", "Gain", 0.0f, 1.0f, 0.0f));
    //addParameter(frequency = new juce::AudioParameterFloat("frequency", "Frequency", 220.0f, 880.0f, 440.0f));
    frequency = 440;
}

Zad4AudioProcessor::~Zad4AudioProcessor()
{
}

//==============================================================================
const juce::String Zad4AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Zad4AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Zad4AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Zad4AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Zad4AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Zad4AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Zad4AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Zad4AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Zad4AudioProcessor::getProgramName (int index)
{
    return {};
}

void Zad4AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Zad4AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    updateAngleDelta(frequency);
}

void Zad4AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Zad4AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Zad4AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
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
        auto currentSample = (float)std::sin(currentAngle);
        currentAngle += angleDelta;
        updateAngleDelta(frequency);
        float randomSample = currentSample * currentGain;
        buffer.addSample(0, sample, randomSample);
        buffer.addSample(1, sample, randomSample);
    }       
}

//==============================================================================
bool Zad4AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Zad4AudioProcessor::createEditor()
{
    return new Zad4AudioProcessorEditor (*this);
}

//==============================================================================
void Zad4AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Zad4AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Zad4AudioProcessor();
}

void Zad4AudioProcessor::updateAngleDelta(double frequency)
{
    auto cyclesPerSample = frequency / currentSampleRate;
    angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
}
