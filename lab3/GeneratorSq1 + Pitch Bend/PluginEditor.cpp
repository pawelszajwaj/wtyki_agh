/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GeneratorSqAudioProcessorEditor::GeneratorSqAudioProcessorEditor(GeneratorSqAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    keyboardComponent(audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)

{
    //addAndMakeVisible(keyboardComponent);


    pitchBend.setSliderStyle(juce::Slider::Rotary);
    pitchBend.setRange(-2.0, 2.0, 0.25);
    pitchBend.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    pitchBend.setPopupDisplayEnabled(true, false, this);
    pitchBend.setTextValueSuffix(" Neely");
    pitchBend.setValue(0.0);
    addAndMakeVisible(&pitchBend);
    pitchBend.addListener(this);


    setSize(600, 160);
    startTimer(400);

}

GeneratorSqAudioProcessorEditor::~GeneratorSqAudioProcessorEditor()
{
}

//==============================================================================
void GeneratorSqAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Yare yare daze", getLocalBounds(), juce::Justification::centred, 1);
}

void GeneratorSqAudioProcessorEditor::resized()
{
    keyboardComponent.setBounds(10, 10, getWidth() - 20, getHeight() - 20);
    pitchBend.setBounds(30, 30, 100, 100);
}

void  GeneratorSqAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.procpitchBend = pow(2, (pitchBend.getValue() / 12)) / 1;
}


