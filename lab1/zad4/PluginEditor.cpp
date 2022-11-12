/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Zad4AudioProcessorEditor::Zad4AudioProcessorEditor (Zad4AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    levelSlider.addListener(this);
       
    levelSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    levelSlider.setRange(0.0, 1.0, 0.001);
    levelSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    levelSlider.setPopupDisplayEnabled(true, false, this);
    levelSlider.setTextValueSuffix(" Volume");
    levelSlider.setValue(0.0);
    
    freqSlider.addListener(this);
    freqSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    freqSlider.setRange(220, 880, 0.1);
    freqSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    freqSlider.setPopupDisplayEnabled(true, false, this);
    freqSlider.setTextValueSuffix(" Frequency");
    freqSlider.setValue(440.0);
    

    addAndMakeVisible(levelSlider);
    addAndMakeVisible(freqSlider);

    setSize(400, 300);
}

Zad4AudioProcessorEditor::~Zad4AudioProcessorEditor()
{
}

//==============================================================================
void Zad4AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello!", getLocalBounds(), juce::Justification::centred, 1);
}

void Zad4AudioProcessorEditor::resized()
{
    levelSlider.setBounds(40, 30, 20, getHeight() - 60);
    freqSlider.setBounds(80, 30, 20, getHeight() - 60);
}

void Zad4AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.gain->setValueNotifyingHost(levelSlider.getValue());
    audioProcessor.frequency = freqSlider.getValue();
}
