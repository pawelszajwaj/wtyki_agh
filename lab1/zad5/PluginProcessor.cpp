/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Zad5AudioProcessor::Zad5AudioProcessor()
{
    addParameter(gain = new juce::AudioParameterFloat("gain", "Gain", 0.0f, 127.0f, 80.0f));
    //frequency = 0;
    //noteOnVel = 0;
}

Zad5AudioProcessor::~Zad5AudioProcessor()
{
}

//==============================================================================
const juce::String Zad5AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Zad5AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Zad5AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Zad5AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Zad5AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Zad5AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Zad5AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Zad5AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Zad5AudioProcessor::getProgramName (int index)
{
    return {};
}

void Zad5AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Zad5AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    updateAngleDelta(440); 
}

void Zad5AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Zad5AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Zad5AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    juce::MidiBuffer processedMidi;
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        const auto time = metadata.samplePosition;
        if (message.isNoteOn())
        {
            amplitude = 1;
            message = juce::MidiMessage::noteOn(message.getChannel(),
                message.getNoteNumber(), (juce::uint8)noteOnVel);
            frequency = message.getMidiNoteInHertz(message.getNoteNumber());
        }
        else if (message.isNoteOff())
        {
            amplitude = 0;
        }
        processedMidi.addEvent(message, time);


    }
    midiMessages.swapWith(processedMidi);

    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        auto currentSample = (float)std::sin(currentAngle);
        currentAngle += angleDelta;
        updateAngleDelta(frequency);
        float randomSample = currentSample * amplitude * noteOnVel / 127;
        buffer.addSample(0, sample, randomSample);
        buffer.addSample(1, sample, randomSample);
    }


}

//==============================================================================
bool Zad5AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Zad5AudioProcessor::createEditor()
{
    return new Zad5AudioProcessorEditor (*this);
}

//==============================================================================
void Zad5AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Zad5AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Zad5AudioProcessor();
}

void Zad5AudioProcessor::updateAngleDelta(double frequency)
{
    auto cyclesPerSample = frequency / currentSampleRate;
    angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
}