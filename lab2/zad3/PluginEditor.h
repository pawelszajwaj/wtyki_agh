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
class Filtr_dpAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    Filtr_dpAudioProcessorEditor (Filtr_dpAudioProcessor&);
    ~Filtr_dpAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void onStateSwitch();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Filtr_dpAudioProcessor& audioProcessor;

    juce::Slider fFreq;
    void sliderValueChanged(juce::Slider* slider) override;

    juce::ToggleButton turnOn;
    juce::Slider qStep;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filtr_dpAudioProcessorEditor)
};
