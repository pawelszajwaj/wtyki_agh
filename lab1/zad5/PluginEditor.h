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
class Zad5AudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Slider::Listener
{
public:
    Zad5AudioProcessorEditor (Zad5AudioProcessor&);
    ~Zad5AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Zad5AudioProcessor& audioProcessor;

    juce::Slider midiVolume;

    void sliderValueChanged(juce::Slider* slider) override;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Zad5AudioProcessorEditor)
};
