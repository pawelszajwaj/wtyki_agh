/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GeneratorWt1AudioProcessorEditor::GeneratorWt1AudioProcessorEditor(GeneratorWt1AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    keyboardComponent(audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)

{
    //addAndMakeVisible(keyboardComponent);


    pitchBend.setSliderStyle(juce::Slider::Rotary);
    pitchBend.setRange(-2.0, 2.0, 0.01);
    pitchBend.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    pitchBend.setPopupDisplayEnabled(true, false, this);
    pitchBend.setTextValueSuffix(" Neely");
    pitchBend.setValue(16.0);
    addAndMakeVisible(&pitchBend);
    pitchBend.addListener(this);


    setSize(600, 160);
    startTimer(400);

}

GeneratorWt1AudioProcessorEditor::~GeneratorWt1AudioProcessorEditor()
{
}

//==============================================================================
void GeneratorWt1AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Yare yare daze", getLocalBounds(), juce::Justification::centred, 1);
}

void GeneratorWt1AudioProcessorEditor::resized()
{
    keyboardComponent.setBounds(10, 10, getWidth() - 20, getHeight() - 20);
    pitchBend.setBounds(30, 30, 100, 100);
}

void  GeneratorWt1AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.steps = pitchBend.getValue();
}


