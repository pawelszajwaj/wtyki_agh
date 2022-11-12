/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Zad5AudioProcessorEditor::Zad5AudioProcessorEditor (Zad5AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    midiVolume.addListener(this);

    midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    midiVolume.setRange(0.0, 127.0, 1.0);
    midiVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    midiVolume.setPopupDisplayEnabled(true, false, this);
    midiVolume.setTextValueSuffix(" Velocity");
    midiVolume.setValue(1.0);

    addAndMakeVisible(&midiVolume);


    setSize (400, 300);
}

Zad5AudioProcessorEditor::~Zad5AudioProcessorEditor()
{
}

//==============================================================================
void Zad5AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void Zad5AudioProcessorEditor::resized()
{
    midiVolume.setBounds(40, 30, 20, getHeight() - 60);
}

void Zad5AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.noteOnVel = midiVolume.getValue();
}