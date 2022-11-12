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
class Zad1AudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    Zad1AudioProcessorEditor (Zad1AudioProcessor&);
    ~Zad1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void onStateSwitch();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Zad1AudioProcessor& audioProcessor;
    juce::Slider qStep;
    juce::ToggleButton turnOn;
    void sliderValueChanged(juce::Slider* slider) override;

    juce::Slider drywet;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Zad1AudioProcessorEditor)
};
