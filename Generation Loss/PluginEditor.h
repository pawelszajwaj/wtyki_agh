/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GenerationLossAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    GenerationLossAudioProcessorEditor (GenerationLossAudioProcessor&);
    ~GenerationLossAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::Slider sliderLowPassFilter;
    juce::Slider sliderHighPassFilter;
    juce::Slider sliderPinkNoise;
    juce::Slider sliderSawLevel;
    juce::Slider sliderQuantization;

    void sliderValueChanged(juce::Slider* slider) override;

    juce::Image background;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GenerationLossAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenerationLossAudioProcessorEditor)
};
