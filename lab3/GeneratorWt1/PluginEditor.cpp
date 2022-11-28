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
    addAndMakeVisible(keyboardComponent);
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
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void GeneratorWt1AudioProcessorEditor::resized()
{
    keyboardComponent.setBounds(10, 10, getWidth() - 20, getHeight() - 20);
}

