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
class Generator1AudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    Generator1AudioProcessorEditor (Generator1AudioProcessor&);
    ~Generator1AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Generator1AudioProcessor& audioProcessor;

    void timerCallback() override
    {
        keyboardComponent.grabKeyboardFocus();
        stopTimer();
    }
    juce::MidiKeyboardComponent keyboardComponent;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Generator1AudioProcessorEditor)
};
