/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Filtr_dpAudioProcessorEditor::Filtr_dpAudioProcessorEditor (Filtr_dpAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(200, 200);

    fFreq.setSliderStyle(juce::Slider::LinearBarVertical);
    fFreq.setRange(500.0, 10000.0, 10.0);
    fFreq.setTextBoxStyle(juce::Slider::NoTextBox, false, 90, 0);
    fFreq.setPopupDisplayEnabled(true, false, this);
    fFreq.setTextValueSuffix(" Hz");
    fFreq.setValue(1.0);
    addAndMakeVisible(&fFreq);
    fFreq.addListener(this);

    qStep.addListener(this);
    turnOn.setButtonText("On");
    turnOn.setToggleState(false, juce::sendNotification);
    addAndMakeVisible(&turnOn);
    {
        turnOn.onClick = [this] {Filtr_dpAudioProcessorEditor::onStateSwitch(); };
    }
}

Filtr_dpAudioProcessorEditor::~Filtr_dpAudioProcessorEditor()
{
}

//==============================================================================
void Filtr_dpAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Riot Freq", getLocalBounds(), juce::Justification::centred, 1);
}

void Filtr_dpAudioProcessorEditor::resized()
{
    fFreq.setBounds(40, 30, 20, getHeight() - 60);
    turnOn.setBounds(getWidth() - 80, 30, 80, 30);
}

void Filtr_dpAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.freq = fFreq.getValue();
}

void Filtr_dpAudioProcessorEditor::onStateSwitch()
{
    auto state = turnOn.getToggleState();
    audioProcessor.turnedOn = (bool)state;
}