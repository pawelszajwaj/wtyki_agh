/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class GenerationLossAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    GenerationLossAudioProcessor();
    ~GenerationLossAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // filters
    float lpfreq = 20000.0f, hpfreq = 20.0f;
    void updateFilter();

    // pink Noise
    float pinkNoiseLevel = 0.0f;

    // saw sound 60 Hz
    float sawLevel = 0.0f;
    int sawDelta = 0;

    // quantize 
    int quantizationSteps = 8;

    //flutter
    juce::CatmullRomInterpolator interpolator;
    float flutterLengthArray[74];
    float flutterSpeedArray[74];
    float interpolationMemory[2][1024];
    void prepareInterpolate();
    int currentInterpolationStep = 0;
    int interpolationMemoryIndex = 0;
    void Interpolate(juce::AudioBuffer<float>& buffer);
    bool isInterpolationOn = false;

private:

    juce::LinearSmoothedValue<float> smoother{ 0.0 };

    juce::AudioBuffer<float> tempBuffer;
    juce::AudioBuffer<float> interpolationBuffer;
    juce::AudioBuffer<float> helpmeBuffer;

    juce::dsp::ProcessorDuplicator < juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowPassFilter;
    juce::dsp::ProcessorDuplicator < juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highPassFilter;
    juce::dsp::ProcessorDuplicator < juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> pinkNoiseFilter;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenerationLossAudioProcessor)
};
