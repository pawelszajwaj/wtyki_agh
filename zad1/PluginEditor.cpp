/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Zad1AudioProcessorEditor::Zad1AudioProcessorEditor (Zad1AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    midiVolume.addListener(this);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 200);

    // MOD 3 parametry suwaka
    midiVolume.setSliderStyle(juce::Slider::LinearBarVertical);
    midiVolume.setRange(0.0, 127.0, 1.0);
    midiVolume.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    midiVolume.setPopupDisplayEnabled(true, false, this);
    midiVolume.setTextValueSuffix(" Velocity");
    midiVolume.setValue(1.0);
    // MOD 4 dodanie suwaka do edytora
    addAndMakeVisible(&midiVolume);
}

Zad1AudioProcessorEditor::~Zad1AudioProcessorEditor()
{
}

//==============================================================================
void Zad1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // MOD 6 wypelnienie calego okna
    g.fillAll(juce::Colours::white);
    // MOD 7 kolor rysowania
    g.setColour(juce::Colours::black);
    // MOD 8 nowy tekst
    g.setFont(15.0f);
    g.drawFittedText("Fix MIDI Velocity", 0, 0, getWidth(), 30,
        juce::Justification::centred, 1);
}

void Zad1AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    midiVolume.setBounds(40, 30, 20, getHeight() - 60);
}

void Zad1AudioProcessorEditor::sliderValueChanged(juce::Slider * slider)
{
    audioProcessor.noteOnVel = midiVolume.getValue();
}
