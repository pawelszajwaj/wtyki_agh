/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Zad1AudioProcessor::Zad1AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

Zad1AudioProcessor::~Zad1AudioProcessor()
{
}

//==============================================================================
const juce::String Zad1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Zad1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Zad1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Zad1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Zad1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Zad1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Zad1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Zad1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Zad1AudioProcessor::getProgramName (int index)
{
    return {};
}

void Zad1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Zad1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Zad1AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Zad1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Zad1AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
                    sample = 0;
                    if (channelData[smp] > 0)
                    {
                        sample = std::ceil(channelData[smp] * steps) / steps * wetdryMix/100; //dry
                        sample += (channelData[smp]) * (1 - wetdryMix/100); //wet
                        channelData[smp] = sample; //out
                    }
                    if (channelData[smp] < 0)
                    {
                        sample = std::floor(channelData[smp] * steps) / steps * wetdryMix/100; //dry
                        sample += (channelData[smp]) * (1 - wetdryMix/100); //wet
                        sample = (channelData[smp]);
                        channelData[smp] = sample; //out
                    }

                }
            }
        }
    }

}

//==============================================================================
bool Zad1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Zad1AudioProcessor::createEditor()
{
    return new Zad1AudioProcessorEditor (*this);
}

//==============================================================================
void Zad1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Zad1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Zad1AudioProcessor();
}
