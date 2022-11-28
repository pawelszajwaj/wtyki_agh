/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GeneratorWt1AudioProcessor::GeneratorWt1AudioProcessor() : synthAudioSource(keyboardState)
#ifndef JucePlugin_PreferredChannelConfigurations
     , AudioProcessor (BusesProperties()
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

GeneratorWt1AudioProcessor::~GeneratorWt1AudioProcessor()
{
}

//==============================================================================
const juce::String GeneratorWt1AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GeneratorWt1AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GeneratorWt1AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GeneratorWt1AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GeneratorWt1AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GeneratorWt1AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GeneratorWt1AudioProcessor::getCurrentProgram()
{
    return 0;
}

void GeneratorWt1AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GeneratorWt1AudioProcessor::getProgramName (int index)
{
    return {};
}

void GeneratorWt1AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GeneratorWt1AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synthAudioSource.prepareToPlay(samplesPerBlock, sampleRate);
    synthAudioSource.midiCollector.reset(sampleRate);
}

void GeneratorWt1AudioProcessor::releaseResources()
{
    synthAudioSource.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GeneratorWt1AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GeneratorWt1AudioProcessor::processBlock (juce::AudioBuffer<float>&buffer, juce::MidiBuffer & midiMessages)
{
    juce::MidiMessage m;
    int time;
    for (juce::MidiBuffer::Iterator i(midiMessages); 
        i.getNextEvent(m, time);)
        synthAudioSource.midiCollector.addMessageToQueue(m);
    juce::AudioSourceChannelInfo channelInfo = juce::AudioSourceChannelInfo(buffer);
    synthAudioSource.getNextAudioBlock(channelInfo);
}

//==============================================================================
bool GeneratorWt1AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GeneratorWt1AudioProcessor::createEditor()
{
    return new GeneratorWt1AudioProcessorEditor (*this);
}

//==============================================================================
void GeneratorWt1AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GeneratorWt1AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GeneratorWt1AudioProcessor();
}
