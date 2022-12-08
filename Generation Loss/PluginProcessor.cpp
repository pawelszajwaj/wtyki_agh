/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GenerationLossAudioProcessor::GenerationLossAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), lowPassFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 20000.0f, 0.1f)),
    highPassFilter(juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 20.0f, 0.1f)),
    pinkNoiseFilter(juce::dsp::IIR::Coefficients<float>::makeLowShelf(getSampleRate(), 10000.0f, 0.1f, 3))

#endif
{
}

GenerationLossAudioProcessor::~GenerationLossAudioProcessor()
{
}

//==============================================================================
const juce::String GenerationLossAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GenerationLossAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GenerationLossAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GenerationLossAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GenerationLossAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GenerationLossAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GenerationLossAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GenerationLossAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GenerationLossAudioProcessor::getProgramName (int index)
{
    return {};
}

void GenerationLossAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GenerationLossAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    lowPassFilter.prepare(spec);
    lowPassFilter.reset();
    highPassFilter.prepare(spec);
    highPassFilter.reset();
    pinkNoiseFilter.prepare(spec);
    pinkNoiseFilter.reset();

    tempBuffer.clear();
    interpolationBuffer.clear();
    helpmeBuffer.clear();

    prepareInterpolate();

    smoother.reset(getSampleRate(), 2/ getSampleRate());
}

void GenerationLossAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GenerationLossAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void GenerationLossAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    tempBuffer.makeCopyOf(buffer);
    tempBuffer.clear();

    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        for (auto channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            auto currentSample = buffer.getSample(channel, sample);
            buffer.setSample(channel, sample, (std::floor((currentSample + 1) * pow(2, quantizationSteps))) / pow(2, quantizationSteps) - 1);
        }
    }

    //////interpolation
    Interpolate(buffer);

    ////////////////////

    // after this line only addons like saw and noise can be affected
    // if you want to edit signal, do it before this line
    juce::dsp::AudioBlock<float> block(buffer);
    lowPassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    highPassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    updateFilter();

    auto* channelData = buffer.getWritePointer(0, 0);
    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float randomSample = ((juce::Random::getSystemRandom().nextFloat() * 2) - 1) * pinkNoiseLevel;
        tempBuffer.addSample(0, sample, randomSample);
        tempBuffer.addSample(1, sample, randomSample);
    }

    juce::dsp::AudioBlock<float> blockNoise(tempBuffer);
    pinkNoiseFilter.process(juce::dsp::ProcessContextReplacing<float>(blockNoise));

    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        buffer.addSample(0, sample, tempBuffer.getSample(0, sample));
        buffer.addSample(1, sample, tempBuffer.getSample(1, sample));
    }

    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float sawSample = ((sawDelta / (getSampleRate() / 60)) * 2 - 1) * sawLevel;
        buffer.addSample(0, sample, sawSample);
        buffer.addSample(1, sample, sawSample);
        sawDelta++;
        if (sawDelta == (getSampleRate() / 60))
            sawDelta = 0;
    }


}

//==============================================================================
bool GenerationLossAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GenerationLossAudioProcessor::createEditor()
{
    return new GenerationLossAudioProcessorEditor (*this);
}

//==============================================================================
void GenerationLossAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GenerationLossAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GenerationLossAudioProcessor();
}

void GenerationLossAudioProcessor::updateFilter() 
{
    // add Q (currently 0.1f) as variable later and silder as well
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), lpfreq, 0.1f);
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), hpfreq, 0.1f);
    *pinkNoiseFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(getSampleRate(), 10000.0f, 0.1f, 0.1);
}


void GenerationLossAudioProcessor::prepareInterpolate()
{
    for (int i = 0; i < sizeof(flutterLengthArray)/sizeof(flutterLengthArray[0]); i++)
    {
        flutterSpeedArray[i] = std::sin(i * juce::MathConstants<double>::pi / (sizeof(flutterSpeedArray)/sizeof(flutterSpeedArray[0]) / 2)
            - juce::MathConstants<double>::pi);
        flutterSpeedArray[i] = pow(2, flutterSpeedArray[i] / 48);
        flutterLengthArray[i] = ceil(480 * flutterSpeedArray[i]);
        if (i > 37)
            flutterLengthArray[i] -= 1;
    }
}

void GenerationLossAudioProcessor::Interpolate(juce::AudioBuffer<float>& buffer)
{
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    interpolationBuffer.setSize(totalNumOutputChannels, flutterLengthArray[currentInterpolationStep]);

    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        for (auto channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            smoother = buffer.getSample(channel, sample);
            interpolationMemory[channel][sample + interpolationMemoryIndex] = smoother.getNextValue();
        }
    }

    interpolationMemoryIndex += buffer.getNumSamples() - interpolationBuffer.getNumSamples();
    
    for (auto sample = 0; sample < interpolationBuffer.getNumSamples(); ++sample)
    {
        for (auto channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            interpolationBuffer.setSample(channel, sample, interpolationMemory[channel][sample]);
        }
    }

    interpolator.process(flutterSpeedArray[currentInterpolationStep], *interpolationBuffer.getArrayOfReadPointers(),
        *buffer.getArrayOfWritePointers(), buffer.getNumSamples(), flutterLengthArray[currentInterpolationStep], 0);

    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        buffer.setSample(1, sample, buffer.getSample(0, sample));
    }

    for (auto sample = 0; sample < interpolationMemoryIndex; ++sample)
    {
        for (auto channel = 0; channel < totalNumOutputChannels; ++channel)
        {
            interpolationMemory[channel][sample] = interpolationMemory[channel][interpolationBuffer.getNumSamples() + sample];
        }
    }

    currentInterpolationStep++;
    if (currentInterpolationStep == sizeof(flutterLengthArray)/sizeof(flutterLengthArray[0]))
        currentInterpolationStep = 0;
    
}

