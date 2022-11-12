/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Zad3AudioProcessorEditor::Zad3AudioProcessorEditor (Zad3AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    levelSlider.addListener(this);

    levelSlider.setRange(0.0, 0.25);
    levelSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    levelSlider.setRange(0.0, 1.0, 0.001);
    levelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    levelSlider.setPopupDisplayEnabled(true, false, this);
    levelSlider.setTextValueSuffix("Volume");
    levelSlider.setValue(0.0);

    addAndMakeVisible(levelSlider);

    setSize(400, 300);
}

Zad3AudioProcessorEditor::~Zad3AudioProcessorEditor()
{
}

//==============================================================================
void Zad3AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("BOOM", getLocalBounds(), juce::Justification::centred, 1);
}

void Zad3AudioProcessorEditor::resized()
{
    levelSlider.setBounds(40, 30, 20, getHeight() - 60);
}
void Zad3AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.gain->setValueNotifyingHost(levelSlider.getValue());
}