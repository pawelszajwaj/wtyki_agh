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
class Zad2AudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    Zad2AudioProcessorEditor (Zad2AudioProcessor&);
    ~Zad2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void sliderValueChanged(juce::Slider* slider); //override;

    Zad2AudioProcessor& audioProcessor;
    juce::Slider VolumeSlider;

    void boolValueChanged();

    juce::ToggleButton phaseButton {"Invert Phase"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Zad2AudioProcessorEditor)
};
