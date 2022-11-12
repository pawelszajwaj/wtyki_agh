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
class Zad3AudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Slider::Listener
{
public:
    Zad3AudioProcessorEditor (Zad3AudioProcessor&);
    ~Zad3AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    void sliderValueChanged(juce::Slider* slider) override;
    Zad3AudioProcessor& audioProcessor;
    juce::Random random;
    juce::Slider levelSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Zad3AudioProcessorEditor)
};
