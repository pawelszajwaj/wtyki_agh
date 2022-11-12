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
class OverdriveAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    OverdriveAudioProcessorEditor(OverdriveAudioProcessor&);
    ~OverdriveAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    void onStateSwitch();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OverdriveAudioProcessor& audioProcessor;
    juce::Slider qStep;
    juce::ToggleButton turnOn;
    void sliderValueChanged(juce::Slider* slider) override;

    juce::Slider drywet;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OverdriveAudioProcessorEditor)
};
