/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Filtr_dpAudioProcessor::Filtr_dpAudioProcessor()
{
}


Filtr_dpAudioProcessor::~Filtr_dpAudioProcessor()
{
}

//==============================================================================
const juce::String Filtr_dpAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Filtr_dpAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Filtr_dpAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Filtr_dpAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Filtr_dpAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Filtr_dpAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Filtr_dpAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Filtr_dpAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Filtr_dpAudioProcessor::getProgramName (int index)
{
    return {};
}

void Filtr_dpAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Filtr_dpAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec psp;
    psp.sampleRate = sampleRate;
    psp.numChannels = getMainBusNumOutputChannels();
    psp.maximumBlockSize = samplesPerBlock;
    lFilter.prepare(psp);
    lFilter.reset();
    lFilter.setMode(juce::dsp::LadderFilterMode::LPF24);
    lFilter.setCutoffFrequencyHz(freq);
    lFilter.setResonance(0.7f);
    lFilter.setDrive(1.0f);

    freq = 400;
    turnedOn = false;

}

void Filtr_dpAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Filtr_dpAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Filtr_dpAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    juce::dsp::AudioBlock<float> block(buffer);
    lFilter.setEnabled(turnedOn);
    lFilter.setCutoffFrequencyHz(freq);
    lFilter.process(juce::dsp::ProcessContextReplacing<float>(block)); 
}

//==============================================================================
bool Filtr_dpAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Filtr_dpAudioProcessor::createEditor()
{
    return new Filtr_dpAudioProcessorEditor (*this);
}

//==============================================================================
void Filtr_dpAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Filtr_dpAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Filtr_dpAudioProcessor();
}
