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
class GeneratorADSRAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Timer, private juce::Slider::Listener
{
public:
   GeneratorADSRAudioProcessorEditor (GeneratorADSRAudioProcessor&);
    ~GeneratorADSRAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
   GeneratorADSRAudioProcessor& audioProcessor;

    void timerCallback() override
    {
        keyboardComponent.grabKeyboardFocus();
        stopTimer();
    }

    juce::MidiKeyboardComponent keyboardComponent;
    juce::Slider sliderAttack;
    juce::Slider sliderDecay;
    juce::Slider sliderSustain;
    juce::Slider sliderRelease;

    void sliderValueChanged(juce::Slider* slider) override;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneratorADSRAudioProcessorEditor)
};
