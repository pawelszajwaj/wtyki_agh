/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Zad2AudioProcessor::Zad2AudioProcessor()
{
    addParameter(gain = new juce::AudioParameterFloat("gain", "Gain", 0.0f, 1.0f, 0.5f));              
    addParameter(invertPhase = new juce::AudioParameterBool("invertPhase", "Invert Phase", false));
}

Zad2AudioProcessor::~Zad2AudioProcessor()
{
}

//==============================================================================
const juce::String Zad2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Zad2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Zad2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Zad2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Zad2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Zad2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Zad2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Zad2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Zad2AudioProcessor::getProgramName (int index)
{
    return {};
}

void Zad2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Zad2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto phase = *invertPhase ? -1.0f : 1.0f;
    previousGain = *gain * phase;
}

void Zad2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Zad2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Zad2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto phase = *invertPhase ? -1.0f : 1.0f;
    auto currentGain = *gain * phase;
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
}

//==============================================================================
bool Zad2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Zad2AudioProcessor::createEditor()
{
    return new Zad2AudioProcessorEditor (*this);
}

//==============================================================================
void Zad2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml(new juce::XmlElement
        ("Z2Tlumik"));
    xml->setAttribute("gain", (double)*gain);
    xml->setAttribute("invertPhase", *invertPhase);
    copyXmlToBinary(*xml, destData);
}

void Zad2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary 
        (data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName("W2Tlumik"))
        {
            *gain = (float)xmlState->getDoubleAttribute("gain", 1.0);
            *invertPhase = xmlState->getBoolAttribute("invertPhase", false);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Zad2AudioProcessor();
}
