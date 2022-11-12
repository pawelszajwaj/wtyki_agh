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
class Zad4AudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Slider::Listener
{
public:
    Zad4AudioProcessorEditor (Zad4AudioProcessor&);
    ~Zad4AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Zad4AudioProcessor& audioProcessor;
    juce::Random random;
    juce::Slider levelSlider;
    juce::Slider freqSlider;

    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Zad4AudioProcessorEditor)
};
