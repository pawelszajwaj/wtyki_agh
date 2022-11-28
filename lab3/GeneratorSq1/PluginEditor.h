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
class GeneratorWt1AudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Timer, private juce::Slider::Listener
{
public:
   GeneratorWt1AudioProcessorEditor (GeneratorWt1AudioProcessor&);
    ~GeneratorWt1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
   GeneratorWt1AudioProcessor& audioProcessor;

    void timerCallback() override
    {
        keyboardComponent.grabKeyboardFocus();
        stopTimer();
    }
    juce::MidiKeyboardComponent keyboardComponent;
    juce::Slider pitchBend;
    void sliderValueChanged(juce::Slider* slider) override;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneratorWt1AudioProcessorEditor)
};
